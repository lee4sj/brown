/*
 *   FILE: s5fs_subr.c
 * AUTHOR: afenn
 *  DESCR:
 *  $Id: s5fs_subr.c,v 1.1.2.1 2006/06/04 01:02:15 afenn Exp $
 */

#include "kernel.h"
#include "util/debug.h"
#include "mm/kmalloc.h"
#include "globals.h"
#include "proc/sched.h"
#include "proc/kmutex.h"
#include "errno.h"
#include "util/string.h"
#include "util/printf.h"
#include "mm/pframe.h"
#include "mm/mmobj.h"
#include "drivers/dev.h"
#include "drivers/blockdev.h"
#include "fs/stat.h"
#include "fs/vfs.h"
#include "fs/vnode.h"
#include "fs/s5fs/s5fs_subr.h"
#include "fs/s5fs/s5fs.h"
#include "mm/mm.h"
#include "mm/page.h"

#define dprintf(...) dbg(DBG_S5FS, __VA_ARGS__)

#define s5_dirty_super(fs)                                           \
        do {                                                         \
                pframe_t *p;                                         \
                int err;                                             \
                pframe_get(S5FS_TO_VMOBJ(fs), S5_SUPER_BLOCK, &p);   \
                KASSERT(p);                                          \
                err = pframe_dirty(p);                               \
                KASSERT(!err                                         \
                        && "shouldn\'t fail for a page belonging "   \
                        "to a block device");                        \
        } while (0)


static void s5_free_block(s5fs_t *fs, int block);
static int s5_alloc_block(s5fs_t *);


/*
 * Return the disk-block number for the given seek pointer (aka file
 * position).
 *
 * If the seek pointer refers to a sparse block, and alloc is false,
 * then return 0. If the seek pointer refers to a sparse block, and
 * alloc is true, then allocate a new disk block (and make the inode
 * point to it) and return it.
 *
 * Be sure to handle indirect blocks!
 *
 * If there is an error, return -errno.
 *
 * You probably want to use pframe_get, pframe_pin, pframe_unpin, pframe_dirty.
 */
int
s5_seek_to_block(vnode_t *vnode, off_t seekptr, int alloc)
{
	dbg(DBG_S5FS, "\n");

	s5fs_t *s5fs;
	off_t blknum;
	off_t blk_index;
	s5_inode_t *i;
	pframe_t *pf;
	uint32_t *blocks;

	KASSERT(seekptr < vnode->vn_len);

	s5fs = VNODE_TO_S5FS(vnode);
	blknum = S5_DATA_BLOCK(seekptr);
	i = VNODE_TO_S5INODE(vnode);

	KASSERT(i);

	/* if seekptr points to a direct block */
	if (blknum < S5_NDIRECT_BLOCKS) {
		blk_index = i->s5_direct_blocks[blknum];

		/* if a sparse block, and needs alloc */
		if (blk_index == 0 && alloc) {
			if ((blk_index = s5_alloc_block(s5fs)) < 0)
				return blk_index;
			
			i->s5_direct_blocks[blknum] = blk_index;
			s5_dirty_inode(VNODE_TO_S5FS(vnode), i);
		}
	}

	/* if seekptr points to a indirect block */
	else if (blknum >= S5_NDIRECT_BLOCKS) {
		KASSERT(blknum < (S5_NDIRECT_BLOCKS +
				  (S5_BLOCK_SIZE / (ssize_t)sizeof(uint32_t))));

		int ret = pframe_get(S5FS_TO_VMOBJ(VNODE_TO_S5FS(vnode)),
				     i->s5_indirect_block, &pf);
		pframe_pin(pf);

		blocks = (uint32_t *)pf->pf_addr;
		if ((uint32_t)blknum > S5_MAX_FILE_BLOCKS)
			return -EFBIG;
		blk_index = blocks[blknum - S5_NDIRECT_BLOCKS];

		/* if a sparse block, and needs alloc */
		if (blk_index == 0 && alloc) {
			if ((blk_index = s5_alloc_block(s5fs)) < 0) {
				pframe_unpin(pf);
				return blk_index;
			}

			blocks[blknum] = blk_index;
			pframe_dirty(pf);
		}

		pframe_unpin(pf);
	}	

	return blk_index;
}


