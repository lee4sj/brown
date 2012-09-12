/*
 *  FILE: vfs_syscall.c
 *  AUTH: mcc | jal
 *  DESC:
 *  DATE: Wed Apr  8 02:46:19 1998
 *  $Id: vfs_syscall.c,v 1.9.2.2 2006/06/04 01:02:32 afenn Exp $
 */

#include "kernel.h"
#include "errno.h"
#include "globals.h"
#include "fs/vfs.h"
#include "fs/file.h"
#include "fs/vnode.h"
#include "fs/vfs_syscall.h"
#include "fs/open.h"
#include "fs/fcntl.h"
#include "fs/lseek.h"
#include "mm/kmalloc.h"
#include "util/string.h"
#include "util/printf.h"
#include "fs/stat.h"
#include "util/debug.h"

/* To read a file:
 *      o fget(fd)
 *      o call its virtual read f_op
 *      o update f_pos
 *      o fput() it
 *      o return the number of bytes read, or an error
 *
 * Errors to look out for:
 *      o fget() returns NULL (bad file descriptor) (return -EBADF).
 *      o file_t->f_mode does not have FMODE_READ set (return -EBADF)
 *      o file vnode does not have a read vn_op (it's NULL) (return -EINVAL)
 *      o file is actually a directory (return -EISDIR)
 *
 * In all cases, be sure you do not leak file refcounts by returning before
 * you fput() a file that you fget()'ed.
 */
int
do_read(int fd, void *buf, size_t nbytes)
{
        vnode_t *vn;
	file_t *f;
	int ret;

	if (fd < 0 || fd >= NFILES)
		return -EBADF;

	if (!(f = fget(fd)))
		return -EBADF;

	if (!(f->f_mode & FMODE_READ)) {
		fput(f);
		return -EBADF;
	}
		
	vn = f->f_vnode;

	if (!vn->vn_ops->read) {
		fput(f);
		return -EISDIR;
	}

	ret = vn->vn_ops->read(vn, f->f_pos, buf, nbytes);
	if (ret > 0)
		do_lseek(fd, ret, SEEK_CUR);

	fput(f);
	return ret;
}

/* Very similar to do_read.  Check f_mode to be sure the file is writable.  If
 * f_mode & FMODE_APPEND, do_lseek() to the end of the file, call the write
 * f_op, and fput the file.  As always, be mindful of errors and refcount leaks.
 */
int
do_write(int fd, const void *buf, size_t nbytes)
{
	vnode_t *vn;
	file_t *f;
	int ret;

	if (fd < 0 || fd >= NFILES)
		return -EBADF;

	if (!(f = fget(fd)))
		return -EBADF;

	if (!(f->f_mode & FMODE_WRITE)) {
		fput(f);
		return -EBADF;
	}

	if (f->f_mode & FMODE_APPEND) {
		do_lseek(fd, 0, SEEK_END);
	}

	vn = f->f_vnode;
	if (!vn->vn_ops->write) {
		fput(f);
		return -EISDIR;
	}

	ret = vn->vn_ops->write(vn, f->f_pos, buf, nbytes);
	if (ret > 0)
		do_lseek(fd, ret, SEEK_CUR);

	fput(f);
	return ret;
}

/*
 * Zero curproc->p_files[fd], and fput() the file. Return 0 on success
 */
int
do_close(int fd)
{
	file_t *f;

	if (fd < 0 || fd >= NFILES)
		return -EBADF;

	if (!(f = curproc->p_files[fd]))
		return -EBADF;

	curproc->p_files[fd] = NULL;
	fput(f);

	return 0;
}

/* To dup a file:
 *      o fget(fd) to up fd's refcount
 *      o get_empty_fd()
 *      o point the new fd to the same file_t* as the given fd
 *      o return the new file descriptor
 *
 * Don't fput() the fd unless something goes wrong.  Since we are creating
 * another reference to the file_t*, we want to up the refcount.
 */
int
do_dup(int fd)
{
        file_t *f;
	int dup_fd;

	if (fd < 0 || fd >= NFILES)
		return -EBADF;

	if (!(f = fget(fd)))
		return -EBADF;

	if ((dup_fd = get_empty_fd(curproc)) < 0) {
		fput(f);
		return dup_fd;
	}

	curproc->p_files[dup_fd] = f;
	return dup_fd;
}

/* Same as do_dup, but insted of using get_empty_fd() to get the new fd,
 * they give it to us in 'nfd'.  If nfd is in use (and not the same as ofd)
 * do_close() it first.  Then return the new file descriptor.
 */
int
do_dup2(int ofd, int nfd)
{
	file_t *f;
	int err;

	if (ofd < 0 || ofd >= NFILES || nfd < 0 || nfd >= NFILES)
		return -EBADF;

	if (!(f = fget(ofd)))
		return -EBADF;

	if (ofd == nfd) {
		fput(f);
		return nfd;
	}
	
	if (curproc->p_files[nfd]) {
		if ((err = do_close(nfd))) {
			fput(f);
			return err;
		}
	}

	curproc->p_files[nfd] = f;

        return nfd;
}

