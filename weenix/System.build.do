#!/bin/bash -e

./system-check -qe > $3

redo-always
redo-stamp < $3
