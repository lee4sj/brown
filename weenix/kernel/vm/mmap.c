#include "globals.h"
#include "errno.h"
#include "types.h"

#include "mm/mm.h"
#include "mm/tlb.h"
#include "mm/mman.h"
#include "mm/page.h"

#include "proc/proc.h"

#include "util/string.h"
#include "util/debug.h"

#include "fs/vnode.h"
#include "fs/vfs.h"
#include "fs/file.h"
#include "fs/stat.h"

#include "vm/vmmap.h"
#include "vm/mmap.h"

/*
 * This function implements the mmap(2) syscall, but only
 * supports the MAP_SHARED, MAP_PRIVATE, MAP_FIXED, and
 * MAP_ANON flags.
 *
 * Add a mapping to the current process's address space.
 * You need to do some error checking; see the ERRORS section
 * of the manpage for the problems you should anticipate.
 * After error checking most of the work of this function is
 * done by vmmap_map(), but remember to clear the TLB.
 */
int
do_mmap(void *addr, size_t len, int prot, int flags,
        int fd, off_t off, void **ret)
{
	dbg(DBG_VM, "\n");

	file_t *f = NULL;
	vnode_t *vn = NULL;
	size_t npages;
	int r;

	if ((fd < 0 || fd >= NFILES) && !((flags & MAP_ANON) && fd == -1))
		return -EBADF;

	if (!PAGE_ALIGNED(addr) ||
	    len == 0 ||
	    ((flags & MAP_TYPE) == MAP_TYPE) || ((flags & MAP_TYPE) == 0))
		return -EINVAL;

	if (len >= (USER_MEM_HIGH - USER_MEM_LOW))
		return -EINVAL;

	if ((flags & MAP_FIXED) || addr != NULL) {
		if (!(USER_MEM_LOW <= (uintptr_t)addr && (uintptr_t)addr < USER_MEM_HIGH))
			return -EINVAL;
	}

	if (fd != -1) {
		f = fget(fd);
		if (f == NULL)
			return -EBADF;

		if ((!S_ISREG(f->f_vnode->vn_mode) &&
		     !S_ISCHR(f->f_vnode->vn_mode)) ||
		    ((flags & MAP_PRIVATE) && !(f->f_mode & FMODE_READ)) ||
		    ((flags & MAP_SHARED) && (prot & PROT_WRITE) &&
		     !((f->f_mode & FMODE_WRITE) && (f->f_mode & FMODE_READ))) ||
		    ((prot & PROT_WRITE) && (f->f_mode == FMODE_APPEND))) {
			fput(f);
			return -EACCES;
		}

		if (S_ISREG(f->f_vnode->vn_mode) && f->f_vnode->vn_len <= off) {
			fput(f);
			return -EINVAL;
		}
	}

	if (f)
		vn = f->f_vnode;

	npages = ADDR_TO_PN((len) + PAGE_OFFSET(addr));
	npages = ADDR_TO_PN(len);
	(PAGE_ALIGNED(len))? npages : (npages++);

	vmarea_t *vma;
	r = vmmap_map(curproc->p_vmmap, vn, ADDR_TO_PN(addr), npages,
		      prot, flags, off, VMMAP_DIR_HILO, &vma);

	if (r < 0) {
		*ret = NULL;
		if (f)
			fput(f);

		return r;
	}

	if (addr && ret)
		*ret = addr;
	else if (ret)
		*ret = PN_TO_ADDR(vma->vma_start);

	if (f)
		fput(f);

	tlb_flush_range((uintptr_t)PAGE_ALIGN_DOWN(*ret), npages);

	return r;
}


/*
 * This function implements the munmap(2) syscall.
 *
 * As with do_mmap() it should perform the required error checking,
 * before calling upon vmmap_remove() to do most of the work.
 * Remember to clear the TLB.
 */
int
do_munmap(void *addr, size_t len)
{
	dbg(DBG_VM, "\n");

	int ret;

	if ((uintptr_t)addr < USER_MEM_LOW || (uintptr_t)addr >= USER_MEM_HIGH ||
	    ((uintptr_t)addr + len) > USER_MEM_HIGH)
		return -EINVAL;

	if (!PAGE_ALIGNED(addr) || len == 0 || addr == NULL)
		return -EINVAL;

	if (len > (USER_MEM_HIGH - USER_MEM_LOW))
		return -EINVAL;

	uint32_t npages = ADDR_TO_PN(len);
	(PAGE_ALIGNED(len))? npages : (npages++);
 	ret = vmmap_remove(curproc->p_vmmap, ADDR_TO_PN(addr), npages);

	tlb_flush_range((uintptr_t)addr, npages);
	return ret;
}

