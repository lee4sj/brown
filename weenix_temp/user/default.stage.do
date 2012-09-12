#!/bin/bash -e

# By default, staging a package will depend on all the targets
# listed in its stage map being, well, staged.

PKG=$(basename $1)

TARGETS=$(./stagemap --values $PKG/Stagemap)

for target in $TARGETS; do
	mkdir -p $(dirname $PKG/staging/$target)
done

{
	echo $PKG/Stagemap
	for target in $TARGETS; do
		echo $PKG/staging/$target
	done
} | xargs redo-ifchange
