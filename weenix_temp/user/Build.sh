# bash library

# Meant to be included by individual module build files
# (like myexec.do, default.o.do, etc.)
# Has some fun functions to compile and link things.

# Note that this script is NOT sanitary: don't call more
# than one function from it in a given shell script if you
# want them to work.

# Source/include paths, etc. have extra '../' because we're getting
# run from INSIDE a particular package. Note that this only
# works if we're running at the top-level inside that package
# (i.e., PWD should be ...weenix/user/mypkg/)

pushd ../ > /dev/null
source User.sh
source ../Compile.sh
popd > /dev/null

LDFLAGS="$LDFLAGS -z nodefaultlib"

CURPKG=$(basename $PWD)

# Build object files in ways that binaries/libraries
# generally like to.
# $1, $2: same as for compile fxn in Compile.sh
# (Name of target, place to put target)
function compile-bin {

	INCLUDE_DIR=../include/
	CFLAGS="$CFLAGS -I$INCLUDE_DIR"
	compile $1 $2
}

function compile-lib {
	
	CFLAGS="$CFLAGS -fpic"
	compile-bin $1 $2

}

# Common code for linking. Don't call this directly.
function link-common {
	local pkgs="libc" paths="/lib /usr/lib" libs="c"
	# Parse command-line options
	while getopts ":P:L:l:" opt; do
		case $opt in
			P)
				pkgs="$pkgs $OPTARG"
			;;
			L)
				paths="$paths $OPTARG"
			;;
			l)
				libs="$libs $OPTARG"
			;;
			*)
				print -u2 "Usage: link-common -P<package> -L<path> -l<libname> ..."
				return 2
			;;
		esac
	done
	# Also look in the current package by default for any libraries
	for pkg in $CURPKG $pkgs; do
		for p in $paths; do
			LDFLAGS="$LDFLAGS -L../$pkg/staging/$p"
		done
	done
	for lib in $libs; do
		LDFLAGS="$LDFLAGS -l$lib"
	done
	shift $((OPTIND - 1))
	local target=$2
	shift 2
	{
		echo "$@"
		for pkg in $pkgs; do
			echo ../$pkg/$pkg.stage
		done
	} | xargs redo-ifchange
	$LD -o "$target" "$@" $LDFLAGS
}

# Link a binary.
# -P[package]: a package that this binary depends on to build
#              (which probably contains a library we want)
# -L[path]:    where in those packages' staging area to look for libs
# -l[libname]: a library name to link against
#
# e.g., if we pass -Pmypkg -L/usr/mypkg/lib/ -lfoo
# we look for libfoo in ../mypkg/staging/usr/mypkg/lib/
# (These get converted into better -L and -l options to the linker)
#
# Note that binaries automatically receive
# -Plibc -L/usr/lib/ -lc
# implicitly (linking against libc)
#
# We also automatically seach the current package's staging area with -L,
# but we DON'T create dependencies (as this is a "package-internal" matter).
# Up to individual package to structure dependencies correctly.
#
# After all options:
# $1: target binary name, without extension (from redo $1).
# $2: output binary name (from redo $3).
# $3, $4, ...: object files to link.
# Note that all options need to appear before actual arguments.

function link-bin {
	mkdir -p objs/
	if (( 0 != $DYNAMIC )); then
		LDFLAGS="$LDFLAGS -e main --dynamic-linker $ON_DISK_LINKER"
	else
		LDFLAGS="$LIBC_ENTRY_OBJS $LDFLAGS -static -e $LIBC_STATIC_ENTRY"
	fi
	link-common "$@"
}

# Link a library.
# Very similar to link-bin, but builds libraries (duh).
# Same options + arguments, EXCEPT that must have an option
# (appearing before all others) of --dynamic-name or --static-name
# to indicate whether to build a dynamic or static library.
# This option also must take an argument indicating the target name
# of this link, with its extension (from redo $1$2).

function link-lib {
	mkdir -p objs/
	local build_dynamic="$1"
	if [[ $build_dynamic == "--dynamic-name" ]]; then
		LDFLAGS="$LDFLAGS -shared"
		# Find the on-disk name by looking up target name in Stagemap
		LDFLAGS="$LDFLAGS -soname=$(../stagemap --lookup ./Stagemap $2)"

		# Drop the weird arguments
		shift 2
		link-common "$@"
	elif [[ $build_dynamic == "--static-name" ]]; then
		# Drop the weird arguments
		shift 2
		# We're a static library, can't track dependencies, so
		# skip all dependency options.
		while getopts ":F:L:l:" opt; do
			# Bash doesn't allow empty loops
			opt=$opt
		done
		shift $((OPTIND - 1))

		local target=$2
		shift 2
		redo-ifchange "$@"
		# Build static library
		$AR crs "$target" "$@"
	else
		print -u2 "link-lib: Need --dynamic-name or --static-name"
		return 2
	fi
}
