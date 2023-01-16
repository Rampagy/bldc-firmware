/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* Kernel includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "timers.h"
//#include "semphr.h"

#include <stdint.h>
#include <math.h>

/* Hardware and starter kit includes. */
#include "arm_comm.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "system_stm32f4xx.h"

/* Project includes. */
#include "stm32f4xx_rcc.h"
#include "debug.h"
#include "foc.h"

extern xDebugStats_t xDebugStats;

/*-----------------------------------------------------------*/

int main(void)
{
    /* Remove the secret clock doubler.... */
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR &= ~RCC_CFGR_PPRE2;

    RCC->CFGR |= RCC_CFGR_PPRE1_DIV8;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV4;

    /* Update the MCU and peripheral clock frequencies */
    SystemCoreClockUpdate();

    /* Initialize all four LEDs built into the starter kit */
    STM_EVAL_LEDInit( LED3 );
    STM_EVAL_LEDInit( LED4 );
    STM_EVAL_LEDInit( LED5 );
    STM_EVAL_LEDInit( LED6 );

    /* Initialize the debug/performance timer. */
    vInitDebug();

	/* performance test*/
	uint16_t startTime, stopTime = 0u;
	for (volatile int i = 0; i < 100; i++) {
		for (float j = 0.0f; j < 360.0f; j += 1.0f) {
			uint32_t tAout, tBout, tCout, sector = 0u;

			// determine the alpha-beta vectors from angle-magnitude
			float beta = MAX_AMPLITUDE*sinf( (float)j*PI_OVER_180 );
			float alpha = MAX_AMPLITUDE*cosf( (float)j*PI_OVER_180 );

			startTime = TIM12->CNT;
			foc_svm0(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector);
			stopTime = TIM12->CNT;
			xDebugStats.foc0Clocks += (uint32_t)((uint16_t)(stopTime - startTime));

			startTime = TIM12->CNT;
			foc_svm1(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector);
			stopTime = TIM12->CNT;
			xDebugStats.foc1Clocks += (uint32_t)((uint16_t)(stopTime - startTime));
			
			startTime = TIM12->CNT;
			foc_svm2(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector);
			stopTime = TIM12->CNT;
			xDebugStats.foc2Clocks += (uint32_t)((uint16_t)(stopTime - startTime));
			
			startTime = TIM12->CNT;
			foc_svm3(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector);
			stopTime = TIM12->CNT;
			xDebugStats.foc3Clocks += (uint32_t)((uint16_t)(stopTime - startTime));
			
			startTime = TIM12->CNT;
			foc_svm4(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector);
			stopTime = TIM12->CNT;
			xDebugStats.foc4Clocks += (uint32_t)((uint16_t)(stopTime - startTime));
			
			startTime = TIM12->CNT;
			foc_svm5(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector);
			stopTime = TIM12->CNT;
			xDebugStats.foc5Clocks += (uint32_t)((uint16_t)(stopTime - startTime));
		}
	}

	while ( 1 );
}
/*-----------------------------------------------------------*/