/*
* This file was automatically generated using csnake v0.3.5.
*
* This file should not be edited directly, any changes will be
* overwritten next time the script is run.
* Make any changes to the file 'GenLookUpTables.py', not this file.
*
* Source code for csnake is available at:
* https://gitlab.com/andrejr/csnake
*
* csnake is also available on PyPI, at :
* https://pypi.org/project/csnake
*/

#ifndef COMM_LOOK_UP_TABLE_CH
#define COMM_LOOK_UP_TABLE_CH

/* Lookup table used for Sinusoidal PWM commutation */
uint8_t SPWM_LUT[360] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 97, 97, 97, 96, 96, 96, 95, 95, 95, 94, 94, 93, 93, 92, 92, 91, 91, 90, 90, 89, 89, 88, 88, 87, 87, 86, 85, 85, 84, 83, 83, 82, 81, 81, 80, 79, 79, 78, 77, 76, 76, 75, 74, 73, 73, 72, 71, 70, 70, 69, 68, 67, 66, 65, 65, 64, 63, 62, 61, 60, 60, 59, 58, 57, 56, 55, 54, 53, 53, 52, 51, 50, 49, 48, 47, 47, 46, 45, 44, 43, 42, 41, 40, 40, 39, 38, 37, 36, 35, 35, 34, 33, 32, 31, 30, 30, 29, 28, 27, 27, 26, 25, 24, 24, 23, 22, 21, 21, 20, 19, 19, 18, 17, 17, 16, 15, 15, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15, 15, 16, 17, 17, 18, 19, 19, 20, 21, 21, 22, 23, 24, 24, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 33, 34, 35, 35, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45, 46, 47, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70, 70, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 79, 79, 80, 81, 81, 82, 83, 83, 84, 85, 85, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 92, 92, 93, 93, 94, 94, 95, 95, 95, 96, 96, 96, 97, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100};

/* Lookup table used for Alternating Reversing Sequence Space Vector PWM commutation: */
/* Understanding Space Vector Modulation by Peter Pinewski https://go.gale.com/ps/i.do?id=GALE%7CA18320578&sid=sitemap&v=2.1&it=r&p=AONE&sw=w&userGroupName=anon%7Ebc1f9809 */
/* https://youtu.be/5eQyoVMz1dY */
uint8_t ARS_SVPWM_LUT[360] = {93, 94, 94, 95, 95, 95, 96, 96, 96, 97, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 97, 97, 97, 96, 96, 96, 95, 95, 95, 94, 94, 93, 92, 91, 89, 88, 87, 85, 84, 82, 81, 80, 78, 77, 75, 74, 72, 71, 69, 68, 67, 65, 64, 62, 61, 59, 58, 56, 55, 53, 52, 50, 48, 47, 45, 44, 42, 41, 39, 38, 36, 35, 33, 32, 31, 29, 28, 26, 25, 23, 22, 20, 19, 18, 16, 15, 13, 12, 11, 9, 8, 7, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 8, 9, 11, 12, 13, 15, 16, 18, 19, 20, 22, 23, 25, 26, 28, 29, 31, 32, 33, 35, 36, 38, 39, 41, 42, 44, 45, 47, 48, 50, 52, 53, 55, 56, 58, 59, 61, 62, 64, 65, 67, 68, 69, 71, 72, 74, 75, 77, 78, 80, 81, 82, 84, 85, 87, 88, 89, 91, 92, 93, 94, 94, 95, 95, 95, 96, 96, 96, 97, 97, 97, 98, 98, 98, 98, 99, 99, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 97, 97, 97, 96, 96, 96, 95, 95, 95, 94, 94};

/* Lookup Table used for Null-V0 Space Vector PWM commutation: */
/* Understanding Space Vector Modulation by Peter Pinewski https://go.gale.com/ps/i.do?id=GALE%7CA18320578&sid=sitemap&v=2.1&it=r&p=AONE&sw=w&userGroupName=anon%7Ebc1f9809 */
/* https://youtu.be/5eQyoVMz1dY */
uint8_t NULLV0_SVPWM_LUT[360] = {87, 87, 88, 89, 90, 91, 91, 92, 93, 93, 94, 95, 95, 96, 96, 97, 97, 97, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 99, 98, 98, 98, 97, 97, 97, 96, 96, 95, 95, 94, 93, 93, 92, 91, 91, 90, 89, 88, 87, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 75, 74, 73, 72, 71, 69, 68, 67, 66, 64, 63, 62, 60, 59, 57, 56, 54, 53, 52, 50, 48, 47, 45, 44, 42, 41, 39, 37, 36, 34, 33, 31, 29, 28, 26, 24, 22, 21, 19, 17, 16, 14, 12, 10, 9, 7, 5, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 22, 24, 26, 28, 29, 31, 33, 34, 36, 37, 39, 41, 42, 44, 45, 47, 48, 50, 52, 53, 54, 56, 57, 59, 60, 62, 63, 64, 66, 67, 68, 69, 71, 72, 73, 74, 75, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 91, 92, 93, 93, 94, 95, 95, 96, 96, 97, 97, 97, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 99, 98, 98, 98, 97, 97, 97, 96, 96, 95, 95, 94, 93, 93, 92, 91, 91, 90, 89, 88, 87};

#endif /* COMM_LOOK_UP_TABLE_CH */