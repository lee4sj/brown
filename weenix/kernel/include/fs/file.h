#pragma once

#include "types.h"

#define FMODE_READ    1
#define FMODE_WRITE   2
#define FMODE_APPEND  4

struct vnode;

typedef struct file {
        off_t                   f_pos;          /* seek pointer */
        int                     f_mode;         /* access type */
        int                     f_refcount;     /* reference count */
        struct vnode            *f_vnode;       /* related vnode */
} file_t;

/*
 * Returns the file_t assiciated with the given file descriptor for the
 * current process. If there is no associated file_t, returns NULL.
 *
 * If the passed fd == -1, a new file_t, not associated with any process
 * or file descriptor, is created and returned with its refcount set to
 * 1.
 */
struct file *fget(int fd);

/*
 * fref() increments the reference count on the given file.
 */
void fref(file_t *f);

/*
 * fput() decrements the reference count on the given file.
 *
 * If the refcount reaches 0, the storage for the given file_t will be
 * released, and the refcount on the associated vnode (if any) will be
 * decremented.
 */
void fput(file_t *f);