/*
 * This routine creates a special file of the type specified by 'mode' at
 * the location specified by 'path'. 'mode' should be one of S_IFCHR or
 * S_IFBLK (you might note that mknod(2) normally allows one to create
 * regular files as well-- for simplicity this is not the case in Weenix).
 * 'devid', as you might expect, is the device identifier of the device
 * that the new special file should represent.
 *
 * You might use a combination of dir_namev, lookup, and the fs-specific
 * mknod (that is, the containing directory's 'mknod' vnode operation).
 * Return the result of the fs-specific mknod, or an error.
 */
int
do_mknod(const char *path, int mode, unsigned devid)
{
	int ret;
	size_t namelen;
	char *name;
	vnode_t *dir;
	vnode_t *vn;

	if (!S_ISCHR(mode) && !S_ISBLK(mode))
		return -EINVAL;

	if ((ret = dir_namev(path, &namelen, (const char **)&name, NULL, &dir)))
		return ret;

	if (!lookup(dir, name, namelen, &vn)) {
		vput(dir);
		vput(vn);
		return -EEXIST;
	}

	if (!dir->vn_ops->mknod) {
		vput(dir);
		return -ENOSYS;
	}

	ret = dir->vn_ops->mknod(dir, name, namelen, mode, devid);
	vput(dir);

	return ret;
}

/* Use dir_namev() to find the vnode of the dir we want to make the new
 * directory in.  Then use lookup() to make sure it doesn't already exist.
 * Finally call the dir's mkdir vn_ops. Return what it returns.
 */
int
do_mkdir(const char *path)
{
	char *name;
	size_t namelen;
	vnode_t *dir;
	vnode_t *vn = NULL;
	int ret;

	dbg(DBG_VFS, "%s\n", path);

	if ((ret = dir_namev(path, &namelen, (const char **)&name, NULL, &dir)))
		return ret;

	if (namelen > NAME_LEN) {
		vput(dir);
		return -ENAMETOOLONG;
	}

	if (!lookup(dir, name, namelen, &vn)) {
		vput(dir);
		vput(vn);
		return -EEXIST;
	}

	if (!dir->vn_ops->mkdir) {
		vput(dir);
		return -ENOTDIR;
	}

	ret = dir->vn_ops->mkdir(dir, name, namelen);
	vput(dir);

        return ret;
}

/* Use dir_namev() to find the vnode of the directory containing the dir to be
 * removed. Then call the containing dir's rmdir v_op.  The rmdir v_op will
 * return an error if the dir to be removed does not exist or is not empty, so
 * you don't need to worry about that here. Return the value of the v_op,
 * or an error.
 */
int
do_rmdir(const char *path)
{
	char *name;
	size_t namelen;
	vnode_t *dir;
	int ret;

	if ((ret = dir_namev(path, &namelen, (const char **)&name, NULL, &dir)))
		return ret;

	if (!strncmp(name, ".", namelen)) {
		vput(dir);
		return -EINVAL;
	}

	if (!strncmp(name, "..", namelen)) {
		vput(dir);
		return -ENOTEMPTY;
	}

	if (!dir->vn_ops->rmdir) {
		vput(dir);
		return -ENOTDIR;
	}
	
	ret = dir->vn_ops->rmdir(dir, name, namelen);

	vput(dir);
        return ret;
}

/*
 * Same as do_rmdir, but for files.
 */
int
do_unlink(const char *path)
{
	char *name;
	size_t namelen;
	vnode_t *dir;
	vnode_t *vn;
	int ret;

	if ((ret = dir_namev(path, &namelen, (const char **)&name, NULL, &dir)))
		return ret;

	if (!strncmp(name, ".", namelen)) {
		vput(dir);
		return -EINVAL;
	}

	if (!strncmp(name, "..", namelen)) {
		vput(dir);
		return -ENOTEMPTY;
	}

	if ((ret = lookup(dir, name, namelen, &vn))) {
		vput(dir);
		return ret;
	}

	if (S_ISDIR(vn->vn_mode)) {
		vput(dir);
		vput(vn);
		return -EPERM;
	}

	vput(vn);

	if (!dir->vn_ops->unlink) {
		vput(dir);
		return -ENOENT;
	}

	ret = dir->vn_ops->unlink(dir, name, namelen);

	vput(dir);
        return ret;
}

/* To link:
 *      o open_namev(from)
 *      o dir_namev(to)
 *      o call the destination dir's (to) link vn_ops.
 *      o return the result of link, or an error
 *
 * Remember to vput the vnodes returned from open_namev and dir_namev.
 */
int
do_link(const char *from, const char *to)
{
        char *name;
	size_t namelen;
	vnode_t *vn_from;
	vnode_t *dir;
	int ret;

	if ((ret = open_namev(from, 0, &vn_from, NULL)))
		return ret;

	if (S_ISDIR(vn_from->vn_mode)) {
		vput(vn_from);
		return -EPERM;
	}

	if ((ret = dir_namev(to, &namelen, (const char **)&name, NULL, &dir))) {
		vput(vn_from);
		return ret;
	}

	if (!dir->vn_ops->link) {
		vput(vn_from);
		vput(dir);
		return -ENOSYS;
	}

	ret = dir->vn_ops->link(vn_from, dir, name, namelen);
	vput(vn_from);
	vput(dir);

	return ret;
}