/*
 * Locks the mutex for the whole file system
 */
static void
lock_s5(s5fs_t *fs)
{
        kmutex_lock(&fs->s5f_mutex);
}

/*
 * Unlocks the mutex for the whole file system
 */
static void
unlock_s5(s5fs_t *fs)
{
        kmutex_unlock(&fs->s5f_mutex);
}


/*
 * Write len bytes to the given inode, starting at seek bytes from the
 * beginning of the inode. On success, return the number of bytes
 * actually written (which should always be 'len'); on failure, return
 * -errno.
 *
 * This function should allow writing to files or directories, treating
 * them identically.
 *
 * Writing to a sparse block of the file should cause that block to be
 * allocated. Seek to block is the wrong answer, you'll make rm despair!
 *
 * Writing past the end of the file should increase the size of the
 * file. Blocks between the end and where you start writing will be
 * sparse.
 *
 * You will need pframe_dirty, pframe_get
 *
 */
int
s5_write_file(vnode_t *vnode, off_t seek, const char *bytes, size_t len)
{
	dbg(DBG_S5FS, "\n");

	pframe_t *pf;
	s5_inode_t *i;
	int ret;

	off_t cur_seek;
	ssize_t cur_len;

	cur_seek = seek;
	cur_len = MIN((ssize_t)len, S5_BLOCK_SIZE - S5_DATA_OFFSET(cur_seek));

	while (cur_seek != (seek + (off_t)len)) {
		
		/* update vn_len and inode->s5_size */
		i = VNODE_TO_S5INODE(vnode);
		KASSERT(vnode->vn_len == (off_t)i->s5_size);
		if ((cur_seek + cur_len) > vnode->vn_len) {
			vnode->vn_len = cur_seek + cur_len;
			i->s5_size = vnode->vn_len;
			s5_dirty_inode(VNODE_TO_S5FS(vnode), i);
		}

		ret = pframe_get(&vnode->vn_mmobj, S5_DATA_BLOCK(cur_seek), &pf);
		if (ret < 0)
			return ret;
		
		pframe_pin(pf);
		
		/* copy the bytes to the file */
		memcpy(&((char *)pf->pf_addr)[S5_DATA_OFFSET(cur_seek)],
		       &bytes[cur_seek - seek], cur_len);
		pframe_dirty(pf);
		pframe_unpin(pf);

		cur_seek += cur_len;
		cur_len = MIN((ssize_t)len - (cur_seek - seek),
			      S5_BLOCK_SIZE - S5_DATA_OFFSET(cur_seek));
	}

	return len;
}

/*
 * Read up to len bytes from the given inode, starting at seek bytes
 * from the beginning of the inode. On success, return the number of
 * bytes actually read, or 0 if the end of the file has been reached; on
 * failure, return -errno.
 *
 * This function should allow reading from files or directories,
 * treating them identically.
 *
 * Reading from a sparse block of the file should act like reading
 * zeros; it should not cause the sparse blocks to be allocated.
 *
 * If the region to be read would extend past the end of the file, less
 * data will be read than was requested.
 *
 * You probably want to use pframe_get(), memcpy().
 */
int
s5_read_file(struct vnode *vnode, off_t seek, char *dest, size_t len)
{
	dbg(DBG_S5FS, "\n");

	off_t blk_index;
	pframe_t *pf;
	int ret;

	off_t cur_seek;
	ssize_t cur_len;

	cur_seek = seek;
	cur_len = MIN((ssize_t)len, S5_BLOCK_SIZE - S5_DATA_OFFSET(cur_seek));
	cur_len = MIN(cur_len, vnode->vn_len - cur_seek);

	while (cur_seek != (seek + (off_t)len)) {
		len = MIN(cur_len, vnode->vn_len - cur_seek);

		/* check if the end of the file has been reached */
		if (cur_seek >= vnode->vn_len)
			break;

		/* at this point, the seek is not pointing to a sparse block */
		ret = pframe_get(&vnode->vn_mmobj, S5_DATA_BLOCK(cur_seek), &pf);
		if (ret < 0)
			return ret;

		memcpy(&dest[cur_seek - seek],
		       &((char *)pf->pf_addr)[S5_DATA_OFFSET(cur_seek)],
		       (size_t)cur_len);

		cur_seek += cur_len;
		cur_len = MIN((ssize_t)len - (cur_seek - seek),
			      S5_BLOCK_SIZE - S5_DATA_OFFSET(cur_seek));
		cur_len = MIN(cur_len, vnode->vn_len - cur_seek);
	}

	return (cur_seek - seek);
}

