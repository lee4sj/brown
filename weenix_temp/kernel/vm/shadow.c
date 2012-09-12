#include "globals.h"
#include "errno.h"

#include "util/string.h"
#include "util/debug.h"

#include "mm/mmobj.h"
#include "mm/pframe.h"
#include "mm/mm.h"
#include "mm/page.h"
#include "mm/slab.h"
#include "mm/tlb.h"

#include "vm/vmmap.h"
#include "vm/shadow.h"
#include "vm/shadowd.h"

#define SHADOW_SINGLETON_THRESHOLD 5

int shadow_count = 0; /* for debugging/verification purposes */
#ifdef __SHADOWD__
/*
 * number of shadow objects with a single parent, that is another shadow
 * object in the shadow objects tree(singletons)
 */
static int shadow_singleton_count = 0;
#endif

static slab_allocator_t *shadow_allocator;

static void shadow_ref(mmobj_t *o);
static void shadow_put(mmobj_t *o);
static int  shadow_lookuppage(mmobj_t *o, uint32_t pagenum, int forwrite, pframe_t **pf);
static int  shadow_fillpage(mmobj_t *o, pframe_t *pf);
static int  shadow_dirtypage(mmobj_t *o, pframe_t *pf);
static int  shadow_cleanpage(mmobj_t *o, pframe_t *pf);

static mmobj_ops_t shadow_mmobj_ops = {
        .ref = shadow_ref,
        .put = shadow_put,
        .lookuppage = shadow_lookuppage,
        .fillpage  = shadow_fillpage,
        .dirtypage = shadow_dirtypage,
        .cleanpage = shadow_cleanpage
};

/*
 * This function is called at boot time to initialize the
 * shadow page sub system. Currently it only initializes the
 * shadow_allocator object.
 */
void
shadow_init()
{
	shadow_allocator =
		slab_allocator_create("shadow mmobj", sizeof(mmobj_t));
	KASSERT(NULL != shadow_allocator &&
		"failed to create shodow allocator!");
}

/*
 * You'll want to use the shadow_allocator to allocate the mmobj to
 * return, then then initialize it. Take a look in mm/mmobj.h for
 * macros which can be of use here. Make sure your initial
 * reference count is correct.
 */
mmobj_t *
shadow_create()
{
	dbg(DBG_VM, "\n");

	mmobj_t *shadow = slab_obj_alloc(shadow_allocator);
	if (!shadow)
		return NULL;

	dbg(DBG_FORK, "shadow mmobj %p\n", shadow);
	mmobj_init(shadow, &shadow_mmobj_ops);
	shadow->mmo_refcount = 1;

	shadow_count++;

	return shadow;
}

/* Implementation of mmobj entry points: */

/*
 * Increment the reference count on the object.
 */
static void
shadow_ref(mmobj_t *o)
{
	dbg(DBG_VM, "\n");

	dbg(DBG_FORK, "mmobj %p from %d to %d: nres = %d\n", o, o->mmo_refcount,
	    o->mmo_refcount + 1, o->mmo_nrespages);

	KASSERT(o->mmo_refcount != 0);
	KASSERT(o->mmo_nrespages >= 0 && o->mmo_refcount >= o->mmo_nrespages);
	o->mmo_refcount++;
}

void
shadow_destroy(mmobj_t *o) {
	shadow_count--;
	return slab_obj_free(shadow_allocator, o);
}

/*
 * Decrement the reference count on the object. If, however, the
 * reference count on the object reaches the number of resident
 * pages of the object, we can conclude that the object is no
 * longer in use and, since it is a shadow object, it will never
 * be used again. You should unpin and uncache all of the object's
 * pages and then free the object itself.
 */
