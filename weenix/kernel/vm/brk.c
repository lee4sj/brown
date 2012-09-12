#include "globals.h"
#include "errno.h"
#include "util/debug.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/mman.h"
#include "mm/tlb.h"

#include "vm/mmap.h"
#include "vm/vmmap.h"

#include "proc/proc.h"

/*
 * This function implements the brk(2) system call.
 *
 * This routine manages the calling process's "break" -- the ending address
 * of the process's "dynamic" region (often also referred to as the "heap").
 * The current value of a process's break is maintained in the 'p_brk' member
 * of the proc_t structure that represents the process in question.
 *
 * The 'p_brk' and 'p_start_brk' members of a proc_t struct are initialized
 * by the loader. 'p_start_brk' is subsequently never modified; it always
 * holds the initial value of the break. Note that the starting break is
 * not necessarily page aligned!
 *
 * 'p_start_brk' is the lower limit of 'p_brk' (that is, setting the break
 * to any value less than 'p_start_brk' should be disallowed).
 *
 * The upper limit of 'p_brk' is defined by the minimum of (1) the
 * starting address of the next occuring mapping or (2) USER_MEM_HIGH.
 * That is, growth of the process break is limited only in that it cannot
 * overlap with/expand into an existing mapping or beyond the region of
 * the address space allocated for use by userland. (note the presence of
 * the 'vmmap_is_range_empty' function).
 *
 * The dynamic region should always be represented by at most ONE vmarea.
 * Note that vmareas only have page granularity, you will need to take this
 * into account when deciding how to set the mappings if p_brk or p_start_brk
 * is not page aligned.
 *
 * You are guaranteed that the process data/bss region is non-empty.
 * That is, if the starting brk is not page-aligned, its page has
 * read/write permissions.
 *
 * If addr is NULL, you should NOT fail as the man page says. Instead,
 * "return" the current break. We use this to implement sbrk(0) without writing
 * a separate syscall. Look in user/libc/syscall.c if you're curious.
 *
 * Also, despite the statement on the manpage, you MUST support combined use
 * of brk and mmap in the same process.
 *
 * Note that this function "returns" the new break through the "ret" argument.
 * Return 0 on success, -errno on failure.
 */
int
do_brk(void *addr, void **ret)
{
	dbg(DBG_VM, "\n");

	void *cur_sbrk = curproc->p_start_brk;
	vmarea_t *vma;
	vmarea_t *nvma = NULL;

	if (addr == NULL) {
		*ret = curproc->p_brk;
		return 0;
	}

	KASSERT((uintptr_t)curproc->p_brk >= (uintptr_t)curproc->p_start_brk);

	if ((uintptr_t)cur_sbrk > (uintptr_t)addr) {
		return -ENOMEM;
	}

	/* check for upper limits */
	if ((uintptr_t)addr >= USER_MEM_HIGH)
		return -ENOMEM;

	uintptr_t pbrk_pg = ADDR_TO_PN(curproc->p_brk);
	uintptr_t addr_pg = ADDR_TO_PN(addr);
	(PAGE_ALIGNED(addr))? (addr_pg) : (addr_pg++);
	(PAGE_ALIGNED(curproc->p_brk))? (pbrk_pg) : (pbrk_pg++);

	/* if they resides in the same page, just update p_brk */
	if (pbrk_pg == addr_pg) {
		curproc->p_brk = addr;
		*ret = addr;
		return 0;
	}

	/* Get dynamic region's vmarea */
	vma = vmmap_lookup(curproc->p_vmmap, ADDR_TO_PN(cur_sbrk));
	KASSERT(vma && "vmarea for the dynamic region is not found!");

	/* check to see if vma has a next vma */
	if (vma->vma_plink.l_next != &curproc->p_vmmap->vmm_list) {
		nvma = list_item(vma->vma_plink.l_next, vmarea_t, vma_plink);
		KASSERT(nvma &&
			"next vmarea should exist but could not be found!");
	}

	/* check for upper limits */
	if (nvma && addr_pg > nvma->vma_start)
		return -ENOMEM;

	/* Now, update the vma, and curpor->p_brk */
	if (pbrk_pg > addr_pg) {
		vmmap_remove(curproc->p_vmmap, addr_pg, (pbrk_pg - addr_pg));
		tlb_flush_range((uintptr_t)PN_TO_ADDR(addr_pg), pbrk_pg - addr_pg);
	} else {
		vma->vma_end = addr_pg;
	}

	curproc->p_brk = addr;
	*ret = addr;

	return 0;
}
