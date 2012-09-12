# bash library

pushd ../ > /dev/null
redo-ifchange Build.sh
source Build.sh
popd > /dev/null

MODULES="entry main boot util drivers mm proc fs vm api test"
CLEAN_MODULES="entry main boot util drivers mm proc fs vm api test"

KERNEL=kernel.bin

# Everyone needs to know where to look up depended-on header files
INCLUDE_DIR=include
CFLAGS="$CFLAGS -I$INCLUDE_DIR"

# This is an OS kernel - don't look for main
CFLAGS="$CFLAGS -ffreestanding"
CFLAGS="$CFLAGS -D__KERNEL__" # For kernel-user shared stuff

LDFLAGS="$LDFLAGS -omagic -z nodefaultlib -T link.ld --whole-archive"
for mod in $MODULES; do
	LDFLAGS="$LDFLAGS -L$mod -l$mod"
done
