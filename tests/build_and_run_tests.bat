@echo off

:: build test executable
gcc -m32 -I..\include test_CommutationLookupTable.c ..\CommutationLookupTable.c unity/unity.c -o test_CommutationLookupTable

:: generate new solutions
python ../Tools/gen_test_solutions.py

:: run test (if it returns 0 then pass else it failed)
test_CommutationLookupTable.exe