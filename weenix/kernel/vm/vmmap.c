#include "kernel.h"
#include "errno.h"
#include "globals.h"

#include "vm/vmmap.h"
#include "vm/shadow.h"
#include "vm/anon.h"

#include "proc/proc.h"

#include "util/debug.h"
#include "util/list.h"
#include "util/string.h"
#include "util/printf.h"

#include "fs/vnode.h"
#include "fs/file.h"
#include "fs/fcntl.h"
#include "fs/vfs_syscall.h"

#include "mm/slab.h"
#include "mm/page.h"
#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/mmobj.h"

static slab_allocator_t *vmmap_allocator;
static slab_allocator_t *vmarea_allocator;

void
vmmap_init(void)
{
        vmmap_allocator = slab_allocator_create("vmmap", sizeof(vmmap_t));
        KASSERT(NULL != vmmap_allocator && "failed to create vmmap allocator!");
        vmarea_allocator = slab_allocator_create("vmarea", sizeof(vmarea_t));
        KASSERT(NULL != vmarea_allocator && "failed to create vmarea allocator!");
}

vmarea_t *
vmarea_alloc(void)
{
        vmarea_t *newvma = (vmarea_t *) slab_obj_alloc(vmarea_allocator);
        newvma->vma_vmmap = NULL;
        return newvma;
}

void
vmarea_free(vmarea_t *vma)
{
        KASSERT(NULL != vma);
        slab_obj_free(vmarea_allocator, vma);
}

/* Create a new vmmap, which has no vmareas and does
 * not refer to a process. */
vmmap_t *
vmmap_create(void)
{
	dbg(DBG_VM, "\n");
        vmmap_t *vmmap = (vmmap_t *) slab_obj_alloc(vmmap_allocator);
	if (!vmmap)
		return NULL;

	list_init(&vmmap->vmm_list);
	vmmap->vmm_proc = NULL;
        return vmmap;
}

/* Removes all vmareas from the address space and frees the
 * vmmap struct. */
void
vmmap_destroy(vmmap_t *map)
{
	dbg(DBG_VM, "\n");

	KASSERT(map);

	vmarea_t *vma;
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
		vma->vma_obj->mmo_ops->put(vma->vma_obj);
		if (!list_empty(&vma->vma_olink))
			list_remove(&vma->vma_olink);
		list_remove(&vma->vma_plink);
		vmarea_free(vma);
	} list_iterate_end();

	slab_obj_free(vmmap_allocator, map);
}

/* Add a vmarea to an address space. Assumes (i.e. asserts to some extent)
 * the vmarea is valid.  This involves finding where to put it in the list
 * of VM areas, and adding it. Don't forget to set the vma_vmmap for the
 * area. */
void
vmmap_insert(vmmap_t *map, vmarea_t *newvma)
{
	dbg(DBG_VM, "\n");

	KASSERT(newvma->vma_start > 0);
	KASSERT(newvma->vma_end > newvma->vma_start);

	int added = 0;
	vmarea_t *vma;
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
		if (newvma->vma_end <= vma->vma_start) {
			list_insert_before(&vma->vma_plink, &newvma->vma_plink);
			added = 1;
			goto end;
		}
	} list_iterate_end();

end:
	if (!added) {
		list_insert_tail(&map->vmm_list, &newvma->vma_plink);
	}
}

/* Find a contiguous range of free virtual pages of length npages in
 * the given address space. Returns starting vfn for the range,
 * without altering the map. Returns -1 if no such range exists.
 *
 * Your algorithm should be first fit. If dir is VMMAP_DIR_HILO, you
 * should find a gap as high in the address space as possible; if dir
 * is VMMAP_DIR_LOHI, the gap should be as low as possible. */
