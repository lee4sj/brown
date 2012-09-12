#!/bin/bash -e

source Kernel.sh

MODULE_TARGETS=$( for mod in $MODULES; do echo $mod/$mod.module; done)

redo-ifchange cscope $MODULE_TARGETS $ISO_IMAGE $SYMBOLS $BSYMBOLS $GDBCOMM
