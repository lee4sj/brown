#!/bin/bash -e

source Kernel.sh
TMP_FILE=$(mktemp)

redo-ifchange $SYMBOLS

$OBJCOPY -j .stage1 --set-section-flags .stage1=load --change-section-address .stage1=0 -O binary $SYMBOLS $TMP_FILE
cat $TMP_FILE > $3
$OBJCOPY -j .stage2 --set-section-flags .stage2=load --change-section-address .stage2=0 -O binary $SYMBOLS $TMP_FILE
cat $TMP_FILE >> $3
$OBJCOPY -O binary $SYMBOLS $TMP_FILE
cat $TMP_FILE >> $3
