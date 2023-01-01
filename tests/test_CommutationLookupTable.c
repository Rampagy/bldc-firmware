#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/CommutationLookupTable.h"




int main (int argc, char* args[])
{
    Duty_Cycle_Struct phase_dutys;
    char line[1024];
    int line_num = 1;
    int fail_count = 0;

    // read test solutions
    FILE* stream = fopen("solutions/spwm.csv", "r");
    while (fgets(line, 1024, stream))
    {
        int angle, throttle, field_weakening, a, b, c = 0;
        int items_read = sscanf(line, "%d,%d,%d,%d,%d,%d\n", &angle, &throttle, &field_weakening, &a, &b, &c);
        if (items_read == 6)
        {
            phase_dutys = GetPWMDutyCycles((uint16_t)angle, (int8_t)throttle, (uint8_t)field_weakening, spwm_e);

            // Some minor rounding errors due to float vs integer math.  Allow up to 3 degrees of rounding errors across all phases
            if ((abs(phase_dutys.a - a) + abs(phase_dutys.b - b) + abs(phase_dutys.c - c)) > 3)
            {
                printf("spwm line %d test case failed, phases=(%d, %d, %d)\n", line_num, phase_dutys.a, phase_dutys.b, phase_dutys.c);
                fail_count++;
            }
            line_num++;
        }
    }

    // read test solutions
    line_num = 1;
    stream = fopen("solutions/svpwm.csv", "r");
    while (fgets(line, 1024, stream))
    {
        int angle, throttle, field_weakening, a, b, c = 0;
        int items_read = sscanf(line, "%d,%d,%d,%d,%d,%d\n", &angle, &throttle, &field_weakening, &a, &b, &c);
        if (items_read == 6)
        {
            phase_dutys = GetPWMDutyCycles((uint16_t)angle, (int8_t)throttle, (uint8_t)field_weakening, svpwm_e);

            // Some minor rounding errors due to float vs integer math.  Allow up to 3 degrees of rounding errors across all phases
            if ((abs(phase_dutys.a - a) + abs(phase_dutys.b - b) + abs(phase_dutys.c - c)) > 3)
            {
                printf("svpwm line %d test case failed, phases=(%d, %d, %d)\n", line_num, phase_dutys.a, phase_dutys.b, phase_dutys.c);
                fail_count++;
            }
            line_num++;
        }
    }

    return fail_count;
}