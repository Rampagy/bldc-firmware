#include <assert.h>
#include <stdio.h>
#include "include/CommutationLookupTable.h"


int main (int argc, char* args[])
{
    Duty_Cycle_Struct phase_dutys;

    // test full forward at 0 degrees (SPWM)
    phase_dutys = GetPWMDutyCycles(0, 100, 0, spwm_e);
    assert(phase_dutys.a == 100 && phase_dutys.b == 25 && phase_dutys.c == 25);

    // test full reverse at 0 degrees (SPWM)
    phase_dutys = GetPWMDutyCycles(0, -100, 0, spwm_e);
    assert(phase_dutys.a == 0 && phase_dutys.b == 75 && phase_dutys.c == 75);

    return 0;
}