#!/bin/bash -e

source User.sh

PKG_TARGETS=$( for pkg in $PKGS; do echo $pkg/$pkg.stage; done)

redo-ifchange cscope $PKG_TARGETS $IMAGE
