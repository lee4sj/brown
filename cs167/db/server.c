#define _POSIX_C_SOURCE 200112L // nanosleep(2)

#include "db.h"
#include "window.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t wait_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t watchdog_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t watchdog_cond = PTHREAD_COND_INITIALIZER;
int stop = 0;

/*
 * TODO (Part 2): This function should be implemented and called by client
 * threads to wait until progress is permitted. Ensure that the mutex is not
 * left locked if the thread is cancelled while waiting on a condition
 * variable.
 */
static void
ClientControl_wait(void)
{
	if (!stop)
		return;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	pthread_mutex_lock(&wait_lock);
	while (stop) {
		pthread_cond_wait(&wait_cond, &wait_lock);
	}
	pthread_mutex_unlock(&wait_lock);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_testcancel();
}

static void
WatchDog_wait(void)
{
	if (!stop)
		return;

	pthread_mutex_lock(&watchdog_lock);

	while (stop) {
		pthread_cond_wait(&watchdog_cond, &watchdog_lock);
	}

	pthread_mutex_unlock(&watchdog_lock);
}

/*
 * TODO (Part 2): This function should be implemented and called by the main
 * thread to stop the client threads.
 */
static void
ClientControl_stop(void)
{
	pthread_mutex_lock(&wait_lock);
	stop = 1;
	pthread_mutex_unlock(&wait_lock);
}

/*
 * TODO (Part 2): This function should be implemented and called by the main
 * thread to resume client threads.
 */
static void
ClientControl_release(void)
{
	pthread_mutex_lock(&wait_lock);
	stop = 0;
	pthread_cond_broadcast(&wait_cond);
	pthread_mutex_unlock(&wait_lock);
}

/*
 * the encapsulation of a client thread, i.e., the thread that handles commands
 * from clients
 */
typedef struct Client {
#ifdef _PTHREAD_H
	pthread_t thread;
#endif
	window_t *win;
	/*
	 * The thread list (Part 3):
	 * Client threads put themselves in the list and take themselves out,
	 * thus guaranteeing that a client's being in the list means that its
	 * thread is active and thus may be safely cancelled.
	 *
	 * See also ThreadListHead and ThreadListMutex, below.
	 */
	struct Client *prev;
	struct Client *next;

	pthread_barrier_t barrier;
} Client_t;

Client_t *ThreadListHead;
#ifdef _PTHREAD_H
pthread_mutex_t ThreadListMutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static void *RunClient(void *);
static int handle_command(const char *, char *, size_t);
static void ThreadCleanup(void *);
static void DeleteAll(void);

/*
 * TODO (Part 1): Modify this function such that a new thread is created and
 * detached that runs RunClient.
 *
 * TODO (Part 3): Note that window_constructor must be executed atomically:
 * cancellation must be disabled within it.
 *
 * TODO (Part 3): See the comment in RunClient about a barrier.
 */
static void
Client_constructor(int ID)
{
	int olds;
	if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &olds)) {
		printf("failed to create window: failed to set cancelstate\n");
		return;
	}

	char title[16];
	Client_t *new_Client = malloc(sizeof (Client_t));
	if (new_Client == NULL)
		return;
	sprintf(title, "Client %d", ID);

	/*
	 * This constructor creates a window and sets up a communication
	 * channel with it.
	 */
	new_Client->win = window_constructor(title);

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		printf("failed to create window: failed to set cancelstate\n");
		return;
	}

	pthread_barrier_init(&new_Client->barrier, NULL, 3);

	pthread_create(&new_Client->thread, 0, RunClient, new_Client);
	pthread_detach(new_Client->thread);

	pthread_barrier_wait(&new_Client->barrier);
}

static void
Client_destructor(Client_t *client)
{
	pthread_barrier_destroy(&client->barrier);

	/* The destructor removes the window. */
	window_destructor(client->win);

	free(client);
}
 
/*
 * TODO (Part 3): This function cancels every thread in the list (see
 * ThreadListHead). Each thread must remove itself from the thread list.
 */
