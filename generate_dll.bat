:: call gcc -c -Iinclude CommutationLookupTable.c
:: call gcc -shared -o CommutationLookupTable.dll CommutationLookupTable.o
call gcc -Iinclude -shared -o CommutationLookupTable.so CommutationLookupTable.c