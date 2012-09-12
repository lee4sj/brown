#include "kernel.h"
#include "globals.h"
#include "types.h"
#include "errno.h"

#include "util/string.h"
#include "util/printf.h"
#include "util/debug.h"

#include "fs/dirent.h"
#include "fs/fcntl.h"
#include "fs/stat.h"
#include "fs/vfs.h"
#include "fs/vnode.h"

/* This takes a base 'dir', a 'name', its 'len', and a result vnode.
 * Most of the work should be done by the vnode's implementation
 * specific lookup() function, but you may want to special case
 * "." and/or ".." here depnding on your implementation.
 *
 * If dir has no lookup(), return -ENOTDIR.
 *
 * Note: returns with the vnode refcount on *result incremented.
 */
int
lookup(vnode_t *dir, const char *name, size_t len, vnode_t **result)
{
        if (!dir->vn_ops->lookup)
		return -ENOTDIR;

	return dir->vn_ops->lookup(dir, name, len, result);
}

static int
next_namelen(const char *pathname)
{
	int i = 0;

	while (pathname[i] != '/' && pathname[i] != '\0') {
		i++;
	}

	return i;
}

/* When successful this function returns data in the following "out"-arguments:
 *  o res_vnode: the vnode of the parent directory of "name"
 *  o name: the `basename' (the element of the pathname)
 *  o namelen: the length of the basename
 *
 * For example: dir_namev("/s5fs/bin/ls", &namelen, &name, NULL,
 * &res_vnode) would put 2 in namelen, "ls" in name, and a pointer to the
 * vnode corresponding to "/s5fs/bin" in res_vnode.
 *
 * The "base" argument defines where we start resolving the path from:
 * A base value of NULL means to use the process's current working directory,
 * curproc->p_cwd.  If pathname[0] == '/', ignore base and start with
 * vfs_root_vn.  dir_namev() should call lookup() to take care of resolving each
 * piece of the pathname.
 *
 * Note: A successful call to this causes vnode refcount on *res_vnode to
 * be incremented.
 */
int
dir_namev(const char *pathname, size_t *namelen, const char **name,
          vnode_t *base, vnode_t **res_vnode)
{
	int len;
	const char *cur_pathname;
	const char *next_pathname;
	int ret = 0;

	if (pathname[0] == '\0')
		return -ENOENT;

	len = 0;
	cur_pathname = pathname;

        if (pathname[0] == '/') {
		cur_pathname++;
		base = vfs_root_vn;
	}

	if (!base)
		base = curproc->p_cwd;

	*res_vnode = base;
	next_pathname = cur_pathname;
	vref(base);
	vref(*res_vnode);

	while ((next_pathname = strchr(next_pathname, '/')) &&
		next_pathname[1] != '\0') {

		vput(base);
		base = *res_vnode;

		if (next_pathname[1] == '/') {
			vref(*res_vnode);
			next_pathname++;
			continue;
		}

		len = (strchr(cur_pathname, '/') - cur_pathname);
		if ((ret = lookup(base, cur_pathname, len, res_vnode))) {
			vput(base);
			*res_vnode = NULL;
			*name = NULL;

			return ret;
		}

		next_pathname++;
		cur_pathname = next_pathname;
	}

	vput(base);
	*name = cur_pathname;
	next_pathname = strchr(cur_pathname, '/');

	if (next_pathname)
		*namelen = (next_pathname - cur_pathname);
	else
		*namelen = strlen(cur_pathname);

	return 0;
}

/* This returns in res_vnode the vnode requested by the other parameters.
 * It makes use of dir_namev and lookup to find the specified vnode (if it
 * exists).  flag is right out of the parameters to open(2); see
 * <weenix/fnctl.h>.  If the O_CREAT flag is specified, and the file does
 * not exist call create() in the parent directory vnode.
 *
 * Note: Increments vnode refcount on *res_vnode.
 */
int
open_namev(const char *pathname, int flag, vnode_t **res_vnode, vnode_t *base)
{
	size_t namelen;
	char *name;
	vnode_t *pdir;
	int ret;

	if ((ret = dir_namev(pathname, &namelen, (const char **)&name,
			     base, &pdir)) != 0)
		return ret;

	if (namelen >= NAME_LEN) {
		vput(pdir);
		return -ENAMETOOLONG;
	}

	ret = lookup(pdir, name, namelen, res_vnode);

	if (ret == -ENOENT && (flag & O_CREAT)) {
		if ((ret =
		     pdir->vn_ops->create(pdir, name, namelen, res_vnode)) < 0) {
			vput(pdir);
			return ret;
		}
	}

	vput(pdir);
        return ret;
}

#ifdef __GETCWD__
/* Finds the name of 'entry' in the directory 'dir'. The name is writen
 * to the given buffer. On success 0 is returned. If 'dir' does not
 * contain 'entry' then -ENOENT is returned. If the given buffer cannot
 * hold the result then it is filled with as many characters as possible
 * and a null terminator, -ERANGE is returned.
 *
 * Files can be uniquely identified within a file system by their
 * inode numbers. */
int
lookup_name(vnode_t *dir, vnode_t *entry, char *buf, size_t size)
{
        NOT_YET_IMPLEMENTED("GETCWD: lookup_name");
        return -ENOENT;
}


/* Used to find the absolute path of the directory 'dir'. Since
 * directories cannot have more than one link there is always
 * a unique solution. The path is writen to the given buffer.
 * On success 0 is returned. On error this function returns a
 * negative error code. See the man page for getcwd(3) for
 * possible errors. Even if an error code is returned the buffer
 * will be filled with a valid string which has some partial
 * information about the wanted path. */
ssize_t
lookup_dirpath(vnode_t *dir, char *buf, size_t osize)
{
        NOT_YET_IMPLEMENTED("GETCWD: lookup_dirpath");

        return -ENOENT;
}
#endif /* __GETCWD__ */