int
vmmap_find_range(vmmap_t *map, uint32_t npages, int dir)
{
	dbg(DBG_VM, "\n");

	if (dir == VMMAP_DIR_LOHI)
		goto lohi;

        if (dir == VMMAP_DIR_HILO)
		goto hilo;

	/* At this point, dir != VMMAP_DIR_* */
	panic("wrong dir!");

	uint32_t prev_vfn;
	vmarea_t *vma;
lohi:
	prev_vfn = ADDR_TO_PN(USER_MEM_LOW);
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
		if (vma->vma_start - prev_vfn >= npages) {
			return (int)prev_vfn;
		}
		prev_vfn = vma->vma_end;
	} list_iterate_end();

	if (prev_vfn + npages <= ADDR_TO_PN(USER_MEM_HIGH)
	    && prev_vfn >= ADDR_TO_PN(USER_MEM_LOW))
		return (int)prev_vfn;

	return -1;

hilo:
	prev_vfn = ADDR_TO_PN(USER_MEM_HIGH);
	list_iterate_reverse(&map->vmm_list, vma, vmarea_t, vma_plink) {
		if (prev_vfn - vma->vma_end >= npages) {
			return (int)(prev_vfn - npages);
		}

		prev_vfn = vma->vma_start;
	} list_iterate_end();
	
	if (prev_vfn - npages >= ADDR_TO_PN(USER_MEM_LOW)
	    && prev_vfn <= ADDR_TO_PN(USER_MEM_HIGH))
		return (int)(prev_vfn - npages);

	return -1;
}

/* Find the vm_area that vfn lies in. Simply scan the address space
 * looking for a vma whose range covers vfn. If the page is unmapped,
 * return NULL. */
vmarea_t *
vmmap_lookup(vmmap_t *map, uint32_t vfn)
{
	vmarea_t *vma;
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
		if (vma->vma_start <= vfn && vfn < vma->vma_end)
			return vma;
	} list_iterate_end();

	return NULL;
}

/* Allocates a new vmmap containing a new vmarea for each area in the
 * given map. The areas should have no mmobjs set yet. Returns pointer
 * to the new vmmap on success, NULL on failure. This function is
 * called when implementing fork(2). */
vmmap_t *
vmmap_clone(vmmap_t *map)
{
	dbg(DBG_VM, "\n");

	vmmap_t *newmap = vmmap_create();
	if (!newmap)
		return NULL;

	vmarea_t *vma;
	list_t *newmap_prev;
	vmarea_t *newvma;
	newmap_prev = &newmap->vmm_list;

	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
		dbg(DBG_BRK, "vmmap to %x\n", PN_TO_ADDR(vma->vma_start));
		newvma = vmarea_alloc();
		newvma->vma_obj = NULL;
		newvma->vma_start = vma->vma_start;
		newvma->vma_end = vma->vma_end;
		newvma->vma_off = vma->vma_off;
		newvma->vma_prot = vma->vma_prot;
		newvma->vma_flags = vma->vma_flags;

		newvma->vma_vmmap = newmap;
		list_insert_head(newmap_prev, &newvma->vma_plink);
		newmap_prev = &newvma->vma_plink;

		list_init(&newvma->vma_olink);
	} list_iterate_end();

	return newmap;
}

/* Insert a mapping into the map starting at lopage for npages pages.
 * If lopage is zero, we will find a range of virtual addresses in the
 * process that is big enough, by using vmmap_find_range with the same
 * dir argument.  If lopage is non-zero and the specified region
 * contains another mapping that mapping should be unmapped.
 *
 * If file is NULL an anon mmobj will be used to create a mapping
 * of 0's.  If file is non-null that vnode's file will be mapped in
 * for the given range.  Use the vnode's mmap operation to get the
 * mmobj for the file; do not assume it is file->vn_obj. Make sure all
 * of the area's fields except for vma_obj have been set before
 * calling mmap.
 *
 * If MAP_PRIVATE is specified set up a shadow object for the mmobj.
 *
 * All of the input to this function should be valid (KASSERT!).
 * See mmap(2) for for description of legal input.
 * Note that off should be page aligned.
 *
 * Be very careful about the order operations are performed in here. Some
 * operation are impossible to undo and should be saved until there
 * is no chance of failure.
 *
 * If 'new' is non-NULL a pointer to the new vmarea_t should be stored in it.
 */