static void
DeleteAll()
{
	ClientControl_release();
	pthread_mutex_lock(&ThreadListMutex);

	if (ThreadListHead == NULL) {
		pthread_mutex_unlock(&ThreadListMutex);
		return;
	}

	Client_t *cur;
	for (cur = ThreadListHead->next; cur != ThreadListHead;
	     cur = cur->next) {
		pthread_cancel(cur->thread);
	}
	pthread_cancel(ThreadListHead->thread);

	pthread_mutex_unlock(&ThreadListMutex);
}

/*
 * This is the cleanup routine for client threads, called on cancel and exit.
 *
 * TODO (Part 1): This function should free the client instance by calling
 * Client_destructor.
 *
 * TODO (Part 3): This function pulls the client thread from the thread list
 * (see ThreadListHead) and deletes it. Thus the client thread must be in the
 * thread list before the thread acts on a cancel.
 */
static void
ThreadCleanup(void *arg)
{
	Client_t *client = arg;

	/* take self off from the list */
	pthread_mutex_lock(&ThreadListMutex);
	Client_t *prev = client->prev;
	Client_t *next = client->next;

	if (ThreadListHead == client) {
		if (next == ThreadListHead) {
			ThreadListHead = NULL;
			goto no_reordering;
		}
		else
			ThreadListHead = next;
	}

	prev->next = next;
	next->prev = prev;

no_reordering:
	pthread_mutex_unlock(&ThreadListMutex);

	Client_destructor(client);
	return;
}

/*
 * This struct helps keep track of how long it's been since a command has been
 * received by a client thread. The client thread is cancelled if it's been too
 * long. For each client thread an associated "watchdog thread" is created
 * which loops, sleeping wait_secs seconds since the last time the client
 * thread received input. At each wakeup, it checks to see if the client thread
 * has received input since the watchdog went to sleep. If not, the watchdog
 * cancels the client thread, then terminates itself. Otherwise it loops again,
 * sleeping wait_secs seconds since the last time the client thread received
 * input ...
 */
typedef struct Timeout {
	/* time when client thread "times out" */
	struct timeval timeout;

	/* set when client thread receives input */
	/* reset each time watchdog thread wakes up */
	int active;

	Client_t *client;

#ifdef _PTHREAD_H
	pthread_mutex_t timeout_lock;
	pthread_t WatchDogThread;
#endif
} Timeout_t;

static time_t Timeout_wait_secs = 10; /* timeout interval */

static void Timeout_reset(Timeout_t *);
static void *Timeout_WatchDog(void *); /* watchdog thread */

/*
 * TODO (Part 3): Allocate a new timeout instance (described by a Timeout_t)
 * and initialize its members. Set the initial timeout to Timeout_wait_secs
 * from the current time; see gettimeofday(2) and friends. Finally, create a
 * watchdog thread for this timeout.
 */
static Timeout_t *
Timeout_constructor(Client_t *a_client)
{
	Timeout_t *timeout = (Timeout_t *)malloc(sizeof (Timeout_t));
	gettimeofday(&timeout->timeout, NULL);
	timeout->timeout.tv_sec += Timeout_wait_secs;
	timeout->client = a_client;
	pthread_mutex_init(&timeout->timeout_lock, NULL);
	pthread_create(&timeout->WatchDogThread, NULL,
		       Timeout_WatchDog, (void *)timeout);
	return timeout;
}

/*
 * TODO (Part 3): Destroy this timeout instance. Be sure to cancel the watchdog
 * thread and join with it.
 */
static void
Timeout_destructor(void *arg)
{
	Timeout_t *timeout = (Timeout_t *)arg;

	pthread_mutex_destroy(&timeout->timeout_lock);
	pthread_cancel(timeout->WatchDogThread);
	pthread_join(timeout->WatchDogThread, NULL);
	free(timeout);
}

/*
 * This function contains code executed by the client.
 *
 * TODO (Part 1): Make sure that the client instance is properly freed when the
 * thread terminates by calling ThreadCleanup. We strongly suggest you use
 * pthread_cleanup_push(3) and pthread_cleanup_pop(3) for this purpose.
 *
 * TODO (Part 3): Make sure that the timeout instance (and its watchdog thread)
 * is properly freed when the thread terminates by calling Timeout_destructor.
 * We strongly suggest you use pthread_cleanup_push(3) and
 * pthread_cleanup_pop(3) for this purpose.
 */
