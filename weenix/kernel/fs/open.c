/*
 *  FILE: open.c
 *  AUTH: mcc | jal
 *  DESC:
 *  DATE: Mon Apr  6 19:27:49 1998
 */

#include "globals.h"
#include "errno.h"
#include "fs/fcntl.h"
#include "util/string.h"
#include "util/printf.h"
#include "fs/vfs.h"
#include "fs/vnode.h"
#include "fs/file.h"
#include "fs/vfs_syscall.h"
#include "fs/open.h"
#include "fs/stat.h"
#include "util/debug.h"

/* find empty index in p->p_files[] */
int
get_empty_fd(proc_t *p)
{
        int fd;

        for (fd = 0; fd < NFILES; fd++) {
                if (!p->p_files[fd])
                        return fd;
        }

        dbg(DBG_ERROR | DBG_VFS, "ERROR: get_empty_fd: out of file descriptors "
            "for pid %d\n", curproc->p_pid);
        return -EMFILE;
}

/*
 * There a number of steps to opening a file:
 *      1. Get the next empty file descriptor.
 *      2. Call fget to get a fresh file_t.
 *      3. Save the file_t in curproc's file descriptor table.
 *      4. Set file_t->f_mode to OR of FMODE_(READ|WRITE|APPEND) based on
 *         oflags, which can be O_RDONLY, O_WRONLY or O_RDWR, possibly OR'd with
 *         O_APPEND.
 *      5. Use open_namev() to get the vnode for the file_t.
 *      7. Fill in the fields of the file_t.
 *      8. Return new fd.
 *
 * If anything goes wrong at any point (specifically if the call to open_namev
 * fails), be sure to remove the fd from curproc, fput the file_t and return an
 * error.
 */
int
do_open(const char *filename, int oflags)
{
	int fd;
	file_t *f;
	int ret = 0;
	vnode_t *vn;

	/* 1. Get the next empty file descriptor */
	if ((fd= get_empty_fd(curproc)) < 0)
		return fd;	

	/* 2. Call fget to get a fresh file_t */
	f = fget(-1);

	/* 3. Save the file_t in curproc's file descriptor table */
	curproc->p_files[fd] = f;

	/* 4. Set file_t->f_mode to OR of FMODE_(READ|WRITE|APPEND) based on 
	   oflags, which can be O_RDONLY, O_WRONLY or O_RDWR, possibly OR'd with
	   O_APPEND. */
	if (oflags & O_APPEND) {
		f->f_mode |= FMODE_APPEND;
	}

	switch (oflags & 3) {
	case O_RDONLY:
		f->f_mode |= FMODE_READ;
		break;
	case O_WRONLY:
		f->f_mode |= FMODE_WRITE;
		break;
	case O_RDWR:
		f->f_mode |= FMODE_WRITE;
		f->f_mode |= FMODE_READ;
		break;
	default:
		ret = -EINVAL;
		goto err;
	}

	/* 5. Use open_namev() to get the vnode for the file_t. */
	ret = open_namev(filename, oflags, &vn, curproc->p_cwd);
	if (ret)
		goto err;

	if (S_ISDIR(vn->vn_mode) && (oflags & (O_RDWR | O_WRONLY))) {
		vput(vn);
		ret = -EISDIR;
		goto err;
	}
		
	/* 6. Fill in the fields of the file_t. */
	f->f_vnode = vn;

	/* 7. Return new fd. */
	return fd;

err:
	curproc->p_files[fd] = NULL;
	fput(f);
	KASSERT(ret < 0);
	return ret;
}
