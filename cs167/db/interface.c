#define _POSIX_C_SOURCE 200112L // sigemptyset(3)

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

static void terminate(int);

/*
 * This program runs in an xterm window and interacts with the capital cities
 * database program.
 */
int
main(int argc, const char *argv[])
{
	char rbuf[256];
	char qbuf[256];
	int rlen, qlen;
	int ret;
	int ifd, ofd;
	int pid = getpid();
	struct sigaction sa;
#ifdef AUTOMATED_TEST
	FILE *infile;
#endif

	sa.sa_handler = terminate;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGTERM, &sa, NULL);
#ifdef AUTOMATED_TEST
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
#endif

	if (argc != 3) {
		fprintf(stderr, "Usage: interface infile outfile\n");
		exit(EXIT_FAILURE);
	}

	if ((ifd = open(argv[2], O_RDONLY)) == -1) {
		fprintf(stderr, "%s", argv[2]);
		perror("open input FIFO");
		sleep(10);
		exit(EXIT_FAILURE);
	}

	if ((ofd = open(argv[1], O_WRONLY)) == -1) {
		fprintf(stderr, "%s", argv[1]);
		perror("open output FIFO");
		sleep(10);
		exit(EXIT_FAILURE);
	}

	write(ofd, &pid, 4);

	/*
	 * Now that we've opened the FIFOs, make them go away when the process
	 * terminates.
	 */
	unlink(argv[2]);
	unlink(argv[1]);

	rbuf[0] = '\0';

#ifdef AUTOMATED_TEST
	infile = fopen("./tests/WindowScript", "r");
	if (infile == NULL)
		terminate(SIGINT);
#endif

	for (;;) {
		/* Print the previous response (if any) and the prompt. */
		printf("%s\n >> ", rbuf);
#ifndef AUTOMATED_TEST
		fflush(stdout);
#endif

#ifdef AUTOMATED_TEST
		if (fgets(qbuf, sizeof (qbuf), infile) == NULL) {
#else
		if (fgets(qbuf, sizeof (qbuf), stdin) == NULL) {
#endif
			/* EOF means to terminate. */
			qlen = -1;
			if (write(ofd, &qlen, sizeof (qlen)) == -1) {
				perror("write");
				sleep(10);
				exit(EXIT_FAILURE);
			}
			printf("\nGoodbye\n");
			/*
			 * Rather than exit(EXIT_SUCCESS), force the window to
			 * be terminated by the control program.
			 */
			continue;
		} else {
			struct iovec vec[2];

#ifdef AUTOMATED_TEST
			fputs(qbuf, stdout);
#endif

			/*
			 * We have a query. Send it across in two parts: the
			 * length and the string.
			 */
			qlen = (int)strlen(qbuf);
			/* Strip off the newline. */
			qbuf[qlen-1] = '\0';
			vec[0].iov_len = sizeof (qlen);
			vec[0].iov_base = &qlen;
			vec[1].iov_len = (size_t)qlen;
			vec[1].iov_base = qbuf;

			if (writev(ofd, vec, 2) == -1) {
				perror("writev");
				sleep(10);
				exit(EXIT_FAILURE);
			}
		}

		/* Get the length of the response string. */
		if ((ret = read(ifd, &rlen, sizeof (rlen))) == -1) {
			perror("read");
			sleep(10);
			exit(EXIT_FAILURE);
		}

		if (ret != sizeof (rlen)) {
			fprintf(stderr, "read(2) of response length fetched %d "
			    "bytes\n", ret);
			sleep(10);
			exit(EXIT_FAILURE);
		}

		/* Get the response string. */
		if ((ret = read(ifd, rbuf, (size_t)rlen)) == -1) {
			perror("read");
			sleep(10);
			exit(EXIT_FAILURE);
		}

		if (ret != rlen) {
			fprintf(stderr, "read(2) of response string fetched %d "
			    "bytes.\n", ret);
			sleep(10);
			exit(EXIT_FAILURE);
		}
	}
}

static void
terminate(int sig)
{
	fprintf(stderr, "\n");
	psignal(sig, "Caught signal");
	fprintf(stderr, "Goodbye.\n");
	exit(EXIT_SUCCESS);
}
