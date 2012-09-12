#!/bin/bash -e

redo-ifchange ../Config.sh

source Kernel.sh
source ../Config.sh

redo-ifchange $KERNEL

dd if=/dev/zero of=$3 bs=1024 count=1440 2> /dev/null
dd if=$KERNEL of=$3 conv=notrunc 2> /dev/null
