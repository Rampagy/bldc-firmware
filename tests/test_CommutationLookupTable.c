#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unity/unity.h"
#include "../include/CommutationLookupTable.h"

// constructor and deconstructor for unity test suite
void setUp (void) {} /* Is run before every test, put unit init calls here. */
void tearDown (void) {} /* Is run after every test, put unit clean-up calls here. */


void test_spwm_commutation(void)
{
    Duty_Cycle_Struct phase_dutys;
    char line[1024];
    int line_num = 1;

    // read test solutions
    FILE* stream = fopen("solutions/spwm.csv", "r");
    while (fgets(line, 1024, stream))
    {
        // these must be int otherwise they are not read in properly during sscanf()
        int angle, throttle, field_weakening, a, b, c = 0;
        int items_read = sscanf(line, "%d,%d,%d,%d,%d,%d\n", &angle, &throttle, &field_weakening, &a, &b, &c);

        // if 6 items were not read it is not a valid test case
        if (items_read == 6)
        {
            phase_dutys = GetPWMDutyCycles((uint16_t)angle, (int8_t)throttle, (uint8_t)field_weakening, spwm_e);

            char a_error_msg[64];
            char b_error_msg[64];
            char c_error_msg[64];

            sprintf(a_error_msg, "(line %d, Phase A)", line_num);
            sprintf(b_error_msg, "(line %d, Phase B)", line_num);
            sprintf(c_error_msg, "(line %d, Phase C)", line_num);

            // Some minor rounding errors due to float vs integer math.  Allow up to 1 degrees of tolerance per phase
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, a, phase_dutys.a, a_error_msg);
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, b, phase_dutys.b, b_error_msg);
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, c, phase_dutys.c, c_error_msg);
        }
        line_num++;
    }
}

void test_ars_svpwm_commutation(void)
{
    Duty_Cycle_Struct phase_dutys;
    char line[1024];
    int line_num = 1;

    // read test solutions
    FILE* stream = fopen("solutions/ars_svpwm.csv", "r");
    while (fgets(line, 1024, stream))
    {
        // these must be int otherwise they are not read in properly during sscanf()
        int angle, throttle, field_weakening, a, b, c = 0;
        int items_read = sscanf(line, "%d,%d,%d,%d,%d,%d\n", &angle, &throttle, &field_weakening, &a, &b, &c);

        // if 6 items were not read it is not a valid test case
        if (items_read == 6)
        {
            phase_dutys = GetPWMDutyCycles((uint16_t)angle, (int8_t)throttle, (uint8_t)field_weakening, ars_svpwm_e);

            char a_error_msg[64];
            char b_error_msg[64];
            char c_error_msg[64];

            sprintf(a_error_msg, "(line %d, Phase A)", line_num);
            sprintf(b_error_msg, "(line %d, Phase B)", line_num);
            sprintf(c_error_msg, "(line %d, Phase C)", line_num);

            // Some minor rounding errors due to float vs integer math.  Allow up to 1 degrees of tolerance per phase
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, a, phase_dutys.a, a_error_msg);
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, b, phase_dutys.b, b_error_msg);
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, c, phase_dutys.c, c_error_msg);
        }
        line_num++;
    }
}

void test_nullv0_svpwm_commutation(void)
{
    Duty_Cycle_Struct phase_dutys;
    char line[1024];
    int line_num = 1;

    // read test solutions
    FILE* stream = fopen("solutions/nullv0_svpwm.csv", "r");
    while (fgets(line, 1024, stream))
    {
        // these must be int otherwise they are not read in properly during sscanf()
        int angle, throttle, field_weakening, a, b, c = 0;
        int items_read = sscanf(line, "%d,%d,%d,%d,%d,%d\n", &angle, &throttle, &field_weakening, &a, &b, &c);

        // if 6 items were not read it is not a valid test case
        if (items_read == 6)
        {
            phase_dutys = GetPWMDutyCycles((uint16_t)angle, (int8_t)throttle, (uint8_t)field_weakening, nullv0_svpwm_e);

            char a_error_msg[64];
            char b_error_msg[64];
            char c_error_msg[64];

            sprintf(a_error_msg, "(line %d, Phase A)", line_num);
            sprintf(b_error_msg, "(line %d, Phase B)", line_num);
            sprintf(c_error_msg, "(line %d, Phase C)", line_num);

            // Some minor rounding errors due to float vs integer math.  Allow up to 1 degrees of tolerance per phase
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, a, phase_dutys.a, a_error_msg);
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, b, phase_dutys.b, b_error_msg);
            TEST_ASSERT_UINT8_WITHIN_MESSAGE(1, c, phase_dutys.c, c_error_msg);
        }
        line_num++;
    }
}

int main (int argc, char* args[])
{
    (void)argc;
    (void)args;

    UNITY_BEGIN();
    RUN_TEST(test_spwm_commutation);
    RUN_TEST(test_ars_svpwm_commutation);
	RUN_TEST(test_nullv0_svpwm_commutation);
    return UNITY_END();
}