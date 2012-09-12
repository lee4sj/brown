#!/bin/bash -e

source User.sh
source ../Config.sh

tmpfile=$(mktemp /tmp/weenix.disk0.XXXXXXXXXX)
rm -f $tmpfile

PKG_TARGETS=$( for pkg in $PKGS; do echo $pkg/$pkg.stage; done)
redo-ifchange $PKG_TARGETS

# Synchronize all staging directories into the global staging directory
for pkg in $PKGS; do
	rsync --exclude="default.do" -r $pkg/staging/ staging/ >&2
done

../fsmaker $3 -e "format -b $DISK_BLOCKS -i $DISK_INODES -d staging/"
