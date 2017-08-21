#!/bin/bash
mkdir -p build && cd build || (echo "Error creating directory" >&2 && exit -1)
clang -c ../src/lexer.c
clang -c ../src/parser.c
clang -c ../src/traverser.c
clang -c ../src/transformer.c
clang -c ../src/generator.c
clang -c ../src/linker.c
clang -c ../src/executor.c
clang -c ../src/compiler.c
clang -c ../src/util.c
clang -c ../src/calc.c
clang lexer.o parser.o traverser.o transformer.o generator.o linker.o executor.o util.o calc.o compiler.o -o calc