/*
 * Allocate a new disk-block off the block free list and return it. If
 * there are no free blocks, return -ENOSPC.
 *
 * This will not initialize the contents of an allocated block; these
 * contents are undefined.
 *
 * If the super block's s5s_nfree is 0, you need to refill 
 * s5s_free_blocks and reset s5s_nfree.  You need to read the contents 
 * of this page using the pframe system in order to obtain the next set of
 * free block numbers.
 *
 * Don't forget to dirty the appropriate blocks!
 *
 * You'll probably want to use lock_s5(), unlock_s5(), pframe_get(),
 * and s5_dirty_super()
 */
static int
s5_alloc_block(s5fs_t *fs)
{
	dbg(DBG_S5FS, "\n");

	int blk_index;
	s5_super_t *s;
	pframe_t *nf_blocks;
	int ret;

	s = fs->s5f_super;

	lock_s5(fs);

	KASSERT(S5_NBLKS_PER_FNODE > s->s5s_nfree);

	/* if end of loaded free blocks, load more free blocks */
	if (s->s5s_nfree == 0) {
		/* get the pframe of the block that has the next free blocks */
		blk_index = s->s5s_free_blocks[S5_NBLKS_PER_FNODE - 1];

		/* if no more free blocks, return -ENOSPC */
		if (blk_index == -1)
			return -ENOSPC;

		ret = pframe_get(S5FS_TO_VMOBJ(fs), blk_index, &nf_blocks);
		if (ret < 0)
			return ret;
		KASSERT(nf_blocks->pf_addr);

		/* copy them to the superblock */
		memcpy(s->s5s_free_blocks, nf_blocks->pf_addr,
		       S5_NBLKS_PER_FNODE * sizeof(uint32_t));

		/* reset s->s5s_nfree */
		s->s5s_nfree = S5_NBLKS_PER_FNODE;
	}

	/* Now, allocate a free block */
	blk_index = s->s5s_free_blocks[S5_NBLKS_PER_FNODE - (s->s5s_nfree--)];

	s5_dirty_super(fs);

	unlock_s5(fs);
	return blk_index;
}


/*
 * Given a filesystem and a block number, frees the given block in the
 * filesystem.
 *
 * This function may potentially block.
 *
 * The caller is responsible for ensuring that the block being placed on
 * the free list is actually free and is not resident.
 */
static void
s5_free_block(s5fs_t *fs, int blockno)
{
        s5_super_t *s = fs->s5f_super;


        lock_s5(fs);

        KASSERT(S5_NBLKS_PER_FNODE > s->s5s_nfree);

        if ((S5_NBLKS_PER_FNODE - 1) == s->s5s_nfree) {
                /* get the pframe where we will store the free block nums */
                pframe_t *prev_free_blocks = NULL;
                KASSERT(fs->s5f_bdev);
                pframe_get(&fs->s5f_bdev->bd_mmobj, blockno, &prev_free_blocks);
                KASSERT(prev_free_blocks->pf_addr);

                /* copy from the superblock to the new block on disk */
                memcpy(prev_free_blocks->pf_addr, (void *)(s->s5s_free_blocks),
                       S5_NBLKS_PER_FNODE * sizeof(int));
                pframe_dirty(prev_free_blocks);

                /* reset s->s5s_nfree and s->s5s_free_blocks */
                s->s5s_nfree = 0;
                s->s5s_free_blocks[S5_NBLKS_PER_FNODE - 1] = blockno;
        } else {
                s->s5s_free_blocks[s->s5s_nfree++] = blockno;
        }

        s5_dirty_super(fs);

        unlock_s5(fs);
}

/*
 * Creates a new inode from the free list and initializes its fields.
 * Uses S5_INODE_BLOCK to get the page from which to create the inode
 *
 * This function may block.
 */
