# This file must be sourced after Vars.sh or CFLAGS will be overwritten
# bash library
# Target architecture
CFLAGS="$CFLAGS -m32 -march=i686 -fsigned-char"
LDFLAGS="$LDFLAGS -m elf_i386"
