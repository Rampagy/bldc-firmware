:: call gcc -c -Iinclude CommutationLookupTable.c
:: call gcc -shared -o CommutationLookupTable.dll CommutationLookupTable.o
::call gcc -Iinclude -shared -o CommutationLookupTable.so CommutationLookupTable.c

gcc -Iinclude test_CommutationLookupTable.c CommutationLookupTable.c -o test_CommutationLookupTable

:: run test (if it returns 0 then pass else it failed)
test_CommutationLookupTable.exe

@echo off
if errorlevel 0 (
    echo Tests Passed
) else (
    echo Tests Failed %errorlevel%
    exit /b %errorlevel%
)