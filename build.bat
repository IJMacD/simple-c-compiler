@echo off
cd build
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
set compilerflags=/Od /Zi /EHsc
set linkerflags=/OUT:calc.exe
cl.exe %compilerflags% ../src/common/*.c ../src/c/*.c /link %linkerflags%

set linkerflags=/OUT:calc-java.exe
cl.exe %compilerflags% ../src/common/*.c ../src/java/*.c /link %linkerflags%

set linkerflags=/OUT:calc-python.exe
cl.exe %compilerflags% ../src/common/*.c ../src/python/*.c /link %linkerflags%

set linkerflags=/OUT:calc-llvm.exe
cl.exe %compilerflags% ../src/common/*.c ../src/llvm/*.c /link %linkerflags%

set linkerflags=/OUT:calc-wasm.exe
cl.exe %compilerflags% ../src/common/*.c ../src/wasm/*.c /link %linkerflags%

set linkerflags=/OUT:calc-cil.exe
cl.exe %compilerflags% ../src/common/*.c ../src/cil/*.c /link %linkerflags%
