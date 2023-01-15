#ifndef FOC_H
#define FOC_H

#include <stdint.h>
#include "utils_math.h"

/* Alternating-Reversing Sequence defines */
//#define ONE_BY_SQRT2			(0.7071067811865475)
#define ONE_BY_SQRT3			(0.57735026919)
#define TWO_BY_SQRT3			(2.0f * 0.57735026919)

/* Null=V0 defines */
#define MAX_AMPLITUDE			(0.866025403784439f)	// max alpha and beta magnitude
#define ONE_OVER_MAX_AMPLITUDE	(1.0f/MAX_AMPLITUDE)	// reciprocal of max alpha and beta magnitude
#define PI_OVER_180				(0.01745329251f)		// pi/180

#define PI_OVER_3				(1.0471975512f)		// 60 degrees
#define TWO_PI_OVER_3			(2.0f*PI_OVER_3)	// 120 degrees
#define THREE_PI_OVER_3			(3.0f*PI_OVER_3)	// 180 degrees
#define FOUR_PI_OVER_3			(4.0f*PI_OVER_3)	// 240 degrees
#define FIVE_PI_OVER_3			(5.0f*PI_OVER_3)	// 300 degrees
#define SIX_PI_OVER_3			(6.0f*PI_OVER_3)	// 360 degrees


void foc_svm0(float alpha, float beta, uint32_t PWMFullDutyCycle, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);
void foc_svm1(float alpha, float beta, uint32_t PWMFullDutyCycle, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);
void foc_svm2(float alpha, float beta, uint32_t PWMFullDutyCycle, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);
void foc_svm3(float alpha, float beta, uint32_t PWMFullDutyCycle, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);
void foc_svm4(float alpha, float beta, uint32_t PWMFullDutyCycle, uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector);

#endif