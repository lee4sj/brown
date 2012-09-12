# bash library

pushd ../ > /dev/null
redo-ifchange Build.sh
source Build.sh
popd > /dev/null

# Check whether they need the fancy disk or not
if (( 0 != $VM )); then
	PKGS="basefs coreutils ed init ld-weenix libc sh simple tests"
else
	PKGS="basefs"
fi

IMAGE=disk0.img

ON_DISK_LINKER=/lib/ld-weenix.so
ON_DISK_LIBC=/lib/libc.so

LIBC_ENTRY_OBJS=../libc/objs/entry.o
LIBC_STATIC_ENTRY=__libc_static_entry
