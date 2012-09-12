#include "types.h"
#include "globals.h"
#include "kernel.h"
#include "errno.h"

#include "util/debug.h"

#include "proc/proc.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/page.h"
#include "mm/mmobj.h"
#include "mm/pframe.h"
#include "mm/pagetable.h"

#include "vm/pagefault.h"
#include "vm/vmmap.h"

/*
 * This gets called by _pt_fault_handler in mm/pagetable.c The
 * calling function has already done a lot of error checking for
 * us. In particular it has checked that we are not page faulting
 * while in kernel mode. Make sure you understand why an
 * unexpected page fault in kernel mode is bad in Weenix. You
 * should probably read the _pt_fault_handler function to get a
 * sense of what it is doing.
 *
 * Before you can do anything you need to find the vmarea that
 * contains the address that was faulted on. Make sure to check
 * the permissions on the area to see if the process has
 * permission to do [cause]. If either of these checks does not
 * pass kill the offending process, setting its exit status to
 * EFAULT (normally we would send the SIGSEGV signal, however
 * Weenix does not support signals).
 *
 * Now it is time to find the correct page (don't forget
 * about shadow objects, especially copy-on-write magic!). Make
 * sure that if the user writes to the page it will be handled
 * correctly.
 *
 * Finally call pt_map to have the new mapping placed into the
 * appropriate page table.
 *
 * @param vaddr the address that was accessed to cause the fault
 *
 * @param cause this is the type of operation on the memory
 *              address which caused the fault, possible values
 *              can be found in pagefault.h
 */
void
handle_pagefault(uintptr_t vaddr, uint32_t cause)
{
	dbg(DBG_BRK, "access to %x, cause=%u\n", vaddr, cause);

	vmarea_t *vma;
	mmobj_t *o;
	pframe_t *pf;
	int ret;

	if (vaddr == 0) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	vma = vmmap_lookup(curproc->p_vmmap, ADDR_TO_PN(vaddr));
	if (!vma) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	/* error checking on permissions */
	if (!(vma->vma_prot & PROT_READ) && !(vma->vma_prot & PROT_WRITE)) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	if ((cause & FAULT_WRITE) && !(vma->vma_prot & PROT_WRITE)) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	if ((cause & FAULT_EXEC) && !(vma->vma_prot & PROT_EXEC)) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	if ((cause & FAULT_PRESENT) && !(vma->vma_prot & PROT_READ)) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	o = vma->vma_obj;
	KASSERT(o);

	uint32_t rpage = vma->vma_off + (ADDR_TO_PN(vaddr) - vma->vma_start);
	ret = pframe_lookup(o, rpage, (vma->vma_prot & PROT_WRITE)? 1 : 0, &pf);
	if (ret < 0) {
		do_exit(EFAULT);
		panic("This process should have been exited!");
	}

	uint32_t pdflags = PD_PRESENT | PD_USER;
	uint32_t ptflags = PT_PRESENT | PT_USER;
	if (vma->vma_prot & PROT_WRITE) {
		pdflags |= PD_WRITE;
		ptflags |= PT_WRITE;
	}

	if (cause & FAULT_WRITE) {
		pframe_dirty(pf);
	}

	ret = pt_map(curproc->p_pagedir, (uintptr_t)PAGE_ALIGN_DOWN(vaddr),
		     pt_virt_to_phys((uintptr_t)pf->pf_addr), pdflags, ptflags);
	KASSERT(ret == 0);
}
