/*!
   @file   CS123Common.h
   @author Travis Fischer (fisch0920@gmail.com)
   @author Ben Herila (ben@herila.net)
   @date   Fall 2010

   @brief  Contains data structures and macros commonly used in CS123.
*/
#pragma once
#ifndef __CS123COMMON_H__
#define __CS123COMMON_H__

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <QMessageBox>

// from http://en.wikipedia.org/wiki/Assertion_(computing)
#define COMPILE_TIME_ASSERT(pred) switch(0){case 0:case pred:;}

typedef double REAL;

//! A structure for a color.  Each channel is 8 bits [0-255].
struct BGRA
{
    BGRA() : b(0), g(0), r(0), a(0) {}
    BGRA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) : b(blue), g(green), r(red), a(alpha) {}

    // C++ TIP:
    // A union struct. Essentially, this makes b <==> channels[0],
    // g <==> channels[1], etc. In other words: b, g, r, and a are
    // stored at the same memory location as channels[4]. Note that
    // sizeof(b)+sizeof(g)+sizeof(r)+sizeof(a) = sizeof(channels)
    // so the memory overlaps **exactly**.
    //
    // You might want to take advantage of union structs in later
    // assignments, although we don't require or expect you to do so.
    //
    union {
        struct { unsigned char b, g, r, a; };
        unsigned char channels[4];
    };

    // @TODO: [OPTIONAL] You can implement some operators here for color arithmetic.

};

//! A structure to store a rectangle on the screen.
struct CS123Rectangle {
   int minX;
   int maxX;
   int minY;
   int maxY;
};


// ----------------------
// Deallocation Utilities
// ----------------------

#define safeFree(p)  \
   do {              \
      if ((p)) {     \
         free((p));  \
         (p) = NULL; \
      }              \
   } while(0)

#define safeDelete(p)   \
   do {                 \
      if ((p)) {        \
         delete ((p));  \
         (p) = NULL;    \
      }                 \
   } while(0)

#define safeDeleteArray(p) \
   do {                    \
      if ((p)) {           \
         delete[] ((p));   \
         (p) = NULL;       \
      }                    \
   } while(0)


// ---------------------
// Common math utilities
// ---------------------

#ifdef MIN
   #undef MIN
#endif

#ifdef MAX
   #undef MAX
#endif

#ifdef MIN3
   #undef MIN3
#endif

#ifdef MAX3
   #undef MAX3
#endif

#ifdef ABS
   #undef ABS
#endif

#define MIN(x, y) ({          \
   typeof (x) x_ = (x);       \
   typeof (y) y_ = (y);       \
   ((x_ < y_) ? x_ : y_);     \
})

#define MAX(x, y) ({          \
   typeof (x) _x_ = (x);      \
   typeof (y) _y_ = (y);      \
   ((_x_ > _y_) ? _x_ : _y_); \
})

#define MIN3(x, y, z) ({      \
   typeof (x) x__ = (x);      \
   typeof (y) y__ = (y);      \
   typeof (z) z__ = (z);      \
   ((x__ < y__) ? (x__ < z__ ? x__ : z__) : (y__ < z__ ? y__ : z__)); \
})

#define MAX3(x, y, z) ({      \
   typeof (x) __x = (x);      \
   typeof (y) __y = (y);      \
   typeof (z) __z = (z);      \
   ((__x > __y) ? (__x > __z ? __x: __z) : (__y > __z ? __y : __z)); \
})

#define ABS(x) ({                   \
   register typeof(x) ___tx = (x);  \
   (___tx >= 0 ? ___tx : -___tx);   \
})



#ifndef M_PI
#define M_PI 3.14159265f
#endif

#define _EPSILON_ 1e-8
#define EPSILON _EPSILON_

#define EQ(a, b) (fabs((a) - (b)) < _EPSILON_)
#define NEQ(a, b) (fabs((a) - (b)) > _EPSILON_)


#define NYI(f) { \
       char ss[999]; \
       (sprintf(ss, "Not yet implemented: %s, file %s, line %d\n", \
                              f, __FILE__, __LINE__)); \
       QMessageBox mb; \
       mb.setText(QString(ss)); \
       mb.setIcon(QMessageBox::Critical); \
       mb.exec(); \
       exit(0xf); \
   }


#endif

