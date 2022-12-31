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

#include "CommutationLookupTable.h"

Duty_Cycle_Struct GetPWMDutyCycles(uint8_t throttle_perc, uint8_t field_weakening_perc, commutation_type_enum commutation_type)
{
    switch (commutation_type)
    {
        case spwm_e:
            throttle_perc * SPWM_LUT[ ((uint16_t)direct_axis_angle + (90*(100-(uint16_t)field_weakening_perc))/100) % 360 ]
}