int
s5_alloc_inode(fs_t *fs, uint16_t type, devid_t devid)
{
        s5fs_t *s5fs = FS_TO_S5FS(fs);
        pframe_t *inodep;
        s5_inode_t *inode;
        int ret = -1;

        KASSERT((S5_TYPE_DATA == type)
                || (S5_TYPE_DIR == type)
                || (S5_TYPE_CHR == type)
                || (S5_TYPE_BLK == type));


        lock_s5(s5fs);

        if (s5fs->s5f_super->s5s_free_inode == (uint32_t) -1) {
                unlock_s5(s5fs);
                return -ENOSPC;
        }

        pframe_get(&s5fs->s5f_bdev->bd_mmobj,
                   S5_INODE_BLOCK(s5fs->s5f_super->s5s_free_inode),
                   &inodep);
        KASSERT(inodep);

        inode = (s5_inode_t *)(inodep->pf_addr)
                + S5_INODE_OFFSET(s5fs->s5f_super->s5s_free_inode);

        KASSERT(inode->s5_number == s5fs->s5f_super->s5s_free_inode);

        ret = inode->s5_number;

        /* reset s5s_free_inode; remove the inode from the inode free list: */
        s5fs->s5f_super->s5s_free_inode = inode->s5_next_free;
        pframe_pin(inodep);
        s5_dirty_super(s5fs);
        pframe_unpin(inodep);


        /* init the newly-allocated inode: */
        inode->s5_size = 0;
        inode->s5_type = type;
        inode->s5_linkcount = 0;
        memset(inode->s5_direct_blocks, 0, S5_NDIRECT_BLOCKS * sizeof(int));
        if ((S5_TYPE_CHR == type) || (S5_TYPE_BLK == type))
                inode->s5_indirect_block = devid;
        else
                inode->s5_indirect_block = 0;

        s5_dirty_inode(s5fs, inode);

        unlock_s5(s5fs);

        return ret;
}


/*
 * Free an inode by freeing its disk blocks and putting it back on the
 * inode free list.
 *
 * You should also reset the inode to an unused state (eg. zero-ing its
 * list of blocks and setting its type to S5_FREE_TYPE).
 *
 * Don't forget to free the indirect block if it exists.
 *
 * You probably want to use s5_free_block().
 */
void
s5_free_inode(vnode_t *vnode)
{
        uint32_t i;
        s5_inode_t *inode = VNODE_TO_S5INODE(vnode);
        s5fs_t *fs = VNODE_TO_S5FS(vnode);

        KASSERT((S5_TYPE_DATA == inode->s5_type)
                || (S5_TYPE_DIR == inode->s5_type)
                || (S5_TYPE_CHR == inode->s5_type)
                || (S5_TYPE_BLK == inode->s5_type));

        /* free any direct blocks */
        for (i = 0; i < S5_NDIRECT_BLOCKS; ++i) {
                if (inode->s5_direct_blocks[i]) {
                        dprintf("freeing block %d\n", inode->s5_direct_blocks[i]);
                        s5_free_block(fs, inode->s5_direct_blocks[i]);

                        s5_dirty_inode(fs, inode);
                        inode->s5_direct_blocks[i] = 0;
                }
        }

        if (((S5_TYPE_DATA == inode->s5_type)
             || (S5_TYPE_DIR == inode->s5_type))
            && inode->s5_indirect_block) {
                pframe_t *ibp;
                uint32_t *b;

                pframe_get(S5FS_TO_VMOBJ(fs),
                           (unsigned)inode->s5_indirect_block,
                           &ibp);
                KASSERT(ibp
                        && "because never fails for block_device "
                        "vm_objects");
                pframe_pin(ibp);

                b = (uint32_t *)(ibp->pf_addr);
                for (i = 0; i < S5_NIDIRECT_BLOCKS; ++i) {
                        KASSERT(b[i] != inode->s5_indirect_block);
                        if (b[i])
                                s5_free_block(fs, b[i]);
                }

                pframe_unpin(ibp);

                s5_free_block(fs, inode->s5_indirect_block);
        }

        inode->s5_indirect_block = 0;
        inode->s5_type = S5_TYPE_FREE;
        s5_dirty_inode(fs, inode);

        lock_s5(fs);
        inode->s5_next_free = fs->s5f_super->s5s_free_inode;
        fs->s5f_super->s5s_free_inode = inode->s5_number;
        unlock_s5(fs);

        s5_dirty_inode(fs, inode);
        s5_dirty_super(fs);
}

