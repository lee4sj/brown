#pragma once

typedef struct window {
	int in;
	int out;
	int pid;
} window_t;

window_t *window_constructor(const char *);
void window_destructor(window_t *);
void serve(window_t *, char *, char *);
