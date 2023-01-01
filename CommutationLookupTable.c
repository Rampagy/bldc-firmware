#include "CommutationLookupTable.h"


Duty_Cycle_Struct GetPWMDutyCycles(uint16_t direct_axis_angle, uint8_t throttle_perc, uint8_t field_weakening_perc, commutation_type_enum commutation_type)
{
	int16_t field_weakening_angle;
    uint16_t lookup_angle_a;
    uint16_t lookup_angle_b;
    uint16_t lookup_angle_c;
	int8_t sign;
	
    Duty_Cycle_Struct duty_cycles = {
        .a = 0,
        .b = 0,
        .c = 0
    };
    
    // bounds checks and sign determination
    field_weakening_perc = (field_weakening_perc >= 100) ? 0 : field_weakening_perc;
    sign = (throttle_perc >= 0) ? 1 : -1;
    
    field_weakening_angle = (int16_t)direct_axis_angle + (int16_t)sign*((90*(100-(uint16_t)field_weakening_perc))/100);
    lookup_angle_a = (0 + field_weakening_angle) % 360;
    lookup_angle_b = (240 + field_weakening_angle) % 360;
	lookup_angle_c = (120 + field_weakening_angle) % 360;
    
    
    /* Determine commutation type */
    switch (commutation_type)
    {
        case spwm_e:
            // use sinusoidal pwm LUT
            duty_cycles.a = SPWM_LUT[lookup_angle_a];
            duty_cycles.b = SPWM_LUT[lookup_angle_b];
            duty_cycles.c = SPWM_LUT[lookup_angle_c];
            break;
         
        case svpwm_e:
        default:
            // use space vector pwm LUT
            duty_cycles.a = SVPWM_LUT[lookup_angle_a];
            duty_cycles.b = SVPWM_LUT[lookup_angle_b];
            duty_cycles.c = SVPWM_LUT[lookup_angle_c];
            break;
    } /* ~switch (commutation_type) */
     
    /* Scale according to the desired throttle */
    duty_cycles.a = (throttle_perc*duty_cycles.a) / 100;
    duty_cycles.b = (throttle_perc*duty_cycles.b) / 100;
    duty_cycles.c = (throttle_perc*duty_cycles.c) / 100;
    return duty_cycles;
}