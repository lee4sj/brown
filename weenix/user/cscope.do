#!/bin/bash -e

pushd .. >/dev/null
redo-ifchange Build.sh
source Build.sh
popd >/dev/null

if [[ -z $CSCOPE ]]; then
	echo "warning: skipping cscope index generation: $CSCOPE_ERROR" >&2
	redo-always
else
	CSCOPE_FILES=$(find -name "*.[chS]")

	redo-ifchange $CSCOPE_FILES
	find -name "*.[chS]" > cscope.files
	$CSCOPE -k -b -q -v > /dev/null
fi
