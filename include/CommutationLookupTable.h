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

#ifndef COMM_LOOK_UP_TABLE_H
#define COMM_LOOK_UP_TABLE_H

/* INCLUDES */
#include <stdint.h>

/* TYPE DEFINITIONS */
typedef enum
{
    spwm_e,
    svpwm_e
} commutation_type_enum;

typedef struct
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
} Duty_Cycle_Struct;

/* GLOBAL VARIABLES */

/* FUNCTION PROTOTYPES */
Duty_Cycle_Struct GetPWMDutyCycles(uint16_t direct_axis_angle, int8_t throttle_perc, uint8_t field_weakening_perc, commutation_type_enum commutation_type);

#endif /* COMM_LOOK_UP_TABLE_H */