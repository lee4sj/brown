#!/bin/bash -e

source Kernel.sh

redo-ifchange $IMAGE

if which mkisofs >/dev/null 2>/dev/null; then
	MAKEISO=mkisofs
else
	MAKEISO=genisoimage
fi

mkdir -p empty/
$MAKEISO -quiet -input-charset utf-8 -o $3 -b $IMAGE $IMAGE
rmdir empty/
