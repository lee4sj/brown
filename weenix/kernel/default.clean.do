#!/bin/bash -e

DIR=$(dirname $1)

rm -f $DIR/*.a $DIR/objs/*.o $DIR/*.gdbcomm

if [[ -f $DIR/Submodules ]]; then
	SUBMODULES=$(cat $DIR/Submodules)
	for submod in $SUBMODULES; do
		echo $DIR/$submod/$submod.clean;
	done | xargs redo
fi