int
vmmap_map(vmmap_t *map, vnode_t *file, uint32_t lopage, uint32_t npages,
          int prot, int flags, off_t off, int dir, vmarea_t **new)
{
	dbg(DBG_VM, "\n");

	vmarea_t *vma;
	mmobj_t *obj;
	int ret;

	if (lopage == 0) {
		lopage = vmmap_find_range(map, npages, dir);
		if (lopage == (uint32_t)-1)
			return -ENOMEM;
	}
	else
		vmmap_remove(map, lopage, npages);

	dbg(DBG_BRK, "vmmap to %x\n", PN_TO_ADDR(lopage));
	vma = vmarea_alloc();
	vma->vma_start = lopage;
	vma->vma_end = lopage + npages;
	vma->vma_off = off;
	vma->vma_prot = prot;
	vma->vma_flags = flags;

	vma->vma_vmmap = map;
	vmmap_insert(map, vma);

	if (file) {
		ret = file->vn_ops->mmap(file, vma, &obj);
		if (ret < 0)
			return ret;

		if ((flags & MAP_PRIVATE)) {
			mmobj_t *shadow_obj = shadow_create();
			shadow_obj->mmo_shadowed = obj;
			shadow_obj->mmo_un.mmo_bottom_obj =
				mmobj_bottom_obj(obj);

			vma->vma_obj = shadow_obj;
		} else {
			vma->vma_obj = obj;
		}
	} else {
		obj = anon_create();
		if (!obj)
			return -ENOSPC;

		vma->vma_obj = obj;
	}

	list_insert_tail(mmobj_bottom_vmas(obj), &vma->vma_olink);

	if (new)
		*new = vma;

	return 0;
}

/*
 * We have no guarantee that the region of the address space being
 * unmapped will play nicely with our list of vmareas.
 *
 * You must iterate over each vmarea that is partially or wholly covered
 * by the address range [addr ... addr+len). The vm-area will fall into one
 * of four cases, as illustrated below:
 *
 * key:
 *          [             ]   Existing VM Area
 *        *******             Region to be unmapped
 *
 * Case 1:  [   ******    ]
 * The region to be unmapped lies completely inside the vmarea. We need to
 * split the old vmarea into two vmareas. be sure to increment the
 * reference count to the file associated with the vmarea.
 *
 * Case 2:  [      *******]**
 * The region overlaps the end of the vmarea. Just shorten the length of
 * the mapping.
 *
 * Case 3: *[*****        ]
 * The region overlaps the beginning of the vmarea. Move the beginning of
 * the mapping (remember to update vma_off), and shorten its length.
 *
 * Case 4: *[*************]**
 * The region completely contains the vmarea. Remove the vmarea from the
 * list.
 */
int
vmmap_remove(vmmap_t *map, uint32_t lopage, uint32_t npages)
{
	dbg(DBG_VM, "\n");

	uint32_t endpage = lopage + npages;
	vmarea_t *vma;
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
		/* Case 1:  [   ******    ] */
		if (vma->vma_start < lopage && endpage < vma->vma_end) {
			dbg(DBG_BRK, "vmmap to %x\n", PN_TO_ADDR(endpage));
			vmarea_t *newvma = vmarea_alloc();
			newvma->vma_start = endpage;
			newvma->vma_end = vma->vma_end;
			newvma->vma_off = vma->vma_off + (endpage - vma->vma_start);
			newvma->vma_prot = vma->vma_prot;
			newvma->vma_flags = vma->vma_flags;
			newvma->vma_vmmap = vma->vma_vmmap;

			vma->vma_obj->mmo_ops->ref(vma->vma_obj);
			newvma->vma_obj = vma->vma_obj;

			list_insert_head(&vma->vma_plink, &newvma->vma_plink);
			list_insert_tail(mmobj_bottom_vmas(vma->vma_obj),
					 &newvma->vma_olink);

			vma->vma_end = lopage;
		}

		/* Case 2:  [      *******]** */
		else if (vma->vma_start < lopage && lopage < vma->vma_end) {
			vma->vma_end = lopage;
		}

		/* Case 3: *[*****        ] */
		else if (vma->vma_start < endpage && endpage < vma->vma_end) {
			vma->vma_off = vma->vma_off + (endpage - vma->vma_start);
			vma->vma_start = endpage;
		}

		/* Case 4: *[*************]** */
		else if (vma->vma_start >= lopage && endpage >= vma->vma_end) {
			list_remove(&vma->vma_plink);
			list_remove(&vma->vma_olink);
			mmobj_t *o = vma->vma_obj;

			o->mmo_ops->put(o);
			vmarea_free(vma);
		}
	} list_iterate_end();

	/* TODO: Should this be curproc? */
	pt_unmap_range(curproc->p_pagedir, (uintptr_t)PN_TO_ADDR(lopage),
		       (uintptr_t)PN_TO_ADDR(endpage));

	return 0;
}

