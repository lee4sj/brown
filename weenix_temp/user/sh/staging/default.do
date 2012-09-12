#!/bin/bash -e

# To rebuild a particular staged file, we need to
# first look it up in the package's stagemap.

# $1 is going to look like usr/bin/foo
# PACKAGE=mypkg
# TARGET=/usr/bin/foo
TARGET=/$1
cd .. # We're in mypkg/
PACKAGE=$(basename $PWD)

ORIGIN=$(../stagemap --reverse-lookup Stagemap $TARGET)

redo-ifchange Stagemap $ORIGIN

# copy file (or directory, hence -r) to staged location
cp -r $ORIGIN staging/$3
