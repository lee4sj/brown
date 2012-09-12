#!/bin/bash -e

pushd ../ > /dev/null
source User.sh
popd > /dev/null

shopt -s nullglob

SOURCES=$(echo *.c)
OBJS=$(for f in $SOURCES; do echo objs/${f%.c}.o; done)
ASM_SOURCES=$(echo *.S)
OBJS="$OBJS $(for f in $ASM_SOURCES; do echo objs/${f%.S}.o; done)"

mkdir -p objs/

redo-ifchange ../libc.stage $OBJS

# Change these flags or their order and YOU DIE
# (or you have to fix the dynamic linker; close enough)

LDFLAGS="-m elf_i386 -z nodefaultlib"
LDFLAGS="$LDFLAGS -shared -soname=$ON_DISK_LINKER"
# Tells the linker to bind symbols to defns within this library. This simplifies
# the relocation table so by execution time the dynamic linker can bootstrap
# (i.e., relocate itself)
LDFLAGS="$LDFLAGS -Bsymbolic"
# Make sure we enter in the right place
LDFLAGS="$LDFLAGS -e _bootstrap"
# Link statically against libc
LDFLAGS="$LDFLAGS -static -L../libc/staging/lib/ -lc"
$LD -o $3 $OBJS $LDFLAGS

