#ifndef HASH_H
#define HASH_H

typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */
typedef  unsigned       char ub1;   /* unsigned 1-byte quantities */

ub4 hash( register ub1 *k, register ub4 length, register ub4 initval);

#endif
