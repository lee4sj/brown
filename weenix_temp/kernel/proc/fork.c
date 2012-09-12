#include "types.h"
#include "globals.h"
#include "errno.h"

#include "util/debug.h"
#include "util/string.h"

#include "proc/proc.h"
#include "proc/kthread.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/page.h"
#include "mm/pframe.h"
#include "mm/mmobj.h"
#include "mm/pagetable.h"
#include "mm/tlb.h"

#include "fs/file.h"
#include "fs/vnode.h"

#include "vm/shadow.h"
#include "vm/vmmap.h"

#include "api/exec.h"

#include "main/interrupt.h"

/* Pushes the appropriate things onto the kernel stack of a newly forked thread
 * so that it can begin execution in userland_entry.
 * regs: registers the new thread should have on execution
 * kstack: location of the new thread's kernel stack
 * Returns the new stack pointer on success. */
static uint32_t
fork_setup_stack(const regs_t *regs, void *kstack)
{
        /* Pointer argument and dummy return address, and userland dummy return
         * address */
        uint32_t esp = ((uint32_t) kstack) + DEFAULT_STACK_SIZE - (sizeof(regs_t) + 12);
        *(void **)(esp + 4) = (void *)(esp + 8); /* Set the argument to point to location of struct on stack */
        memcpy((void *)(esp + 8), regs, sizeof(regs_t)); /* Copy over struct */
        return esp;
}

/**
 * do_fork helper function.
 * Allocates two shadow objects per a privately mapped vmarea, and put them
 * in a list (shadow_list). The link on the mmobjs are the mmo_un.mmo_vmas
 *
 * @return: 1 on success, 0 on failure.
 */
int alloc_shadow_objs(list_t *shadow_list) {
	vmarea_t *vma;
	mmobj_t *o;

	list_iterate_begin(&curproc->p_vmmap->vmm_list, vma, vmarea_t,
			   vma_plink) {
		if (vma->vma_flags & MAP_PRIVATE) {
			if ((o = shadow_create()) == NULL)
				goto err;
			list_insert_tail(shadow_list, &o->mmo_un.mmo_vmas);

			if ((o = shadow_create()) == NULL)
				goto err;
			list_insert_tail(shadow_list, &o->mmo_un.mmo_vmas);
		}
	} list_iterate_end();

	return 1;
err:
	/* go through the current shadow_list and delet all. */
	list_iterate_begin(shadow_list, o, mmobj_t, mmo_un.mmo_vmas) {
		list_remove(&o->mmo_un.mmo_vmas);
		shadow_destroy(o);
	} list_iterate_end();

	return 0;
}

/*
 * The implementation of fork(2). Once this works,
 * you're practically home free. This is what the
 * entirety of Weenix has been leading up to.
 * Go forth and conquer.
 */
