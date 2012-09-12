/*
 *  @file originally dbg.h, now dbg.H
 * @author kma, modified by ashuang to work with CC and to
 *         remove some unnecessary details
 *  @desc mcc's debugging spice
 */

#ifndef DBG_H
#define DBG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "dbg_modes.h"

void dbg_add_mode(const char *name);
void dbg_add_modes(const char *modes);
void dbg_init();

extern unsigned long long dbg_modes;
extern short dbg_initiated;

typedef struct dbg_mode {
  const char *d_name;
  unsigned long long d_mode;
} dbg_mode_t;

typedef struct dbg_mode_color {
  unsigned long long d_mode;
  const char *color;
} dbg_mode_color_t;


const char* DCOLOR(unsigned long long d_mode);

// NO_DBG will only be defined when CC (or other compiler) is run with the -DNO_DBG flag
#ifndef NO_DBG

#define dbg(mode, args...) do { if(!dbg_initiated) dbg_init();  \
                            if (dbg_modes & (mode)) { \
			      fprintf(stderr, "%s", DCOLOR(mode));       \
			      fprintf(stderr, ## args);                  \
			      fprintf(stderr, "%s", _NORMAL_);           \
			    } \
                           } while(0)
#define dbg_active(mode) (dbg_modes & (mode))

#else

#define dbg(mode, arg) 
#define dbg_active(mode) false

#endif

#ifdef __cplusplus
}
#endif
#endif
