# C-to-MIPS-Compiler
:star:This repository is the compiler project of Instruction Architecture and Compiler course in 2nd Year Imperial College London EIE department. The project passed 184 out of 202 of examiners' tests (including unseen tests not in this repo). 

## Guide
The project specification is in the project_description directory.

Use <code> ./utility/test_dir.sh </code> to run all test cases.

Use <code> ./utility/test_dir.sh DIRNAME </code> to run a specific testcase folder. DIRNAME is the folder name of the testcase type that you want to run. For example, if you want to run testcases in */compiler_tests/array*, run code <code> ./utility/test_dir.sh array </code>.

Use <code> ./utility/test.sh </code> to run the single testcase */compiler_tests/my_test.c*. You can use this during debugging process, by substituting the content of *my_test.c* and *my_test_driver.c* with the C programme that you want to test. 
