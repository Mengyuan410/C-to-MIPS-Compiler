#!/bin/bash

# filename = "compiler_tests/my_test"

# make bin/c_compiler 

# bin/c_compiler -S "${filename}.c" -o "${filename}.s"

# mips-linux-gnu-gcc -mfp32 -static -o "${filename}.o" -c "${filename}.s"

# mips-linux-gnu-gcc -mfp32 -static -o "${filename}" "${filename}.o" "${filename}_driver.c"

# qemu-mips "${filename}"
# echo $?

make bin/c_compiler 

bin/c_compiler -s compiler_tests/my_test.c -o my_test.s
#mips-linux-gnu-gcc -fverbose-asm -S compiler_tests/my_test.c -o my_test.s

mips-linux-gnu-gcc -mfp32 -static -o my_test.o -c my_test.s

mips-linux-gnu-gcc -mfp32 -static -o my_test my_test.o compiler_tests/my_test_driver.c

qemu-mips my_test

echo $?