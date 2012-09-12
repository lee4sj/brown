#!/bin/bash -e

source Kernel.sh

shopt -s nullglob

PATH_TO_MODULE=$(dirname $1)

SCRIPTS=$(echo $PATH_TO_MODULE/*.gdb $PATH_TO_MODULE/*.py)

redo-ifchange $SCRIPTS

touch $3

for s in $SCRIPTS; do
	echo "$(pwd)/$s" >> $3
done

if [[ -f $PATH_TO_MODULE/Submodules ]]; then
	SUBMODULES=$(cat $PATH_TO_MODULE/Submodules)
	for submod in $SUBMODULES; do
		echo $PATH_TO_MODULE/$submod/$submod.gdbcomm
	done | xargs redo-ifchange
	for submod in $SUBMODULES; do
		cat $PATH_TO_MODULE/$submod/$submod.gdbcomm >> $3
	done
fi
