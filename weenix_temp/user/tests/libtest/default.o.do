#!/bin/bash -e

cd ..

source ../Build.sh
compile-lib libtest/$1 libtest/$3
