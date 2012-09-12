#include "drivers/tty/n_tty.h"

#include "errno.h"

#include "drivers/tty/driver.h"
#include "drivers/tty/ldisc.h"
#include "drivers/tty/tty.h"

#include "mm/kmalloc.h"

#include "proc/kthread.h"

#include "util/debug.h"

/* helpful macros */
#define EOFC            '\x4'
#define TTY_BUF_SIZE    128
#define ldisc_to_ntty(ldisc) \
        CONTAINER_OF(ldisc, n_tty_t, ntty_ldisc)

static void n_tty_attach(tty_ldisc_t *ldisc, tty_device_t *tty);
static void n_tty_detach(tty_ldisc_t *ldisc, tty_device_t *tty);
static int n_tty_read(tty_ldisc_t *ldisc, void *buf, int len);
static const char *n_tty_receive_char(tty_ldisc_t *ldisc, char c);
static const char *n_tty_process_char(tty_ldisc_t *ldisc, char c);

static tty_ldisc_ops_t n_tty_ops = {
        .attach       = n_tty_attach,
        .detach       = n_tty_detach,
        .read         = n_tty_read,
        .receive_char = n_tty_receive_char,
        .process_char = n_tty_process_char
};

struct n_tty {
        kmutex_t            ntty_rlock;
        ktqueue_t           ntty_rwaitq;
        char               *ntty_inbuf;
        int                 ntty_rhead;
        int                 ntty_rawtail;
        int                 ntty_ckdtail;

	tty_ldisc_t         ntty_ldisc;

	/* For n_tty_receive_char return value*/
	char                ret[5];
};

static int buf_empty(int rhead, int ckdtail) {
	return !(ckdtail - rhead);
}

static int buf_full(int rhead, int rawtail) {
	return !(rawtail + 1 - rhead);
}

tty_ldisc_t *
n_tty_create()
{
        n_tty_t *ntty = (n_tty_t *)kmalloc(sizeof(n_tty_t));
        if (NULL == ntty) return NULL;
        ntty->ntty_ldisc.ld_ops = &n_tty_ops;
        return &ntty->ntty_ldisc;
}

void
n_tty_destroy(tty_ldisc_t *ldisc)
{
        KASSERT(NULL != ldisc);
        kfree(ldisc_to_ntty(ldisc));
}

/*
 * Initialize the fields of the n_tty_t struct, allocate any memory
 * you will need later, and set the tty_ldisc field of the tty.
 */
void
n_tty_attach(tty_ldisc_t *ldisc, tty_device_t *tty)
{
	n_tty_t *ntty = ldisc_to_ntty(ldisc);
	kmutex_init(&ntty->ntty_rlock);
	sched_queue_init(&ntty->ntty_rwaitq);

	ntty->ntty_inbuf = (char *)kmalloc(TTY_BUF_SIZE);
	KASSERT(ntty->ntty_inbuf);

	ntty->ntty_rhead = 0;
	ntty->ntty_rawtail = 0;
	ntty->ntty_ckdtail = 0;

	tty->tty_ldisc = ldisc;
}

/*
 * Free any memory allocated in n_tty_attach and set the tty_ldisc
 * field of the tty.
 */
void
n_tty_detach(tty_ldisc_t *ldisc, tty_device_t *tty)
{
        tty->tty_ldisc = NULL;

	n_tty_t *ntty = ldisc_to_ntty(ldisc);
	KASSERT(sched_queue_empty(&ntty->ntty_rwaitq));

	KASSERT(ntty->ntty_inbuf != NULL);
	kfree(ntty->ntty_inbuf);

	n_tty_destroy(ldisc);
}

/*
 * Read a maximum of len bytes from the line discipline into buf. If
 * the buffer is empty, sleep until some characters appear. This might
 * be a long wait, so it's best to let the thread be cancellable.
 *
 * Then, read from the head of the buffer up to the tail, stopping at
 * len bytes or a newline character, and leaving the buffer partially
 * full if necessary. Return the number of bytes you read into the
 * buf.

 * In this function, you will be accessing the input buffer, which
 * could be modified by other threads. Make sure to make the
 * appropriate calls to ensure that no one else will modify the input
 * buffer when we are not expecting it.
 *
 * Remember to handle newline characters and CTRL-D, or ASCII 0x04,
 * properly.
 */
