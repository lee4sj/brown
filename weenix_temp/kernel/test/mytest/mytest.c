#include "test/mytest.h"

#include "globals.h"
#include "errno.h"

#include "api/exec.h"

#include "proc/kmutex.h"
#include "proc/kthread.h"
#include "proc/proc.h"
#include "proc/sched.h"

#include "drivers/blockdev.h"
#include "drivers/bytedev.h"
#include "drivers/dev.h"
#include "drivers/tty/virtterm.h"

#include "fs/vfs_syscall.h"
#include "fs/fcntl.h"
#include "fs/lseek.h"

#include "util/debug.h"
#include "util/string.h"

#include "mm/mm.h"
#include "mm/mman.h"

#include "vm/vmmap.h"

static void *
kthread_test_func(int arg1, void *arg2)
{
	/* static int count = 0; */

	/* dbg(DBG_TEST, "proc: %d\n", curproc->p_pid); */

	/* if (arg1 == 0) { */
	/* 	int a; */
	/* 	for (a = 0; a < 10; a++) */
	/* 		dbg(DBG_TEST, "%d\n", a); */
	/* } */

	/* else if (arg1 == 1) { */
	/* 	int a; */
	/* 	proc_t *p; */
	/* 	kthread_t *thr; */

	/* 	for (a = 0; a < 3; a++) { */
	/* 		p = proc_create("testing process"); */
	/* 		thr = kthread_create(p, kthread_test_func, */
	/* 				     2, NULL); */
	/* 		sched_make_runnable(thr); */
	/* 	} */

	/* 	kthread_exit(NULL); */
	/* } */

	/* else if (arg1 == 2) { */
	/* 	KASSERT(curproc->p_pproc->p_pid == 1); */
	/* } */

	/* else if (arg1 == 3) { */
	/* 	kmutex_t *mtx = arg2; */
	/* 	int a; */
	/* 	for (a = 0; a < 10; a++) { */
	/* 		kmutex_lock(mtx); */
	/* 		if (curproc->p_pid == 127) */
	/* 		    count = 0; */
	/* 		else */
	/* 		    count++; */
	/* 		dbg(DBG_TEST, "proc %d: %d\n", curproc->p_pid, count); */
	/* 		kmutex_unlock(mtx); */

	/* 		/\* so that other threads can start waiting *\/ */
	/* 		/\* for the mutex *\/ */
	/* 		sched_yld(); */
	/* 	} */
	/* } */

	
	/* else if (arg1 == 4) { */
	/* 	kmutex_t *mtx = arg2; */
	/* 	int a; */
	/* 	for (a = 0; a < 10; a++) { */
	/* 		kmutex_lock(mtx); */
	/* 		dbg(DBG_TEST, "proc %d: %d\n", curproc->p_pid, a); */
	/* 	} */
	/* } */

	return NULL;
}

