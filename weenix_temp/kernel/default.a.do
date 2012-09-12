#!/bin/bash -e

source Kernel.sh

shopt -s nullglob

# Get the actual module name
MODULE=$(basename $1)
PATH_TO_MODULE=$(dirname $1)

# Depend on objects for all .c files
SOURCES=$(echo $PATH_TO_MODULE/*.c)
OBJS=$(for f in $SOURCES; do echo $(dirname $f)/objs/$(basename $f .c).o; done)
ASM_SOURCES=$(echo $PATH_TO_MODULE/*.S)
OBJS="$OBJS $(for f in $ASM_SOURCES; do echo $(dirname $f)/objs/$(basename $f .S).o; done)"

mkdir -p $PATH_TO_MODULE/objs/

redo-ifchange $OBJS

if [[ -f $PATH_TO_MODULE/Submodules ]]; then
	redo-ifchange $PATH_TO_MODULE/Submodules
	SUBMODULES=$(cat $PATH_TO_MODULE/Submodules)
	# Build each submodule
	for submod in $SUBMODULES; do
		echo $PATH_TO_MODULE/$submod/$submod.module
	done | xargs redo-ifchange
	for submod in $SUBMODULES; do
		# The disgusting archive combining hack
		cd $PATH_TO_MODULE/objs/ && $AR x ../$submod/lib$submod.a && cd - > /dev/null
	done
else
	redo-ifcreate $PATH_TO_MODULE/Submodules
fi
for obj in $(echo $PATH_TO_MODULE/objs/*.o); do
	cp $obj $PATH_TO_MODULE/objs/${MODULE}_$(basename $obj)
done

# Finally, build / combine the archive
rm -f $3
$AR crs $3 $PATH_TO_MODULE/objs/${MODULE}_*.o > /dev/null
rm -f $PATH_TO_MODULE/objs/${MODULE}_*.o