/*
 * Locate the directory entry in the given inode with the given name,
 * and return its inode number. If there is no entry with the given
 * name, return -ENOENT.
 *
 * You'll probably want to use s5_read_file and name_match
 *
 * You can either read one dirent at a time or optimize and read more.
 * Either is fine.
 */
int
s5_find_dirent(vnode_t *vnode, const char *name, size_t namelen)
{
	dbg(DBG_S5FS, "\n");

	s5_dirent_t d;
	int i;
	int ret;

	for (i = 0; i < (vnode->vn_len / (ssize_t)sizeof(s5_dirent_t)); i++) {
		ret = s5_read_file(vnode, i * sizeof(s5_dirent_t), (char *)&d,
				   sizeof(s5_dirent_t));
		KASSERT((ret != 0) && (ret % sizeof(s5_dirent_t) == 0));

		/* if the end of directory entries is reached */
		if (name_match(d.s5d_name, "", 0))
			break;

		/* if name is found */
		if (name_match(d.s5d_name, name, namelen)) {
			return d.s5d_inode;
		}
	}

	return -ENOENT;
}

/*
 * Locate the directory entry in the given inode with the given name,
 * and delete it. If there is no entry with the given name, return
 * -ENOENT.
 *
 * In order to ensure that the directory entries are contiguous in the
 * directory file, you will need to move the last directory entry into
 * the remove dirent's place.
 *
 * When this function returns, the inode refcount on the removed file
 * should be decremented.
 *
 * It would be a nice extension to free blocks from the end of the
 * directory file which are no longer needed.
 *
 * Don't forget to dirty appropriate blocks!
 *
 * You probably want to use vget(), vput(), s5_read_file(),
 * s5_write_file(), and s5_dirty_inode().
 */
int
s5_remove_dirent(vnode_t *vnode, const char *name, size_t namelen)
{
	dbg(DBG_S5FS, "\n");

	uint32_t inum_f;
	s5_inode_t *i_f;
	vnode_t *vn_f;
	int index_f = -1;
	int index_last = -1;
	s5_dirent_t d;
	s5_dirent_t last_entry;
	int ret;

	int i;
	for (i = 0; i < (vnode->vn_len / (ssize_t)sizeof(s5_dirent_t)); i++) {
		ret = s5_read_file(vnode, i * sizeof(s5_dirent_t), (char *)&d,
				   sizeof(s5_dirent_t));
		KASSERT((ret != 0) && (ret % sizeof(s5_dirent_t) == 0));

		/* if name is found */
		if (name_match(d.s5d_name, name, namelen)) {
			/* decrease the refcount on the removed file. */
			inum_f = d.s5d_inode;

			KASSERT((vn_f = vget(vnode->vn_fs, inum_f)));

			i_f = VNODE_TO_S5INODE(vn_f);
			i_f->s5_linkcount--;
			s5_dirty_inode(VNODE_TO_S5FS(vn_f), i_f);

			vput(vn_f);

			/* the index of the removed file entry in the pdir */
			index_f = i;
		}

		/* if the end of directory entries is reached */
		if (name_match(d.s5d_name, "", 0)) {
			/* the index of the last entry */
			index_last = i - 1;
		}
	}

	/* if the removed file entry is not found */
	if (index_f == -1)
		return -ENOENT;

	if (index_last == -1)
		index_last = (vnode->vn_len / sizeof(s5_dirent_t)) - 1;

	KASSERT(index_f <= index_last);
	/* if the removed file entry is not the last entry */
	if (index_f != index_last) {
		/* copy the last entry to the removed file entry */
		ret = s5_read_file(vnode, index_last * sizeof(s5_dirent_t),
				   (char *)&last_entry, sizeof(s5_dirent_t));
		KASSERT((ret != 0) && (ret % sizeof(s5_dirent_t) == 0));

		ret = s5_write_file(vnode, index_f * sizeof(s5_dirent_t),
				    (char *)&last_entry, sizeof(s5_dirent_t));
		KASSERT((ret != 0) && (ret % sizeof(s5_dirent_t) == 0));
	}

	/* Now, remove the last entry */
	last_entry.s5d_name[0] = '\0';
	ret = s5_write_file(vnode, index_last * sizeof(s5_dirent_t),
			    (char *)&last_entry, sizeof(s5_dirent_t));
	KASSERT((ret != 0) && (ret % sizeof(s5_dirent_t) == 0));

	return 0;
}

