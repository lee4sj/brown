#!/bin/bash -e

source Vars.sh

# clean will rebuild these files, so this just makes sure they are up to date
rm -f System.build
rm -f Config.build

redo $KERN_DIR/clean $USER_DIR/clean
