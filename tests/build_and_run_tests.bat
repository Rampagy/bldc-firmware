:: build test executable
gcc -I..\include test_CommutationLookupTable.c ..\CommutationLookupTable.c -o test_CommutationLookupTable

:: generate new solutions
python ../Tools/gen_test_solutions.py

:: run test (if it returns 0 then pass else it failed)
test_CommutationLookupTable.exe > test_output.log

@echo off
if %ERRORLEVEL% NEQ 0 (
    echo %ERRORLEVEL% Tests cases failed 
    exit /b %ERRORLEVEL%
) else (
    echo Tests Passed
)