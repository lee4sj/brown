#pragma once

#include "types.h"

#include "fs/open.h"
#include "util/list.h"

struct vnode;
struct file;
struct vfs;
struct fs;

/* name_match: fname should be null-terminated, name is namelen long */
#define name_match(fname, name, namelen) \
        ( strlen(fname) == namelen && !strncmp((fname), (name), (namelen)) )

typedef struct fs_ops {
        /*
         * This is called by vget.
         *
         * read_vnode will be passed a vnode_t*, which will have its vn_fs
         * and vn_vno fields initialized.
         *
         * read_vnode must initialize the following members of the provided
         * vnode_t:
         *     vn_ops
         *     vn_mode
         *         and vn_devid if appropriate
         *     vn_len
         *     vn_i
         *
         * This entry point is ALLOWED TO BLOCK.
         */
        void (*read_vnode)(struct vnode *vn);

        /*
         * The inverse of read_vnode; delete_vnode is called by vput when the
         * specified vnode_t no longer needs to exist (it is neither actively
         * nor passively referenced).
         *
         * This entry point is ALLOWED TO BLOCK.
         */
        void (*delete_vnode)(struct vnode *vn);

        /*
         * Returns 1 if the vnode still exists in the filesystem, 0 of it can
         * be deleted. Called by vput when there are no active references to
         * the vnode. If query_vnode returns 0, vput evicts all pages of the vnode
         * from memory so that it can be deleted.
         */
        int (*query_vnode)(struct vnode *vn);

        /*
         * Unmount the filesystem, performing any necessary reference count
         * checks.  Returns 0 on success, negative number on error.  If this
         * is NULL, the default action (vput(fs->fs_root)) is performed.
         *
         * This entry point is ALLOWED TO BLOCK.
         */
        int (*umount)(struct fs *fs);
} fs_ops_t;

#ifndef STR_MAX
#define STR_MAX 32
#endif

/* similar to Linux's super_block. */
typedef struct fs {
        char            fs_dev[STR_MAX];
        char            fs_type[STR_MAX];

#ifdef __MOUNTING__
        /* If mounting is implemented (not-required) then this should point to
         * the vnode of the file that this file system is mounted on. For the root
         * file system this will just point to the root of that file system. */
        struct vnode    *fs_mtpt;
        list_link_t     fs_link;
#endif

        /* Initialized by the fs-implementation's mount routine: */
        fs_ops_t        *fs_op;           /* vfs operations */
        struct vnode    *fs_root;         /* the root vnode of this vfs */
        void            *fs_i;            /* instance specific data */
} fs_t;

/* - this is the vnode on which we will mount the vfsroot fs.
 */
extern struct vnode *vfs_root_vn;

/* TA BLANK {{{ */
/*
 * - called by the idle process at system shutdown
 * - at this point, idleproc is the only process running
 *     => so, there should be no "live" vnodes
 *
 * unmount the root filesystem (and first unmount any filesystems mounted
 * on the root filesystem in the proper order (bottom up)).
 *
 * At that point, there should be no actively-being-used vnodes since all
 * processes other than pageoutd will have exited.
 */

/* TA BLANK }}} */
/* VFS Shutdown: */
/*
 *     Called by the idle process at system shutdown.
 */
int vfs_shutdown();

/* Pathname resolution: */
/* (the corresponding definitions live in namev.c) */
int lookup(struct vnode *dir, const char *name, size_t len,
           struct vnode **result);
int dir_namev(const char *pathname, size_t *namelen, const char **name,
              struct vnode *base, struct vnode **res_vnode);
int open_namev(const char *pathname, int flag,
               struct vnode **res_vnode, struct vnode *base);

#ifdef __GETCWD__
int lookup_name(struct vnode *dir, struct vnode *entry, char *buf, size_t size);
int lookup_dirpath(struct vnode *dir, char *buf, size_t size);
#endif /* __GETCWD__ */

int mountfunc(fs_t *fs);

#ifdef __MOUNTING__
int vfs_mount(struct vnode *mtpt, fs_t *fs);
int vfs_umount(fs_t *fs);
#endif

