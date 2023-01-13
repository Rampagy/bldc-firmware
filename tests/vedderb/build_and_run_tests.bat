@echo off

:: build test executable
gcc -Wdouble-promotion -m32 -Wall -Wextra -std=c99 test_vedderb_foc.c ..\unity\unity.c -o test_vedderb_foc

:: run test (if it returns 0 then pass else it failed)
test_vedderb_foc.exe