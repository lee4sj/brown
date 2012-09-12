# bash library

# Used as user and kernel compilation are very similar,
# but just different enough to be annoying.

# Hunts down the source corresponding to an object file, builds it,
# with header dependencies and all that fun stuff.
#
# $1: File to build, WITHOUT the .o extension.
# $2: File to build into (since we're on redo...)
# Needs CC, CFLAGS, and ASFLAGS from env.
#
# Note that this assumes that objects files go in a subdirectory
# of where the source files are (objs/ or something)
function compile {

	local SRC SRCFILE TMPFILE DEPENDS

	SRC=$(dirname $(dirname $1))/$(basename $1)

	# Check if we're compiling assembly file
	if [[ -f $SRC.S ]]; then
		SRCFILE=$SRC.S
	else
		SRCFILE=$SRC.c
	fi

	# Hunt down header dependencies
	TMPFILE=$(mktemp)
	$CC -MM -MF $TMPFILE $SRCFILE $CFLAGS
	DEPENDS=$(sed -e 's/^.*://' -e 's/\\//g' < $TMPFILE)
	rm -f $TMPFILE

	redo-ifchange $DEPENDS

	# Check if we're compiling assembly file
	if [[ -f $SRC.S ]]; then
		$CC -c $SRCFILE -o $2 $ASFLAGS $CFLAGS
	else
		$CC -c $SRCFILE -o $2 $CFLAGS
	fi
}
