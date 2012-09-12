#!/bin/bash -e

source Kernel.sh

MODULE_ARS=$( for mod in $MODULES; do echo $mod/lib$mod.a; done )

redo-ifchange $MODULE_ARS

$LD $LDFLAGS -o $3