static void *
RunClient(void *arg)
{
	pthread_cleanup_push(ThreadCleanup, arg);

	Client_t *client = arg;

	/*
	 * Establish our watchdog thread to deal with timeouts. Timeout's
	 * destructor cancels the watchdog thread and doesn't return until
	 * after joining with it.
	 */
	Timeout_t *timeout = Timeout_constructor(client);
	pthread_cleanup_push(Timeout_destructor, timeout);
	
	/* add the client to the list */
	pthread_mutex_lock(&ThreadListMutex);
	if (ThreadListHead) {
		Client_t *next = ThreadListHead->next;
		ThreadListHead->next = client;
		next->prev = client;
		client->prev = ThreadListHead;
		client->next = next;
	} else {
		ThreadListHead = client;
		ThreadListHead->next = ThreadListHead;
		ThreadListHead->prev = ThreadListHead;
	}
	pthread_mutex_unlock(&ThreadListMutex);


	/* The client thread is now fully started. */

	/*
	 * TODO (Part 3): Other threads waiting for this thread need to be
	 * notified somehow. (Hint: Consider the watchdog thread, which
	 * shouldn't worry about timeouts till now, and the main thread, which
	 * must not call DeleteAll until this thread is fully started. A
	 * barrier might be useful.)
	 */
	pthread_barrier_wait(&client->barrier);

	/*
	 * main loop of the client: fetch commands from window, interpret and
	 * handle them, return results to window
	 */
	{
		char command[256];
		/* response must be NULL for the first call to serve */
		char response[256] = {0};

		serve(client->win, response, command);

		/* we've received input: reset timer */
		Timeout_reset(timeout);

		while (handle_command(command, response, sizeof (response))) {
			/* check wait */
			ClientControl_wait();
			pthread_testcancel();

			/* we've processed a command: reset timer */
			Timeout_reset(timeout);

			serve(client->win, response, command);

			/* we've received input: reset timer */
			Timeout_reset(timeout);
		}
	}

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_exit((void *)1);
}

static int
handle_command(const char *command, char *response, size_t len)
{
	if (command[0] == EOF) {
		strncpy(response, "all done", len - 1);
		return (0);
	} else {
		interpret_command(command, response, len);
	}
	return (1);
}

/*
 * TODO (Part 3): This function is called by client threads each time they
 * receive a command. It stores the current time + Timeout_wait_secs in
 * timeout; see gettimeofday(2)_and friends. This function also indicates that
 * new input has been received. If the timeout expires and no further input has
 * been received, then the watchdog thread cancels us. Note: be sure to lock
 * the appropriate mutex!
 */
void
Timeout_reset(Timeout_t *timeout)
{
	pthread_mutex_lock(&timeout->timeout_lock);
	gettimeofday(&timeout->timeout, NULL);
	timeout->timeout.tv_sec += Timeout_wait_secs;
	timeout->active = 1;
	pthread_mutex_unlock(&timeout->timeout_lock);
}

/*
 * There is one watchdog thread per client thread. It cancels the client if it
 * hasn't received a command within the specified time period (stored in
 * wait_secs).
 */
