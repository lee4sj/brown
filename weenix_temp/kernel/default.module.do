#!/bin/bash -e

# By default, building a module just involves building its archive file

redo-ifchange $(dirname $1)/lib$(basename $1).a
