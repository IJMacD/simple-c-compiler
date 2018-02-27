#!/bin/bash
mkdir -p build && cd build || (echo "Error creating directory" >&2 && exit -1)

rm *.o
clang -c ../src/*.c ../src/c/*.c
clang -lm *.o -o calc

rm *.o
clang -c ../src/*.c ../src/java/*.c
clang -lm *.o -o calc-java

rm *.o
clang -c ../src/*.c ../src/python/*.c
clang -lm *.o -o calc-python

rm *.o
clang -c ../src/*.c ../src/llvm/*.c
clang -lm *.o -o calc-llvm
