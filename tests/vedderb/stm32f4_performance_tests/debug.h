#ifndef DEBUG_H
#define DEBUG_H

/* Peripheral includes. */
#include "stm32f4xx_tim.h"

/*-----------------------------------------------------------*/

typedef struct
{
    uint32_t foc0Clocks;
    uint32_t foc1Clocks;
    uint32_t foc2Clocks;
    uint32_t foc3Clocks;
	uint32_t foc4Clocks;
	uint32_t foc5Clocks;
	uint32_t foc6Clocks;
} xDebugStats_t;

/*-----------------------------------------------------------*/

void vInitDebug( void );

#endif