static void
shadow_put(mmobj_t *o)
{
	dbg(DBG_VM, "\n");

	KASSERT(mmobj_bottom_obj(o) != o);

	dbg(DBG_FORK, "mmobj %p from %d to %d: nres = %d\n", o, o->mmo_refcount,
	    o->mmo_refcount - 1, o->mmo_nrespages);
	KASSERT(o->mmo_nrespages >= 0 && o->mmo_refcount > o->mmo_nrespages);

	if ((--o->mmo_refcount) != o->mmo_nrespages) {
		return;
	}

	/* At this point, mmo_refcount == mmo_nrespages */
	pframe_t *pf;
	list_iterate_begin(&o->mmo_respages, pf, pframe_t, pf_olink) {
		pframe_unpin(pf);
		KASSERT(!pframe_is_pinned(pf) && !pframe_is_free(pf));

		--o->mmo_refcount;
		pframe_free_no_put(pf);
	} list_iterate_end();

	o->mmo_shadowed->mmo_ops->put(o->mmo_shadowed);
	KASSERT(o->mmo_nrespages == 0 && o->mmo_nrespages == o->mmo_refcount);

	return shadow_destroy(o);
}

/* This function looks up the given page in this shadow object. The
 * forwrite argument is true if the page is being looked up for
 * writing, false if it is being looked up for reading. This function
 * must handle all do-not-copy-on-not-write magic (i.e. when forwrite
 * is false find the first shadow object in the chain which has the
 * given page resident). copy-on-write magic (necessary when forwrite
 * is true) is handled in shadow_fillpage, not here. */
static int
shadow_lookuppage(mmobj_t *o, uint32_t pagenum, int forwrite, pframe_t **pf)
{
	dbg(DBG_VM, "\n");

	pframe_t *shadowed_pf;

	KASSERT(NULL != o->mmo_shadowed);

	if (forwrite)
		return pframe_get(o, pagenum, pf);

	/* do-not-copy-on-not-write magic; here, forwrite == 0 */
	mmobj_t *cur_o = o;

	while (cur_o->mmo_shadowed) {
		*pf = pframe_get_resident(cur_o, pagenum);
		if (*pf)
			return 0;

		cur_o = cur_o->mmo_shadowed;
	}

	/* At this point, we could not find the page in the shadow objects
	 * So, the page should be fetched from the bottom_most mmobj */
	cur_o = mmobj_bottom_obj(cur_o);
	int ret = pframe_lookup(cur_o, pagenum, forwrite, pf);
	KASSERT(ret == 0 &&
		"shadow_lookuppage is called for non private mapped address\n");

	return 0;
}

/* As per the specification in mmobj.h, fill the page frame starting
 * at address pf->pf_addr with the contents of the page identified by
 * pf->pf_obj and pf->pf_pagenum. This function handles all
 * copy-on-write magic (i.e. if there is a shadow object which has
 * data for the pf->pf_pagenum-th page then we should take that data,
 * if no such shadow object exists we need to follow the chain of
 * shadow objects all the way to the bottom object and take the data
 * for the pf->pf_pagenum-th page from the last object in the chain). */
/*
 * My note:
 * Also this assumes that the page is not resident on the memory.
 * i.e) the page we want is not on this shadow obj.
 */
static int
shadow_fillpage(mmobj_t *o, pframe_t *pf)
{
	dbg(DBG_VM, "\n");

	KASSERT(NULL != o->mmo_shadowed);
	mmobj_t *cur_o = o->mmo_shadowed;

	/* copy-on-write magic */
	pframe_t *cur_pf;
	while (cur_o->mmo_shadowed) {
		cur_pf = pframe_get_resident(cur_o, pf->pf_pagenum);
		if (cur_pf) {
			pframe_pin(pf);
			memcpy(pf->pf_addr, cur_pf->pf_addr, PAGE_SIZE);
			return 0;
		}

		cur_o = cur_o->mmo_shadowed;
	}

	/* At this point, we could not find the page in the shadow objects
	 * So, the page should be fetched from the bottom_most mmobj */
	cur_o = mmobj_bottom_obj(cur_o);
	int ret = pframe_lookup(cur_o, pf->pf_pagenum, 0, &cur_pf);

	if (ret < 0) {
		pframe_pin(pf);
		return ret;
	}

	memcpy(pf->pf_addr, cur_pf->pf_addr, PAGE_SIZE);

	pframe_pin(pf);
	return 0;
}

/* These next two functions are not difficult. */

static int
shadow_dirtypage(mmobj_t *o, pframe_t *pf)
{
	dbg(DBG_VM, "\n");
	return 0;
}

static int
shadow_cleanpage(mmobj_t *o, pframe_t *pf)
{
	dbg(DBG_VM, "\n");
	return 0;
}
