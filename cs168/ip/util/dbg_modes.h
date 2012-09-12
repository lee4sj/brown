/* dbg_modes.h - add debugging modes here
 * Keith Adams <kma>
 * Jason Lango <jal>
 * 
 * modified by Albert Huang <ashuang>
 */

#ifndef DBG_MODES_H
#define DBG_MODES_H

#include "colordefs.h"

#define DBG_MODE(x)     (1ULL << (x))

#define DBG_ALL         (~0ULL)
#define DBG_ERROR       DBG_MODE(0)     /* error conditions             */
#define DBG_LINK        DBG_MODE(1)
#define DBG_NET         DBG_MODE(2)
#define DBG_UTIL        DBG_MODE(3)
#define DBG_ROUTE       DBG_MODE(4)
#define DBG_DEFAULT     DBG_ERROR

/// There can be no white space in these strings

#define DBG_NAMETAB \
{ "all", DBG_ALL }, \
{ "error", DBG_ERROR }, \
{ "link", DBG_LINK }, \
{ "net", DBG_NET }, \
{ "util", DBG_UTIL }, \
{ "route", DBG_ROUTE }, \
{ NULL,         0 } 

#define DBG_COLORTAB \
{ DBG_ERROR, _BBLUE_ }, \
{ DBG_LINK, _RED_ }, \
{ DBG_NET, _BRED_ }, \
{ DBG_UTIL, _MAGENTA_ }, \
{ DBG_ROUTE, _MAGENTA_ }

#define DBG_ENV         "DBG_MODES"

#endif