void my_proctest(void)
{
	KASSERT(curproc->p_pid == 1);

	/* process and running tests */
	proc_t *test_curproc;
	kthread_t *test_curthr;
	pid_t test_pid;
	pid_t child;
	int status = 0;

	/* Test 1: basic */
	int a;
	for (a = 0; a < 10; a++) {
		test_curproc = proc_create("test 1 process");
		test_curthr = kthread_create(test_curproc, kthread_test_func,
					     0, NULL);
		test_pid = test_curproc->p_pid;
		sched_make_runnable(test_curthr);
		child = do_waitpid(test_curproc->p_pid, 0, &status);
		KASSERT(child == test_pid);
	}

	/* Test 2: multiple threads running and do_waitpid at the very end */
	for (a = 0; a < 10; a++) {
		test_curproc = proc_create("test 2 process");
		test_curthr = kthread_create(test_curproc, kthread_test_func,
					     0, NULL);
		test_pid = test_curproc->p_pid;
		sched_make_runnable(test_curthr);
	}

	for (a = 0; a < 10; a++) {
		child = do_waitpid(-1, 0, &status);
		KASSERT(child > 0);
	}

	/* used -10 instead of -ECHILD */
	KASSERT(do_waitpid(-1, 0, &status) == -10);
	dbg(DBG_TEST, "test 2 succeeded\n");

	/* Test 3: a parent processes exits before the child processes */
	test_curproc = proc_create("test 3 process");
	test_curthr = kthread_create(test_curproc, kthread_test_func,
				     1, NULL);
	test_pid = test_curproc->p_pid;
	sched_make_runnable(test_curthr);

	while (do_waitpid(-1, 0, &status) != -10)
		;

	/* Test 4: load test */
	for (a = 0; a < 100; a++) {
		test_curproc = proc_create("test 1 process");
		test_curthr = kthread_create(test_curproc, kthread_test_func,
					     0, NULL);
		test_pid = test_curproc->p_pid;
		sched_make_runnable(test_curthr);
	}

	while(do_waitpid(-1, 0, &status) != -10)
		;

	/* Test 5: mutex test */
	kmutex_t km;
	kmutex_init(&km);
	for (a = 0; a < 5; a++) {
		test_curproc = proc_create("test 1 process");
		test_curthr = kthread_create(test_curproc, kthread_test_func,
					     3, (void *) &km);
		test_pid = test_curproc->p_pid;
		sched_make_runnable(test_curthr);
	}
	
	while(do_waitpid(-1, 0, &status) != -10)
		;


	/* when this is ran, the kernel should hang due to race condition! */
	/* for (a = 0; a < 5; a++) { */
	/* 	test_curproc = proc_create("test 1 process"); */
	/* 	test_curthr = kthread_create(test_curproc, kthread_test_func, */
	/* 				     4, (void *) &km); */
	/* 	test_pid = test_curproc->p_pid; */
	/* 	sched_make_runnable(test_curthr); */
	/* } */
	
	/* while(do_waitpid(-1, 0, &status) != -10) */
	/* 	; */
}

void my_drivertest(void)
{
	dbg(DBG_TEST, "device driver tests\n");

	char *out_buf = (char *)page_alloc();
	char *in_buf = (char *)page_alloc();
	uint32_t i;

	memset(out_buf, 'r', PAGE_SIZE);
	memset(in_buf, 0, PAGE_SIZE);

	blockdev_t *bd = blockdev_lookup(MKDEVID(DISK_MAJOR, 0));
	KASSERT(bd);

	int err = bd->bd_ops->write_block(bd, out_buf, 10, 1);
	KASSERT(err == 0);

	memset(out_buf, 'k', PAGE_SIZE);
	err = bd->bd_ops->write_block(bd, out_buf, 100, 1);
	KASSERT(err == 0);

	err = bd->bd_ops->read_block(bd, in_buf, 10, 1);
	KASSERT(err == 0);

	for (i = 0; i < PAGE_SIZE; i++) {
		KASSERT(in_buf[i] == 'r');
	}

	memset(in_buf, 0, PAGE_SIZE);
	err = bd->bd_ops->read_block(bd, in_buf, 100, 1);
	KASSERT(err == 0);

	for (i = 0; i < PAGE_SIZE; i++) {
		KASSERT(in_buf[i] == 'k');
	}

	page_free(out_buf);
	page_free(in_buf);
	dbg(DBG_TEST, "blockdev passed!!\n");

	bytedev_t *cd = bytedev_lookup(MKDEVID(2, 0));
	KASSERT(cd);

	char *buf = "Welcome to Weenix!\n";
	err = cd->cd_ops->write(cd, 0, buf, strlen(buf));
	KASSERT(err == (int)strlen(buf));
	dbg(DBG_TEST, "bytedev passed!!\n");
}

void my_vfstest(void)
{
}

