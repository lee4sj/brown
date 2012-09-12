#!/bin/bash -e

source Kernel.sh

redo-ifchange $SYMBOLS

$READELF -s $SYMBOLS | grep -Ev "SECTION|UND|FILE|Num:|Symbol|^$" | awk '{printf "0x%s %s\n", $2, $8}' > $3
