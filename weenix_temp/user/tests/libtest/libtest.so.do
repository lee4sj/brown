#!/bin/bash -e

mkdir -p objs/

cd ..

source ../Build.sh

link-lib --dynamic-name libtest/libtest.so libtest/$1 libtest/$3 libtest/objs/test.o
