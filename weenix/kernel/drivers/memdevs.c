#include "errno.h"
#include "globals.h"

#include "util/string.h"
#include "util/debug.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/mmobj.h"
#include "mm/kmalloc.h"
#include "mm/pframe.h"

#include "drivers/bytedev.h"

#include "vm/anon.h"

#include "fs/vnode.h"

static int null_read(bytedev_t *dev, int offset, void *buf, int count);
static int null_write(bytedev_t *dev, int offset, const void *buf, int count);

static int zero_read(bytedev_t *dev, int offset, void *buf, int count);
static int zero_mmap(vnode_t *file, vmarea_t *vma, mmobj_t **ret);

bytedev_ops_t null_dev_ops = {
        null_read,
        null_write,
        NULL,
        NULL,
        NULL,
        NULL
};

bytedev_ops_t zero_dev_ops = {
        zero_read,
        null_write,
        zero_mmap,
        NULL,
        NULL,
        NULL
};

/*
 * The byte device code needs to know about these mem devices, so create
 * bytedev_t's for null and zero, fill them in, and register them.
 */
void
memdevs_init()
{
	bytedev_t *ndev = (bytedev_t *)kmalloc(sizeof(bytedev_t));
	if (ndev == NULL)
		panic("Not enough memory for null mem device");
	ndev->cd_id = MEM_NULL_DEVID;
	ndev->cd_ops = &null_dev_ops;
	bytedev_register(ndev);

	bytedev_t *zdev = (bytedev_t *)kmalloc(sizeof(bytedev_t));
	if (zdev == NULL)
		panic("Not enough memory for zero mem device");
	zdev->cd_id = MEM_ZERO_DEVID;
	zdev->cd_ops = &zero_dev_ops;
	bytedev_register(zdev);
}

/**
 * Reads a given number of bytes from the null device into a
 * buffer. Any read performed on the null device should read 0 bytes.
 *
 * @param dev the null device
 * @param offset the offset to read from. Should be ignored
 * @param buf the buffer to read into
 * @param count the maximum number of bytes to read
 * @return the number of bytes read, which should be 0
 */
static int
null_read(bytedev_t *dev, int offset, void *buf, int count)
{
        return 0;
}

/**
 * Writes a given number of bytes to the null device from a
 * buffer. Writing to the null device should _ALWAYS_ be successful
 * and write the maximum number of bytes.
 *
 * @param dev the null device
 * @param offset the offset to write to. Should be ignored
 * @param buf buffer to read from
 * @param count the maximum number of bytes to write
 * @return the number of bytes written, which should be count
 */
static int
null_write(bytedev_t *dev, int offset, const void *buf, int count)
{
	return count;
}

/**
 * Reads a given number of bytes from the zero device into a
 * buffer. Any read from the zero device should be a series of zeros.
 *
 * @param dev the zero device
 * @param offset the offset to read from. Should be ignored
 * @param buf the buffer to write to
 * @param count the maximum number of bytes to read
 * @return the number of bytes read. Should always read the maximum
 * number of bytes
 */
static int
zero_read(bytedev_t *dev, int offset, void *buf, int count)
{
	memset(buf, '\0', count);
        return count;
}

/* Don't worry about these until VM. Once you're there, they shouldn't be hard. */

static int
zero_mmap(vnode_t *file, vmarea_t *vma, mmobj_t **ret)
{
	dbg(DBG_VM, "\n");
	*ret = anon_create();

	return 0;
}
