#pragma once

#define _POSIX_C_SOURCE 200112L

#include <pthread.h>
#include <stdlib.h>

typedef struct Node {
	char *name;
	char *value;
	struct Node *lchild;
	struct Node *rchild;

	/* locks for find-grind locking */
	pthread_rwlock_t rwlock;
} Node_t;

enum lock_type {
	RDLOCK = 0,
	WRLOCK,
};

extern Node_t head;

void interpret_command(const char *, char *, size_t);
void cleanup_db();