int
do_fork(struct regs *regs)
{
	dbg(DBG_VM, "\n");
	proc_t *nproc = NULL;
	kthread_t *nkt = NULL;
	vmmap_t * nmap = NULL;
	list_t shadow_list;
	list_init(&shadow_list);

	/* proc creation */
	nproc = proc_create("user proc");
	if (!nproc)
		goto err;

	/* thread creation */
	nkt = kthread_clone(curthr);
	if (!nkt)
		goto err;

	list_insert_tail(&nproc->p_threads, &nkt->kt_plink);
	nkt->kt_proc = nproc;

	/* vmmap creation */
	nmap = vmmap_clone(curproc->p_vmmap);
	if (!nmap)
		goto err;

	/* Since failures can only happen in allocations,
	   first see if we can allocate enough mmobjs for the
	   MAP_PRIVATE region, then put those in a shadow_list,
	   linked on mmo_un.mmo_vmas */
	if (!alloc_shadow_objs(&shadow_list))
		goto err;

	/* now, set up vmareas */
	vmarea_t *nvma;
	vmarea_t *ovma;
	list_link_t *nlink;
	list_link_t *olink;
	nlink = nmap->vmm_list.l_next;
	olink = curproc->p_vmmap->vmm_list.l_next;
	while (nlink != &(nmap->vmm_list)) {
		KASSERT(olink != &(curproc->p_vmmap->vmm_list));

		/* get vmas */
		nvma = list_item(nlink, vmarea_t, vma_plink);
		ovma = list_item(olink, vmarea_t, vma_plink);

		if (ovma->vma_flags & MAP_PRIVATE) {
			mmobj_t *shadow_new;
			mmobj_t *shadow_old;
			mmobj_t *shadowed;

			shadowed = ovma->vma_obj;
			KASSERT(shadowed);

			/* get the shadow objs from the shadow_list */
			KASSERT(!list_empty(&shadow_list));
			shadow_new = list_head(&shadow_list, mmobj_t,
					       mmo_un.mmo_vmas);
			list_remove(&shadow_new->mmo_un.mmo_vmas);

			KASSERT(!list_empty(&shadow_list));
			shadow_old = list_head(&shadow_list, mmobj_t,
					       mmo_un.mmo_vmas);
			list_remove(&shadow_old->mmo_un.mmo_vmas);

			KASSERT(shadow_old != shadow_new);

			/* initialize the shadow_objs */
			shadow_new->mmo_shadowed = shadowed;
			shadow_old->mmo_shadowed = shadowed;

			mmobj_t *bottom = mmobj_bottom_obj(shadowed);
			shadow_new->mmo_un.mmo_bottom_obj = bottom;
			shadow_old->mmo_un.mmo_bottom_obj = bottom;

			nvma->vma_obj = shadow_new;
			ovma->vma_obj = shadow_old;

			list_insert_tail(mmobj_bottom_vmas(shadow_new),
					 &(nvma->vma_olink));

			shadowed->mmo_ops->ref(shadowed);
		} else {
			KASSERT(mmobj_bottom_obj(ovma->vma_obj) == ovma->vma_obj);
			ovma->vma_obj->mmo_ops->ref(ovma->vma_obj);
			nvma->vma_obj = ovma->vma_obj;
		}

		/* looping */
		nlink = nlink->l_next;
		olink = olink->l_next;
	}

	KASSERT(list_empty(&shadow_list));

	/* set proc field */
	nmap->vmm_proc = nproc;
	vmmap_destroy(nproc->p_vmmap);
	nproc->p_vmmap = nmap;
	nproc->p_brk = curproc->p_brk;
	nproc->p_start_brk = curproc->p_brk;

	/* Copy the file table */
	int i;
	file_t *f;
	for (i = 0; i < NFILES; i++) {
		f = fget(i);
		nproc->p_files[i] = f;
	}

	/* set up the new process thread context */
	context_t *c = &nkt->kt_ctx;
	c->c_pdptr = nproc->p_pagedir;

	c->c_eip = (uintptr_t) &userland_entry;
	c->c_kstack = (uintptr_t)nkt->kt_kstack;
	c->c_kstacksz = DEFAULT_STACK_SIZE;

	regs_t nregs;
	memcpy(&nregs, regs, sizeof(regs_t));
	nregs.r_eax = 0;
	c->c_esp = fork_setup_stack(&nregs, nkt->kt_kstack);
	c->c_ebp = c->c_esp;

	/* unmap the userland pagetable and flush the tlb */
	pt_unmap_range(curproc->p_pagedir, USER_MEM_LOW, USER_MEM_HIGH);
	tlb_flush_all();

	/* make the new thread runnable */
	sched_make_runnable(nkt);

	return nproc->p_pid;

err:
	if (nproc != NULL)
		proc_destroy(nproc);
	if (nkt != NULL)
		kthread_destroy(nkt);
	if (nmap != NULL)
		vmmap_destroy(nmap);

	return -ENOMEM;
}