int
n_tty_read(tty_ldisc_t *ldisc, void *buffer, int len)
{
	char *buf = (char *)buffer;
        n_tty_t *ntty = ldisc_to_ntty(ldisc);

	kmutex_lock(&ntty->ntty_rlock);

	/* if the buffer is empty, wait */
	if (buf_empty(ntty->ntty_rhead, ntty->ntty_ckdtail)) {
		kmutex_unlock(&ntty->ntty_rlock);
		sched_cancellable_sleep_on(&ntty->ntty_rwaitq);
		kmutex_lock(&ntty->ntty_rlock);
	}

	int read = 0;
	/* while read < len and buffer is not empty) */
	while (read < len &&
	       !buf_empty(ntty->ntty_rhead, ntty->ntty_ckdtail)) {
		buf[read] = ntty->ntty_inbuf[ntty->ntty_rhead];
		ntty->ntty_rhead = (ntty->ntty_rhead + 1) % TTY_BUF_SIZE;

		if (buf[read] == '\n' || buf[read] == '\r' ||
		    buf[read] == EOFC) {
			read++;
			break;
		}

		read++;
	}
	
	kmutex_unlock(&ntty->ntty_rlock);
	return (read);
}

/*
 * The tty subsystem calls this when the tty driver has received a
 * character. Now, the line discipline needs to store it in its read
 * buffer and move the read tail forward.
 *
 * Special cases to watch out for: backspaces (both ASCII characters
 * 0x08 and 0x7F should be treated as backspaces), newlines ('\r' or
 * '\n'), and full buffers.
 *
 * Return a null terminated string containing the characters which
 * need to be echoed to the screen. For a normal, printable character,
 * just the character to be echoed.
 */
const char *
n_tty_receive_char(tty_ldisc_t *ldisc, char c)
{
	n_tty_t *ntty = ldisc_to_ntty(ldisc);
	int delete = 0;

	kmutex_lock(&ntty->ntty_rlock);

	/* check if the line discipline's buffer is full */
	if (buf_full(ntty->ntty_rhead, ntty->ntty_rawtail)) {
		kmutex_unlock(&ntty->ntty_rlock);
		return NULL;
	}

	
 
	switch (c) {
	case 0x08:
		goto delete_char;
	case 0x7F:
		c = '\b';
		goto delete_char;
	case '\n':
		goto newline_char;
	case '\r':
		goto newline_char;
	case EOFC:
		goto newline_char;
	default:
		goto normal;
	}

newline_char:
	ntty->ntty_inbuf[ntty->ntty_rawtail % TTY_BUF_SIZE] = c;
	ntty->ntty_rawtail = (ntty->ntty_rawtail + 1) % TTY_BUF_SIZE;
	ntty->ntty_ckdtail = ntty->ntty_rawtail;

	ntty->ret[0] = '\n';
	ntty->ret[1] = '\r';
	ntty->ret[2] = '\0';

	/* wake up threads waiting on the buffer to read. */
	sched_broadcast_on(&ntty->ntty_rwaitq);

	goto ret;

delete_char:
	/* if raw buffer is empty, send an empty string terminated by '\0' */
	if (buf_empty(ntty->ntty_rawtail, ntty->ntty_ckdtail)) {
		ntty->ret[0] = '\0';
		goto ret;
	}

	ntty->ntty_rawtail = (ntty->ntty_rawtail - 1 + TTY_BUF_SIZE) %
		TTY_BUF_SIZE;

	ntty->ret[0] = c;
	ntty->ret[1] = ' ';
	ntty->ret[2] = c;
	ntty->ret[3] = '\0';

	goto ret;

normal:
	ntty->ntty_inbuf[ntty->ntty_rawtail % TTY_BUF_SIZE] = c;
	ntty->ntty_rawtail = (ntty->ntty_rawtail + 1) % TTY_BUF_SIZE;

	ntty->ret[0] = c;
	ntty->ret[1] = '\0';

ret:
	kmutex_unlock(&ntty->ntty_rlock);
	return ntty->ret;
}

/*
 * Process a character to be written to the screen.
 *
 * The only special case is '\r' and '\n'.
 */
const char *
n_tty_process_char(tty_ldisc_t *ldisc, char c)
{
	n_tty_t *ntty = ldisc_to_ntty(ldisc);
	kmutex_lock(&ntty->ntty_rlock);

	switch (c) {
	case '\n':
	case '\r':
		ntty->ret[0] = '\n';
		ntty->ret[1] = '\r';
		ntty->ret[2] = '\0';
		break;
	default:
		ntty->ret[0] = c;
		ntty->ret[1] = '\0';
	}

	kmutex_unlock(&ntty->ntty_rlock);
	return ntty->ret;
}
