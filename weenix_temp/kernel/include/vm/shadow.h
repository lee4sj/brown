#pragma once

struct mmobj;

void shadow_init();
struct mmobj *shadow_create(void);
void shadow_destroy(struct mmobj *);

extern int shadow_count;