/*
 * Returns 1 if the given address space has no mappings for the
 * given range, 0 otherwise.
 */
int
vmmap_is_range_empty(vmmap_t *map, uint32_t startvfn, uint32_t npages)
{
	dbg(DBG_VM, "\n");

	uint32_t endpage = startvfn + npages;
	vmarea_t *vma;
	list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
				/* Case 1:  [   ******    ] */
		if (vma->vma_start < startvfn && endpage < vma->vma_end) {
			return 0;
		}

		/* Case 2:  [      *******]** */
		else if (vma->vma_start < startvfn && startvfn < vma->vma_end) {
			return 0;
		}

		/* Case 3: *[*****        ] */
		else if (vma->vma_start < endpage && endpage < vma->vma_end) {
			return 0;
		}

		/* Case 4: *[*************]** */
		else if (vma->vma_start >= startvfn && endpage >= vma->vma_end) {
			return 0;
		}
	} list_iterate_end();

        return 1;
}

/* Read into 'buf' from the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'. To do so, you will want to find the vmareas
 * to read from, then find the pframes within those vmareas corresponding
 * to the virtual addresses you want to read, and then read from the
 * physical memory that pframe points to. You should not check permissions
 * of the areas. Assume (KASSERT) that all the areas you are accessing exist.
 * Returns 0 on success, -errno on error.
 */
int
vmmap_read(vmmap_t *map, const void *vaddr, void *buf, size_t count)
{
	dbg(DBG_VM, "\n");

	mmobj_t *obj;
	vmarea_t *vma;
	pframe_t *pf;
	uint32_t cur_vaddr = (uint32_t)vaddr;
	size_t cur_count = MIN(PAGE_SIZE - PAGE_OFFSET(vaddr), count);
	uint32_t offset;
	char *cur_buf;
	char *cur_pfbuf;
	int ret;

	KASSERT(!vmmap_is_range_empty(map, ADDR_TO_PN(vaddr),
				      (count / PAGE_SIZE) + 1));
	KASSERT((vma = vmmap_lookup(map, ADDR_TO_PN(vaddr))));
	KASSERT((vma->vma_prot & PROT_READ));

	while (cur_vaddr != ((uint32_t)vaddr + count)) {
		if (ADDR_TO_PN(cur_vaddr) >= vma->vma_end) {
			KASSERT(!vmmap_is_range_empty(map, ADDR_TO_PN(cur_vaddr),
						      (count / PAGE_SIZE) + 1));
			KASSERT((vma = vmmap_lookup(map, (uint32_t)cur_vaddr)));
			KASSERT((vma->vma_prot & PROT_READ));
		}

		obj = vma->vma_obj;
		offset = vma->vma_off +
			(ADDR_TO_PN(cur_vaddr) - vma->vma_start);

		ret = pframe_lookup(obj, offset, 0, &pf);

		if (ret < 0)
			return ret;

		KASSERT(pf);

		cur_buf = (char *)((uintptr_t)buf +
				   (cur_vaddr - (uint32_t)vaddr));
		cur_pfbuf = (char *)((uintptr_t)pf->pf_addr +
				     PAGE_OFFSET(cur_vaddr));

		memcpy(cur_buf, cur_pfbuf, cur_count / sizeof(char));

		cur_vaddr += cur_count;
		cur_count = MIN(PAGE_SIZE - PAGE_OFFSET(cur_vaddr),
				(count - cur_count));
	}

	return 0;
}

/* Write from 'buf' into the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'. To do this, you will need to find the correct
 * vmareas to write into, then find the correct pframes within those vmareas,
 * and finally write into the physical addresses that those pframes correspond
 * to. You should not check permissions of the areas you use. Assume (KASSERT)
 * that all the areas you are accessing exist. Remember to dirty pages!
 * Returns 0 on success, -errno on error.
 */
