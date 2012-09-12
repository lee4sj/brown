#!/bin/bash -e

source Kernel.sh

for mod in $MODULES; do
	echo $mod/$mod.gdbcomm
done | xargs redo-ifchange

for mod in $MODULES; do
	cat $mod/$mod.gdbcomm >> $3
done