void *
Timeout_WatchDog(void *arg)
{
	Timeout_t *timeout = arg;
	struct timespec to;

	/*
	 * TODO (Part 3): Make sure that the timeout clock doesn't start till
	 * the client is fully started, and thus that the thread won't time out
	 * until after it has fully started. A barrier may be useful here.
	 */
	pthread_barrier_wait(&timeout->client->barrier);

	to.tv_sec = Timeout_wait_secs;
	to.tv_nsec = 0;

	while (1) {
		struct timeval now;

		/*
		 * sleep for the current timeout interval
		 * (since thread was started or last input)
		 */
		if (nanosleep(&to, NULL) == -1) {
			perror("nanosleep");
			exit(EXIT_FAILURE);
		}

		if (timeout->active == 0) {
			/*
			 * TODO (Part 3): Our client thread hasn't received
			 * input in a while, so cancel it (and self-destruct).
			 */

			pthread_cancel(timeout->client->thread);
			pthread_exit((void *)1);
		} else {
			timeout->active = 0;
		}

		/*
		 * Set the next timeout to be the requested number of seconds
		 * since the last client input; convert an absolute timeout to
		 * an interval.
		 */
		ClientControl_wait();

#ifdef _PTHREAD_H
		pthread_mutex_lock(&timeout->timeout_lock);
#endif
		gettimeofday(&now, NULL);
		to.tv_sec = timeout->timeout.tv_sec - now.tv_sec;
		to.tv_nsec = 1000 * (timeout->timeout.tv_usec - now.tv_usec);
		if (to.tv_nsec < 0) {
			/* "borrow" from seconds */
			to.tv_sec--;
			to.tv_nsec += 1000000000;
		}
#ifdef _PTHREAD_H
		pthread_mutex_unlock(&timeout->timeout_lock);
#endif
	}

	return (NULL);
}

typedef struct SigHandler {
	sigset_t set;
#ifdef _PTHREAD_H
	pthread_t SigThread;
#endif
} SigHandler_t;

static void *SigMon(void *);

/*
 * TODO (Part 3): Allocate a new instance of a signal handler (SigHandler_t),
 * mask off Ctrl-C (SIGINT, see sigsetops(3)), and create the signal-handling
 * thread (SigMon).
 */
static SigHandler_t *
SigHandler_constructor()
{
	SigHandler_t *sighandler =
		(SigHandler_t *) malloc(sizeof (SigHandler_t));
	sigemptyset(&sighandler->set);
	sigaddset(&sighandler->set, SIGINT);

	pthread_sigmask(SIG_BLOCK, &sighandler->set, NULL);
	pthread_create(&sighandler->SigThread, NULL, SigMon, &sighandler->set);
	return sighandler;
}

/*
 * TODO (Part 3): Destroy an instance of a signal handler. Be sure to cancel
 * the signal-handling thread and join with it.
 */
static void
SigHandler_destructor(SigHandler_t *sighandler)
{
	pthread_cancel(sighandler->SigThread);
	pthread_join(sighandler->SigThread, NULL);
	free(sighandler);
}

/*
 * This function contains the code for the signal-handling thread.
 *
 * TODO (Part 3): When a ctrl-C occurs, delete all current client
 * threads with DeleteAll.
 */
static void *
SigMon(void *arg) {
	sigset_t *setp = arg;
	int sig;

	while (1) {
		if (sigwait(setp, &sig))
			printf("error in receiving signal!\n");
		else
			DeleteAll();
	}

	return (NULL);
}

/*
 * TODO (Part 1): Modify this function so that no window is created
 * automatically. Instead, every time you type enter (in the window in which
 * you're running server), a new window is created along with a new thread to
 * handle it.
 */
#define RD_BUF_SIZE 3
int
main(int argc, char *argv[])
{
	SigHandler_t *sig_handler;
	
	if (argc == 2) {
		Timeout_wait_secs = atoi(argv[1]);
	} else if (argc > 2) {
		fprintf(stderr, "Usage: server [timeout_secs]\n");
		exit(EXIT_FAILURE);
	}

	sig_handler = SigHandler_constructor();

	int rd = 0;
	char buffer[RD_BUF_SIZE + 1];
	memset(buffer, 0, RD_BUF_SIZE + 1);

	while ((rd = read(STDIN_FILENO, buffer, RD_BUF_SIZE)) != -1) {
		if (rd == 0)
			break;

		else if (strchr(buffer, 's'))
			ClientControl_stop();

		else if (strchr(buffer, 'g'))
			ClientControl_release();

		else if (strchr(buffer, '\n'))
			Client_constructor(0);
	}

	SigHandler_destructor(sig_handler);

	DeleteAll();

	cleanup_db();
	pthread_exit((void *)1);
}
