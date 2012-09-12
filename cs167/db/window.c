#define _POSIX_C_SOURCE 200112L // mkstemp(3)
#define _XOPEN_SOURCE 600 // mkstemp(3)

#include "window.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

static size_t window_count = 0;

window_t *
window_constructor(const char *label)
{
	char ififo[] = "/tmp/twiniYXXXXXX";
	char ofifo[] = "/tmp/twinoYXXXXXX";
	static char itoa[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	    'X', 'Y', 'Z'};
	window_t *new_window = malloc(sizeof (window_t));
	if (new_window == NULL)
		return (NULL);

	/*
	 * mktemp only produces 26 unique names. By adding another letter, we
	 * allow 26 times as many.
	 */

	ififo[10] = ofifo[10] = itoa[(window_count++) / sizeof (itoa)];
	if (window_count >= sizeof (itoa) * sizeof (itoa)) {
		fprintf(stderr, "could not create FIFO name\n");
		free(new_window);
		return (NULL);
	}
	mkstemp(ififo);
	mkstemp(ofifo);

	/* Clean up after past failures. */
	unlink(ififo);
	/* Create the FIFO (named pipe) for receiving data from the window. */
	if (mkfifo(ififo, 0600) == -1) {
		perror("mkfifo");
		free(new_window);
		exit(EXIT_FAILURE);
	}

	unlink(ofifo);
	/* Create the FIFO for transmitting data to the window. */
	if (mkfifo(ofifo, 0600) == -1) {
		perror("mkfifo");
		free(new_window);
		return (NULL);
	}

	if ((new_window->in = open(ififo, O_RDWR)) == -1) {
		fprintf(stderr, "cannot open input FIFO\n");
		free(new_window);
		return (NULL);
	}

	if ((new_window->out = open(ofifo, O_RDWR)) == -1) {
		fprintf(stderr, "cannot open output FIFO\n");
		free(new_window);
		return (NULL);
	}

	new_window->pid = -1;
	switch (fork()) {
	case -1:
		fprintf(stderr, "could not create process for new window\n");
		free(new_window);
		return (NULL);

	case 0:
		/* Run xterm in the child process. */
		if (execlp("xterm", "xterm", "-T", label, "-n", label, "-ut",
		    "-geometry", "35x20", "-e", "./interface", ififo, ofifo,
		    (char *)NULL) == -1) {
			perror("exec(3) of xterm failed");
			free(new_window);
			exit(EXIT_FAILURE);
		}
		perror("exec(3) somehow failed");
		exit(EXIT_FAILURE);

	default:
		break;
	}
	read(new_window->in, &new_window->pid, 4);
	return (new_window);
}

void
window_destructor(window_t *win)
{
	kill(win->pid, SIGTERM);
	free(win);
}

void
serve(window_t *window, char *response, char *query)
{
	size_t rlen = strlen(response) + 1;
	int qlen;
	int ret;

	/*
	 * A null response must be sent, and only sent, on the first call to
	 * serve().
	 */
	if (rlen > 1) {
		struct iovec vec[2];

		/*
		 * Send the response in two parts: first the length, then the
		 * string.
		 */
		vec[0].iov_base = &rlen;
		vec[0].iov_len = sizeof (rlen);
		vec[1].iov_base = response;
		vec[1].iov_len = rlen;
		if (writev(window->out, vec, 2) == -1) {
			perror("writev");
			exit(EXIT_FAILURE);
		}
	}

	while (1) {
		/* Receive the length of the next query. */
		if ((ret = read(window->in, &qlen, sizeof (qlen))) == -1) {
			/* Handle interrupted system calls. */
			if (errno == EINTR)
				continue;
			perror("read");
			exit(EXIT_FAILURE);
		}
		break;
	}

	if (ret != sizeof (qlen)) {
		fprintf(stderr, "Wrong count returned from read of window: "
		    "%d\n", ret);
		exit(EXIT_FAILURE);
	}

	if (qlen < 0) {
		query[0] = EOF;
		return;
	}

	if (qlen > 256)
		qlen = 256;

	/* Receive the string portion of the query. */
	while (1) {
		if ((ret = read(window->in, query, (size_t)qlen)) == -1) {
			if (errno == EINTR)
				continue;
			perror("I/O error on read from window");
			exit(EXIT_FAILURE);
		}
		break;
	}

	if (ret != qlen) {
		perror("I/O error");
		exit(EXIT_FAILURE);
	}
}