int
vmmap_write(vmmap_t *map, void *vaddr, const void *buf, size_t count)
{
	dbg(DBG_VM, "\n");

	mmobj_t *obj;
	vmarea_t *vma;
	pframe_t *pf;
	uint32_t cur_vaddr = (uint32_t)vaddr;
	size_t cur_count = MIN(PAGE_SIZE - PAGE_OFFSET(vaddr), count);
	uint32_t offset;
	char *cur_buf;
	char *cur_pfbuf;
	int ret;

	KASSERT(!vmmap_is_range_empty(map, ADDR_TO_PN(vaddr),
				      (count / PAGE_SIZE) + 1));
	KASSERT((vma = vmmap_lookup(map, ADDR_TO_PN(vaddr))));
	KASSERT((vma->vma_prot & PROT_READ));

	while (cur_vaddr != ((uint32_t)vaddr + count)) {
		if (ADDR_TO_PN(cur_vaddr) >= vma->vma_end) {
			KASSERT(!vmmap_is_range_empty(map, ADDR_TO_PN(cur_vaddr),
						      (count / PAGE_SIZE) + 1));
			KASSERT((vma = vmmap_lookup(map, (uint32_t)cur_vaddr)));
			KASSERT((vma->vma_prot & PROT_READ));
		}

		obj = vma->vma_obj;
		offset = vma->vma_off +
			(ADDR_TO_PN(cur_vaddr) - vma->vma_start);

		ret = pframe_lookup(obj, offset, 0, &pf);

		if (ret < 0)
			return ret;
		KASSERT(pf);

		cur_buf = (char *)((uintptr_t)buf +
				   (cur_vaddr - (uint32_t)vaddr));
		cur_pfbuf = (char *)((uintptr_t)pf->pf_addr +
				     PAGE_OFFSET(cur_vaddr));

		memcpy(cur_pfbuf, cur_buf, cur_count / sizeof(char));
		KASSERT(!obj->mmo_ops->dirtypage(obj, pf));

		cur_vaddr += cur_count;
		cur_count = MIN(PAGE_SIZE - PAGE_OFFSET(cur_vaddr),
				(count - cur_count));
	}

	return 0;
}

/* a debugging routine: dumps the mappings of the given address space. */
size_t
vmmap_mapping_info(const void *vmmap, char *buf, size_t osize)
{
	dbg(DBG_VM, "\n");

	KASSERT(0 < osize);
        KASSERT(NULL != buf);
        KASSERT(NULL != vmmap);

        vmmap_t *map = (vmmap_t *)vmmap;
        vmarea_t *vma;
        ssize_t size = (ssize_t)osize;

        int len = snprintf(buf, size, "%21s %5s %7s %8s %10s %12s\n",
                           "VADDR RANGE", "PROT", "FLAGS", "MMOBJ", "OFFSET",
                           "VFN RANGE");

        list_iterate_begin(&map->vmm_list, vma, vmarea_t, vma_plink) {
                size -= len;
                buf += len;
                if (0 >= size) {
                        goto end;
                }

                len = snprintf(buf, size,
                               "%#.8x-%#.8x  %c%c%c  %7s 0x%p %#.5x %#.5x-%#.5x\n",
                               vma->vma_start << PAGE_SHIFT,
                               vma->vma_end << PAGE_SHIFT,
                               (vma->vma_prot & PROT_READ ? 'r' : '-'),
                               (vma->vma_prot & PROT_WRITE ? 'w' : '-'),
                               (vma->vma_prot & PROT_EXEC ? 'x' : '-'),
                               (vma->vma_flags & MAP_SHARED ? " SHARED" : "PRIVATE"),
                               vma->vma_obj, vma->vma_off, vma->vma_start, vma->vma_end);
        } list_iterate_end();

end:
        if (size <= 0) {
                size = osize;
                buf[osize - 1] = '\0';
        }
        /*
        KASSERT(0 <= size);
        if (0 == size) {
                size++;
                buf--;
                buf[0] = '\0';
        }
        */
        return osize - size;
}
