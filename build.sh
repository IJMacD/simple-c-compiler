#!/bin/bash
mkdir -p build && cd build || (echo "Error creating directory" >&2 && exit -1)

CC=clang

rm -f *.o
$CC -c ../src/*.c ../src/c/*.c
$CC -lm *.o -o calc

rm -f *.o
$CC -c ../src/*.c ../src/java/*.c
$CC -lm *.o -o calc-java

rm -f *.o
$CC -c ../src/*.c ../src/python/*.c
$CC -lm *.o -o calc-python

rm -f *.o
$CC -c ../src/*.c ../src/llvm/*.c
$CC -lm *.o -o calc-llvm

rm -f *.o
$CC -c ../src/*.c ../src/wasm/*.c
$CC -lm *.o -o calc-wasm

rm -f *.o
$CC -c ../src/*.c ../src/cil/*.c
$CC -lm *.o -o calc-cil
