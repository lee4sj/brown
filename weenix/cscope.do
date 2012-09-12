#!/bin/bash -e

source Vars.sh

redo-ifchange $KERN_DIR/cscope
redo-ifchange $USER_DIR/cscope