/*
 * Create a new directory entry in directory 'parent' with the given name, which
 * refers to the same file as 'child'.
 *
 * When this function returns, the inode refcount on the file that was linked to
 * should be incremented.
 *
 * Remember to incrament the ref counts appropriately
 *
 * You probably want to use s5_find_dirent(), s5_write_file(), and s5_dirty_inode().
 */
int
s5_link(vnode_t *parent, vnode_t *child, const char *name, size_t namelen)
{
	dbg(DBG_S5FS, "\n");

	s5_dirent_t d;
	int ret;
	int index_last = -1;

	KASSERT(S_ISDIR(parent->vn_mode));

	int i;
	for (i = 0; i < (parent->vn_len / (ssize_t)sizeof(s5_dirent_t)); i++) {
		ret = s5_read_file(parent, i * sizeof(s5_dirent_t), (char *)&d,
				   sizeof(s5_dirent_t));
		KASSERT((ret != 0) && (ret % sizeof(s5_dirent_t) == 0));

		/* if name is found */
		if (name_match(d.s5d_name, name, namelen))
			return -EEXIST;

		/* if the end of directory entries is reached */
		if (name_match(d.s5d_name, "", 0)) {
			index_last = i;
			break;
		}
	}

	/* if no empty entry */
	if (index_last == -1)
		index_last = parent->vn_len / sizeof(s5_dirent_t);

	/* index_last is set; now, write dirent */
	KASSERT(namelen < S5_NAME_LEN);
	memcpy(d.s5d_name, name, namelen);
	d.s5d_name[namelen] = '\0';
	d.s5d_inode = VNODE_TO_S5INODE(child)->s5_number;
	ret = s5_write_file(parent, index_last * sizeof(s5_dirent_t),
			    (char *)&d, sizeof(s5_dirent_t));
	KASSERT(ret < 0 || ret == sizeof(s5_dirent_t));
	if (ret < 0)
		return ret;

	/* update child's linkcount */
	s5_inode_t *i_child = VNODE_TO_S5INODE(child);
	i_child->s5_linkcount++;
	s5_dirty_inode(VNODE_TO_S5FS(child), i_child);

	return 0;
}

/*
 * Return the number of blocks that this inode has allocated on disk.
 * This should include the indirect block, but not include sparse
 * blocks.
 *
 * This is only used by s5fs_stat().
 *
 * You'll probably want to use pframe_get().
 */
int
s5_inode_blocks(vnode_t *vnode)
{
	dbg(DBG_S5FS, "\n");

	int numblks = 0;
        s5_inode_t *i = VNODE_TO_S5INODE(vnode);
	int last;

	/* count direct blocks */
	last = MIN(S5_DATA_BLOCK(vnode->vn_len) + 1, S5_NDIRECT_BLOCKS);

	int j;
	for (j = 0; j < last; j++) {
		if (i->s5_direct_blocks[j])
			numblks++;
	}

	if (!i->s5_indirect_block)
		return numblks;

	/* count for indirect blocks */
	pframe_t *indir_blk_pf;
	uint32_t *blocks;
	int ret;

	last = MIN(S5_NDIRECT_BLOCKS / (ssize_t)sizeof(uint32_t),
		   S5_DATA_BLOCK(vnode->vn_len) + 1);

	ret = pframe_get(S5FS_TO_VMOBJ(VNODE_TO_S5FS(vnode)),
			 i->s5_indirect_block, &indir_blk_pf);
	if (ret < 0)
		return ret;

	blocks = (uint32_t *)indir_blk_pf->pf_addr;
	for (j = 0; j < last; j++) {
		if (blocks[j])
			numblks++;
	}

	return numblks;
}

