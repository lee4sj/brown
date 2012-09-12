#!/bin/bash -e

pushd ../ > /dev/null
source User.sh
popd > /dev/null

shopt -s nullglob

SOURCES=$(echo *.c)
OBJS=$(for f in $SOURCES; do echo objs/${f%.c}.o; done)
ASM_SOURCES=$(echo *.S)
OBJS="$OBJS $(for f in $ASM_SOURCES; do echo objs/${f%.S}.o; done)"

mkdir -p objs/

redo-ifchange $OBJS
$AR crs $3 $OBJS
