/*
 *  FILE: vnode.h
 *  AUTH: mcc
 *  DESC:
 *  DATE: Fri Mar 13 18:54:11 1998
 *  $Id: vnode.h,v 1.2.2.2 2006/06/04 01:02:32 afenn Exp $
 */

#pragma once

#include "drivers/dev.h"
#include "drivers/blockdev.h"
#include "drivers/bytedev.h"
#include "util/list.h"
#include "proc/kmutex.h"
#include "mm/mmobj.h"
#include "mm/pframe.h"

struct fs;
struct dirent;
struct stat;
struct vnode;
struct vmarea;

typedef struct vnode_ops {
        /* The following functions map directly to their corresponding
         * system calls. Unless otherwise noted, they return 0 on
         * success, and -errno on failure.
         */

        /* Operations that can be performed on non-directory files: */
        /*
         *     read transferrs at most count bytes from file into buf. It
         *     begins reading from the file at offset bytes into the file. On
         *     success, it returns the number of bytes transferred, or 0 if the
         *     end of the file has been reached (offset >= file->vn_len).
         */
        int (*read)(struct vnode *file, off_t offset, void *buf, size_t count);
        /*
         *     write transferrs count bytes from buf into file. It begins
         *     writing at offset bytes into the file. If offset+len extends
         *     past the end of the file, the file's length will be increased.
         *     If offset is before the end of the file, the existing data is
         *     overwritten. On success, it returns the number of bytes
         *     transferred.
         */
        int (*write)(struct vnode *file, off_t offset, const void *buf, size_t count);
        /*     Everything within 'vma' other than vma->vm_obj (and
         *     vm_link--meaning that 'vma' has not yet been entered into
         *     a list of vm_areas) will have been initialized before this
         *     entry point is called.
         *
         *     Implementations should supply an mmobj through the "ret"
         *     argument (not by setting vma->vm_obj). If for any reason
         *     this cannot be done an appropriate error code should be
         *     returned instead. This function may not perform any operations
         *     which must be undone later (the caller is responsible for ref-ing
         *     the returned object if necessary), nor may it block.
         */
        int (*mmap)(struct vnode *file, struct vmarea *vma, struct mmobj **ret);

        /* Operations that can be performed on directory files: */

        /*     create is called by open_namev(). it should vget() a new vnode,
         *     and create an entry for this vnode in 'dir' of the specified name.
         */
        int (*create)(struct vnode *dir, const char *name,
                      size_t name_len, struct vnode **result);

        /*
         *     mknod creates a special file for the device specified by
         *     'devid' and an entry for it in 'dir' of the specified name.
         */
        int (*mknod)(struct vnode *dir, const char *name, size_t name_len,
                     int mode, devid_t devid);
        /*
         *     lookup sets *result to the vnode in dir with the specified name.
         */
        int (*lookup)(struct vnode *dir, const char *name, size_t name_len,
                      struct vnode **result);
        /*
         *     link sets up a hard link. it links oldvnode into dir with the
         *     specified name.
         */
        int (*link)(struct vnode *oldvnode, struct vnode *dir,
                    const char *name, size_t name_len);
        /*
         *     unlink removes the link to the vnode in dir specified by name
         */
        int (*unlink)(struct vnode *dir, const char *name, size_t name_len);
        /*
         *     mkdir creates a directory called name in dir
         */
        int (*mkdir)(struct vnode *dir,  const char *name, size_t name_len);
        /*
         *     rmdir removes the directory called name from dir. the directory
         *     to be removed must be empty (except for . and .. of course).
         */
        int (*rmdir)(struct vnode *dir,  const char *name, size_t name_len);
        /*
         *     readdir reads one directory entry from the dir into the struct
         *     dirent. On success, it returns the amount that offset should be
         *     increased by to obtain the next directory entry with a
         *     subsequent call to readdir. If the end of the file as been
         *     reached (offset == file->vn_len), no directory entry will be
         *     read and 0 will be returned.
         */
        int (*readdir)(struct vnode *dir, off_t offset, struct dirent *d);

        /* Operations that can be performed on any type of file: */
        /*
         *     stat sets the fields in the given buf, filling it with
         *     information about file.
         */
        int (*stat)(struct vnode *vnode, struct stat *buf);

        /*
         *     Used by vnode vm_object entry points (and by no one else):
         */
        /*
         *         Read the page of 'vnode' containing 'offset' into the
         *         page-aligned and page-sized buffer pointed to by
         *         'pagebuf'.
         */
        int (*fillpage)(struct vnode *vnode, off_t offset, void *pagebuf);
        /*
         *         A hook; an attempt is being made to dirty the page
         *         belonging to 'vnode' that contains 'offset'. (If the
         *         underlying fs supports sparse blocks/pages, and the page
         *         containing this offset is currently sparse, this is
         *         where an attempt should be made to allocate a block in
         *         the underlying fs for that block/page). Return zero on
         *         success and nonzero otherwise (i.e., if there are no
         *         free blocks in the underlying fs, etc).
         */
        int (*dirtypage)(struct vnode *vnode, off_t offset);
        /*
         *         Write the contents of the page-aligned and page-sized
         *         buffer pointed to by 'pagebuf' to the page of 'vnode'
         *         containing 'offset'.
         */
        int (*cleanpage)(struct vnode *vnode, off_t offset, void *pagebuf);
} vnode_ops_t;


#define VN_BUSY        0x1

