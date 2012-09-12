#!/bin/bash -e

mkdir -p objs/

cd ..

source ../Build.sh

link-lib --static-name libtest/libtest.a libtest/$1 libtest/$3 libtest/objs/test.o
