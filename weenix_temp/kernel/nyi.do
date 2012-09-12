#!/bin/bash -e

pushd ../ > /dev/null
redo-ifchange Build.sh
source Build.sh
popd > /dev/null

PROJ_FILTER=
for def in $COMPILE_CONFIG_BOOLS; do
	if eval "echo \$$def" | grep -v 0 > /dev/null; then
		PROJ_FILTER="$PROJ_FILTER grep -v $def |"
	fi
done

for f in $(find . -name "*.c"); do
   	eval "grep -Hn NOT_YET_IMPLEMENTED $f | grep -v define | $PROJ_FILTER sed -e 's/^\\(.*:.*:\\).*\"\\(.*\\):\\(.*\\)\".*/\\1 \\2 \\3/' | awk '{printf(\"%15s %15s() %10s\n\", \$1, \$3, \$2)}'" > /dev/fd/2
done
