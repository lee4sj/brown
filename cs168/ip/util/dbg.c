/**
 *  @file originally dbg.c, now dbg.C
 * @author kma, modified by ashuang to work with CC and to
 *         remove some unnecessary details
 *  @desc mcc's debugging spice
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "dbg_modes.h"
#include "dbg.h"

unsigned long long dbg_modes = 0;
short dbg_initiated = 0;

static dbg_mode_color_t dbg_colortab[] = {
  DBG_COLORTAB
};

static dbg_mode_t dbg_nametab[] = {
  DBG_NAMETAB
};

const char* DCOLOR(unsigned long long d_mode) {
  for (dbg_mode_color_t* mode = dbg_colortab; mode->d_mode != 0; mode++) {
    if (mode->d_mode & d_mode)
      return mode->color;
  }

  return _BWHITE_;
}

/**
 * searches for <code>name</code> in the list of known
 * debugging modes specified in dbg_modes.H and, if it
 * finds <code>name</code>, adds the corresponding
 * debugging mode to a list
 */
void dbg_add_mode(const char *name) {
  int cancel;
  dbg_mode_t *mode;

  if (*name == '-') {
    cancel = 1;
    name++;
  } else {
    cancel = 0;
  }
    
  for (mode = dbg_nametab; mode->d_name != NULL; mode++)
    if (strcmp(name, mode->d_name) == 0)
      break;
  if (mode->d_name == NULL) {
    fprintf(stderr, "Warning: Unknown debug option: " "\"%s\"\n", name);
    return;
  }
    
  if (cancel) {
    dbg_modes &= ~mode->d_mode;
  } else {
    dbg_modes = dbg_modes | mode->d_mode;    
  }
}

/**
 * Cycles through each comma-delimited debugging option and
 * adds it to the debugging modes by calling dbg_add_mode
 */
void dbg_add_modes(const char *modes) {
  char env[256];
  char *name;
    
  strncpy(env, modes, sizeof(env));
  for (name = strtok(env,","); name; name = strtok(NULL, ","))
    dbg_add_mode(name);    
}

/**
 * Reads the environment variable specified in dbg_modes.H and
 * calls dbg_add_modes()
 */
void dbg_init() {
  const char *dbg_env;
  dbg_initiated = 1;
  
  dbg_modes = DBG_DEFAULT;
  
  dbg_env = getenv(DBG_ENV);
  if (!dbg_env)
    return;
  dbg_add_modes(dbg_env);
}
