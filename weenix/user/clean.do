#!/bin/bash -e

source User.sh

PKG_CLEANS=$( for pkg in $PKGS; do echo $pkg/$pkg.clean; done )

for pkg in $PKGS; do
	pushd $pkg/staging > /dev/null
	find . -not -name . -and -not -name default.do | xargs rm -rf
	popd > /dev/null
done

redo $PKG_CLEANS
rm -f cscope.out cscope.in.out cscope.po.out cscope.files
rm -f $IMAGE
rm -rf staging/