typedef struct vnode {
        struct vnode_ops   *vn_ops;         /* vnode operations */
        struct fs          *vn_fs;         /* vfs to which we belong */

#ifdef __MOUNTING__
        /* This field is used only for implementing mount points (not required) */
        /* This field points the the root of the file system mounted at
         * this vnode. If no file system is mounted at this point this is a
         * self pointer (i.e. vn->vn_mount = vn). See vget for why this is
         * makes things easier for us. */
        struct vnode       *vn_mount;
#endif

        /* VFS BLANK {{{ */
        /* XXX: changed because of big changes in vm_obj subsystem.
         * possible cleanup required */
        /* VFS BLANK }}} */
        struct mmobj       vn_mmobj;
#define                    vn_refcount     vn_mmobj.mmo_refcount
#define                    vn_nrespages    vn_mmobj.mmo_nrespages

        ino_t              vn_vno;         /* vnode number */
        int                vn_mode;        /* file type; see stat.h */


        /* vn_len is
         *     - Initialized at the fs-implementation-level (in the
         *       'read_vnode' fs_t entry point).
         *
         *     - Maintained at the fs-implementation-level (within the
         *       implementations of relevant vnode_t entry points).
         */
        off_t              vn_len;         /* length of file */

        kmutex_t           vn_mutex;       /* synchronize reads & writes */
        void              *vn_i;          /* filesystem-specific info */

        /* Relevant to vnodes representing device files: */
        /* VFS BLANK {{{ */
        /* XXX: also changed because of name changes to bytedev_t and blockdev_t */
        /* VFS BLANK }}} */
        devid_t            vn_devid;
        bytedev_t         *vn_cdev;
        blockdev_t        *vn_bdev;

        /* Used (only) by the v{get,ref,put} facilities (vfs/vnode.c): */
        list_link_t        vn_link;        /* link on vnode list */
        int                vn_flags;       /* VN_BUSY */
        ktqueue_t          vn_waitq;
} vnode_t;

/* Core vnode management routines: */
/*
 *     OVERVIEW/DISCUSSION (READ ME):
 *
 *         The following members of struct vnode play a primary role in
 *         vnode management:
 *
 *             - vn_refcount
 *                 - This integer counts every memory reference to the
 *                   relevant vnode; that is, it should be equal to the
 *                   number of (vnode_t *)'s that have the address of this
 *                   particular vnode_t as their value. The value of
 *                   vn_refcount is, by convention, controlled via
 *                   'vget(...)', 'vref(...)', and 'vput(...)'.
 *
 *
 *             - vn_nrespages
 *                 - Every resident page belonging to a given vnode is
 *                   represented by a vm_page structure (see vm_page.h for
 *                   the definition of this structure). Each such vm_page
 *                   structure points to a vm_object that is embedded
 *                   within the relevant vnode and, as a result of this,
 *                   vn_refcount is >= vn_nrespages.
 *
 *
 *         Life cycle of a vnode:
 *             - A vnode either doesn't exist or is in one of two states
 *               that we define as follows:
 *                 - (1) actively-referenced: (vn_refcount > vn_nrespages > 0)
 *                 - (2) passively-referenced: (vn_refcount == vn_nrespages > 0)
 *
 */

/*
 *     Obtain a vnode representing the file that filesystem 'fs' identifies
 *     by inode number 'vnum'; returns the vnode_t corresponding to the
 *     given filesystem and vnode number.
 *     There is no unsuccessful return.
 *
 *     MAY BLOCK.
 */
struct vnode *vget(struct fs *fs, ino_t vnum);

/*
 *     Increment the reference count of the provided vnode.
 */
void vref(vnode_t *vn);

/*
 *     This function decrements the reference count on this vnode.
 *
 *     If, as a result of this, vn_refcount reaches zero, the underlying
 *     fs's 'delete_vnode' entry point will be called and the vnode will be
 *     freed.
 *
 *     If, as a result of this, vn_refcount reaches vn_respages and
 *     vn_nrespages is > 0 (meaning only passive references exist) and
 *     the linkcount on the filesystem is zero (determined using query_vnode),
 *     all resident pages will be forcibly uncached, the underlying fs's
 *     'delete_vnode' entry point will be called, and the vnode will be freed.
 */
void vput(vnode_t *vn);


/* Auxilliary: */

/*     Unmounting (shutting down the VFS) is the primary reason for the
 *     existence of the following three routines (when unmounting an s5 fs,
 *     they are used in the order that they are listed here): */
/*
 *         Checks to see if there are any actively-referenced vnodes
 *         belonging to the specified filesystem.
 *         Returns -EBUSY if there is at least one such actively-referenced
 *         vnode, and 0 otherwise.
 *
 *         (Note: only actively-referenced vnodes affect the routine's
 *         outcome; (passively-referenced vnodes belonging to 'fs' do
 *         not)).
 */
int vfs_is_in_use(struct fs *fs);

/*
 *         Clean and uncache all resident pages of all vnodes belonging to
 *         the specified fs.
 */
void vnode_flush_all(struct fs *fs);

/*
 *         Returns the number of vnodes from this filesystem that are in
 *         use.
 */
int vnode_inuse(struct fs *fs);


/* Diagnostic: */
/*
 *     Prints the vnodes that are in use.  Specifying a fs_t will restrict
 *     the vnodes to just that fs.  Specifying NULL will print all vnodes
 *     in the entire system.
 */
void vnode_print(struct fs *fs);
