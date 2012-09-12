#!/bin/bash -e

source ../Build.sh

redo-ifchange staging/usr/lib/libtest.a staging/usr/lib/libtest.so
link-bin -L/usr/lib/ -ltest $1 $3 objs/memtest.o
