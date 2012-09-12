#!/bin/bash -e

source Kernel.sh

MODULE_CLEANS=$( for mod in $CLEAN_MODULES; do echo $mod/$mod.clean; done )

redo $MODULE_CLEANS
rm -f cscope.out cscope.in.out cscope.po.out cscope.files
rm -f $IMAGE $ISO_IMAGE $KERNEL $SYMBOLS $BSYMBOLS $GDBCOMM