/*      o link newname to oldname
 *      o unlink oldname
 *      o return the value of unlink, or an error
 *
 * Note that this does not provide the same behavior as the
 * Linux system call (if unlink fails then two links to the
 * file could exist).
 */
int
do_rename(const char *oldname, const char *newname)
{
        int ret;

	if ((ret = do_link(oldname, newname)))
		return ret;

	return do_unlink(oldname);
}

/* Make the named directory the current process's cwd (current working
 * directory). Be sure it's actually a directory (return -ENOTDIR if it's not).
 * Don't forget to down the refcount to the old cwd (vput()) and up the
 * refcount to the new cwd (open_namev() or vget()). Return 0 on success.
 */
int
do_chdir(const char *path)
{
	vnode_t *ovn;
	vnode_t *vn;
	int ret;

	if ((ret = open_namev(path, 0, &vn, NULL)))
		return ret;

	if (!S_ISDIR(vn->vn_mode)) {
		vput(vn);
		return -ENOTDIR;
	}

	ovn = curproc->p_cwd;
	curproc->p_cwd = vn;
	vput(ovn);

        return ret;
}

/* Call the readdir f_op on the given fd, filling in the given dirent_t*.
 * If the readdir f_op is successful, it will return a positive value which
 * is the number of bytes copied to the dirent_t.  You need to increment the
 * file_t's f_pos by this amount.  As always, be aware of refcounts, check
 * the return value of the fget and the virtual function, and be sure the
 * virtual function exists (is not null) before calling it.
 *
 * Return either 0 or sizeof(dirent_t), or -errno.
 */
int
do_getdent(int fd, struct dirent *dirp)
{
	file_t *f;
	int ret;

	if (fd < 0 || fd >= NFILES)
		return -EBADF;

	if (!(f = fget(fd)))
		return -EBADF;

	if (!f->f_vnode->vn_ops->readdir) {
		fput(f);
		return -ENOTDIR;
	}

	ret = f->f_vnode->vn_ops->readdir(f->f_vnode, f->f_pos, dirp);
	do_lseek(fd, ret, SEEK_CUR);
	fput(f);

	if (ret > 0)
		ret = sizeof(dirent_t);

	return ret;
}

/*
 * Modify f_pos according to offset and whence.
 */
int
do_lseek(int fd, int offset, int whence)
{
	file_t *f;
	off_t ret = 0;

	if (fd < 0 || fd >= NFILES)
		return -EBADF;

	if (!(f = fget(fd)))
		return -EBADF;

	switch (whence) {
	case SEEK_SET:
		ret = offset;
		break;
	case SEEK_CUR:
		ret = f->f_pos + offset;
		break;
	case SEEK_END:
		ret = f->f_vnode->vn_len + offset;
		break;
	default:
		fput(f);
		return -EINVAL;
	}

	if (ret < 0) {
		fput(f);
		return -EINVAL;
	}

	f->f_pos = ret;
	fput(f);
        return ret;
}

/*
 * Find the vnode associated with the path, and call the stat() vnode operation.
 */
int
do_stat(const char *path, struct stat *buf)
{
	vnode_t *vn;
	int ret;

	if ((ret = open_namev(path, 0, &vn, NULL)))
		return ret;

	if (!vn->vn_ops->stat) {
		vput(vn);
		return -ENOSYS;
	}

	ret = vn->vn_ops->stat(vn, buf);

	vput(vn);
	return ret;
}

#ifdef __MOUNTING__
/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutley sure your Weenix is perfect.
 *
 * This is the syscall entry point into vfs for mounting. You will need to
 * create the fs_t struct and populate its fs_dev and fs_type fields before
 * calling vfs's mountfunc(). mountfunc() will use the fields you populated
 * in order to determine which underlying filesystem's mount function should
 * be run, then it will finish setting up the fs_t struct. At this point you
 * have a fully functioning file system, however it is not mounted on the
 * virtual file system, you will need to call vfs_mount to do this.
 *
 * There are lots of things which can go wrong here. Make sure you have good
 * error handling. Remember the fs_dev and fs_type buffers have limited size
 * so you should not write arbitrary length strings to them.
 */
int
do_mount(const char *source, const char *target, const char *type)
{
        NOT_YET_IMPLEMENTED("MOUNTING: do_mount");
        return -EINVAL;
}

/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutley sure your Weenix is perfect.
 *
 * This function delegates all of the real work to vfs_umount. You should not worry
 * about freeing the fs_t struct here, that is done in vfs_umount. All this function
 * does is figure out which file system to pass to vfs_umount and do good error
 * checking.
 */
int
do_umount(const char *target)
{
        NOT_YET_IMPLEMENTED("MOUNTING: do_umount");
        return -EINVAL;
}
#endif
