#!/bin/bash
mkdir -p build && cd build || (echo "Error creating directory" >&2 && exit -1)
clang -c ../src/*.c
clang *.o -o calc
