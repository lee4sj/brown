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

int anon_count = 0; /* for debugging/verification purposes */

static slab_allocator_t *anon_allocator;

static void anon_ref(mmobj_t *o);
static void anon_put(mmobj_t *o);
static int  anon_lookuppage(mmobj_t *o, uint32_t pagenum, int forwrite, pframe_t **pf);
static int  anon_fillpage(mmobj_t *o, pframe_t *pf);
static int  anon_dirtypage(mmobj_t *o, pframe_t *pf);
static int  anon_cleanpage(mmobj_t *o, pframe_t *pf);

static mmobj_ops_t anon_mmobj_ops = {
        .ref = anon_ref,
        .put = anon_put,
        .lookuppage = anon_lookuppage,
        .fillpage  = anon_fillpage,
        .dirtypage = anon_dirtypage,
        .cleanpage = anon_cleanpage
};

/*
 * This function is called at boot time to initialize the
 * anonymous page sub system. Currently it only initializes the
 * anon_allocator object.
 */
void
anon_init()
{
	anon_allocator = slab_allocator_create("anon mmobj", sizeof(mmobj_t));
	KASSERT(NULL != anon_allocator && "failed to create anon allocator!");
}

/*
 * You'll want to use the anon_allocator to allocate the mmobj to
 * return, then then initialize it. Take a look in mm/mmobj.h for
 * macros which can be of use here. Make sure your initial
 * reference count is correct.
 */
mmobj_t *
anon_create()
{
	dbg(DBG_VM, "\n");

	mmobj_t *anon = slab_obj_alloc(anon_allocator);
	if (!anon)
		return NULL;

	mmobj_init(anon, &anon_mmobj_ops);
	anon_ref(anon);

	dbg(DBG_ANON, "anon_count from %d to %d\n", anon_count, anon_count + 1);
	anon_count++;

	return anon;
}

/* Implementation of mmobj entry points: */

/*
 * Increment the reference count on the object.
 */
static void
anon_ref(mmobj_t *o)
{
	dbg(DBG_VM, "\n");

	KASSERT(o->mmo_nrespages >= 0 && o->mmo_refcount >= o->mmo_nrespages);
	o->mmo_refcount++;
}

/*
 * Decrement the reference count on the object. If, however, the
 * reference count on the object reaches the number of resident
 * pages of the object, we can conclude that the object is no
 * longer in use and, since it is an anonymous object, it will
 * never be used again. You should unpin and uncache all of the
 * object's pages and then free the object itself.
 */
static void
anon_put(mmobj_t *o)
{
	dbg(DBG_VM, "\n");

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

	KASSERT(o->mmo_nrespages == 0 && o->mmo_nrespages == o->mmo_refcount);

	dbg(DBG_ANON, "anon_count from %d to %d\n", anon_count, anon_count - 1);
	anon_count--;

	return slab_obj_free(anon_allocator, o);
}

/* Get the corresponding page from the mmobj. No special handling is
 * required. */
static int
anon_lookuppage(mmobj_t *o, uint32_t pagenum, int forwrite, pframe_t **pf)
{
	dbg(DBG_VM, "\n");

	return pframe_get(o, pagenum, pf);
}

/* The following three functions should not be difficult. */

static int
anon_fillpage(mmobj_t *o, pframe_t *pf)
{
	dbg(DBG_VM, "\n");

	memset(pf->pf_addr, 0, PAGE_SIZE);
	pframe_pin(pf);
	return 0;
}

static int
anon_dirtypage(mmobj_t *o, pframe_t *pf)
{
	dbg(DBG_VM, "\n");
        return 0;
}

static int
anon_cleanpage(mmobj_t *o, pframe_t *pf)
{
	dbg(DBG_VM, "\n");
	return 0;
}
