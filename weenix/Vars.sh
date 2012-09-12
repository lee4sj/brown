# bash library

# This should ALWAYS be sourced after Config.sh

IMAGE=weenix.img
ISO_IMAGE=weenix.iso
SYMBOLS=weenix.dbg
BSYMBOLS=symbols.dbg
GDBCOMM=gdb-commands

GDB_INIT=init.gdb
GDB_TMP_INIT=inittmp.gdb
GDB_COMM_INIT=commands.gdb

KERN_DIR=kernel
USER_DIR=user
SCRIPT_DIR=python

LOG_FILE=bochsout.txt

# Contains general flags we want to use throughout Weenix

# ------------------------------ FLAGS -------------------------------------
# This is an operating system - don't include against standard stuff
CFLAGS="-fno-builtin -nostdinc"
# Warnings
CFLAGS="$CFLAGS -Wall -Wredundant-decls -Wundef -Wpointer-arith -Wfloat-equal"
CFLAGS="$CFLAGS -Wnested-externs -Wvla -Winline"
# Pedantic warnings
CFLAGS="$CFLAGS -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable"
# Very, very pedantic warnings
CFLAGS="$CFLAGS -ansi"
# Debugging
CFLAGS="$CFLAGS -g3"
# Weenix won't compile on newer versions of GCC without this
CFLAGS="$CFLAGS -fno-stack-protector"

# Compiler config definitions

# Add a compiler define of the form -D__VAR__=value for each user-defined option
# (Note that if Config.sh wasn't sourced, this will simply do nothing)
for def in $COMPILE_CONFIG_BOOLS; do
	if (( 0 != $def )); then
		CFLAGS="$CFLAGS -D__${def}__"
	fi
done

# Add a compiler define for each non-boolean, similarly
for def in $COMPILE_CONFIG_DEFS; do
	eval "CFLAGS=\"$CFLAGS -D__${def}__=\$$def\""
done

# Should take this out before giving to students (not like it matters)
CFLAGS="$CFLAGS -DSTUDENT=0"

# Assembly flag
ASFLAGS=-D__ASSEMBLY__