void my_s5fstest(void)
{
	int ret;
	int fd;

	/* mkdir */
	ret = do_mkdir("/myfstest");
	KASSERT(ret == 0 || ret == -EEXIST);

	/* */
	fd = do_open("/myfstest/file1", O_CREAT | O_RDWR);
	KASSERT(fd >= 0 && fd < NFILES);

	char *buf = "writing to a fil\n";
	char in_buf[18];
	in_buf[strlen(buf) + 1] = '\0';
	int len = strlen(buf);
	ret = do_write(fd, buf, len);
	KASSERT(ret == 17);
	dbg(DBG_TEST, "wrote \"%s\"\n", buf);

	KASSERT(!do_lseek(fd, 0, SEEK_SET));
	memset(in_buf, 0, 18);
	ret = do_read(fd, in_buf, len);
	KASSERT(ret == 17);
	dbg(DBG_TEST, "read \"%s\"\n", in_buf);

	ret = do_close(fd);
	KASSERT(ret == 0);

	struct stat s;
	ret = do_stat("/myfstest/file1", &s);
	dbg(DBG_TEST, "%s is %d, ino=%d, size=%d", buf, s.st_mode,
	    s.st_ino, s.st_size);
}

static void *
vm_test_func(int arg1, void *arg2) {

	do_open("/dev/tty0", O_RDONLY);
	do_open("/dev/tty0", O_WRONLY);
	do_open("/dev/tty0", O_WRONLY);

	/* kernel execve */
	char const* argv[] = { "hello", NULL };
	char const* envp[] = { "world", NULL };

	kernel_execve((char *)arg2, (char * const*)argv, (char * const*)envp);

	return NULL;
}

void my_vmtest(void) {
	vmarea_t *vma;
	vmmap_t *map = vmmap_create();
	char buf[PAGE_SIZE];
	char inbuf[PAGE_SIZE];
	int ret;

	/* vmmap functions */
	KASSERT(1 == vmmap_is_range_empty(map, ADDR_TO_PN(USER_MEM_LOW), 10));
	KASSERT((vma = vmmap_lookup(map, ADDR_TO_PN(USER_MEM_LOW))) == NULL);

	ret = vmmap_map(map, NULL, ADDR_TO_PN(USER_MEM_LOW), 2, PROT_READ,
			MAP_SHARED, 0, VMMAP_DIR_LOHI, &vma);
	KASSERT(ret == 0);

	KASSERT((vma = vmmap_lookup(map, ADDR_TO_PN(USER_MEM_LOW))) != NULL);
	KASSERT(0 == vmmap_is_range_empty(map, ADDR_TO_PN(USER_MEM_LOW), 10));

	memset(buf, 1, PAGE_SIZE);
	vmmap_write(map, (void *)USER_MEM_LOW, buf, PAGE_SIZE);

	memset(inbuf, 0, PAGE_SIZE);
	vmmap_read(map, (void *)USER_MEM_LOW, inbuf, PAGE_SIZE);

	for (ret = 0; ret < 100; ret++) {
		KASSERT(inbuf[ret] == 1);
		KASSERT(inbuf[ret] == buf[ret]);
	}

	ret = vmmap_remove(map, ADDR_TO_PN(USER_MEM_LOW), 2);
	KASSERT(ret == 0);
	KASSERT((vma = vmmap_lookup(map, ADDR_TO_PN(USER_MEM_LOW))) == NULL);

	vmmap_destroy(map);

	char *bin = "/usr/bin/hello";
	proc_t *test_curproc = proc_create("test 3 process");
	kthread_t *test_curthr = kthread_create(test_curproc, vm_test_func,
						1, bin);
	sched_make_runnable(test_curthr);

	int status;
	while (do_waitpid(-1, 0, &status) != -10)
		;
	
	bin = "/usr/bin/memtest";
	test_curproc = proc_create("test 3 process");
	test_curthr = kthread_create(test_curproc, vm_test_func, 1, bin);
	sched_make_runnable(test_curthr);

	while (do_waitpid(-1, 0, &status) != -10)
		;

	
	bin = "/usr/bin/vfstest";
	test_curproc = proc_create("test 3 process");
	test_curthr = kthread_create(test_curproc, vm_test_func, 1, bin);
	sched_make_runnable(test_curthr);

	while (do_waitpid(-1, 0, &status) != -10)
		;
}
