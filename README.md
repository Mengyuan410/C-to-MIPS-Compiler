# C-to-MIPS-Compiler
:star:This repository is the compiler project of Instruction Architecture and Compiler course in 2nd Year Imperial College London EIE department. 

The project passed all provided testcases and 184 out of 202 of examiners' testcases (including unseen tests not in this repo). 

## Overview
The project is a C to MIPS compiler written in C++. The compiler lex, parse, and generate correspondin MIPS code from the given input C programme. The project specification is in the *project_description* directory. The project management logbook is contained inside *project_description/project_managment_log.md*.

## Running Guidelines
Use <code> ./utility/test_dir.sh </code> to run all test cases.

Use <code> ./utility/test_dir.sh DIRNAME </code> to run a specific testcase folder. DIRNAME is the folder name of the testcase type that you want to run. For example, if you want to run testcases in */compiler_tests/array*, run code <code> ./utility/test_dir.sh array </code>.

Use <code> ./utility/test.sh </code> to run the single testcase */compiler_tests/my_test.c*. You can use this during debugging process, by substituting the content of *my_test.c* and *my_test_driver.c* with the C programme that you want to test. 
