#include <math.h>
#include <arm_math.h> // for dsp

#include "foc.h"

// original implementation
void foc_svm0(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	uint32_t sector;

	if (beta >= 0.0f) {
		if (alpha >= 0.0f) {
			//quadrant I
			if (ONE_BY_SQRT3 * beta > alpha) {
				sector = 2;
			} else {
				sector = 1;
			}
		} else {
			//quadrant II
			if (-ONE_BY_SQRT3 * beta > alpha) {
				sector = 3;
			} else {
				sector = 2;
			}
		}
	} else {
		if (alpha >= 0.0f) {
			//quadrant IV5
			if (-ONE_BY_SQRT3 * beta > alpha) {
				sector = 5;
			} else {
				sector = 6;
			}
		} else {
			//quadrant III
			if (ONE_BY_SQRT3 * beta > alpha) {
				sector = 4;
			} else {
				sector = 5;
			}
		}
	}

	// PWM timings
	uint32_t tA, tB, tC;

	switch (sector) {

	// sector 1-2
	case 1: {
		// Vector on-times
		uint32_t t1 = (alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;
		uint32_t t2 = (TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tA = (PWMFullDutyCycle + t1 + t2) / 2;
		tB = tA - t1;
		tC = tB - t2;

		break;
	}

	// sector 2-3
	case 2: {
		// Vector on-times
		uint32_t t2 = (alpha + ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;
		uint32_t t3 = (-alpha + ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tB = (PWMFullDutyCycle + t2 + t3) / 2;
		tA = tB - t3;
		tC = tA - t2;

		break;
	}

	// sector 3-4
	case 3: {
		// Vector on-times
		uint32_t t3 = (TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;
		uint32_t t4 = (-alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tB = (PWMFullDutyCycle + t3 + t4) / 2;
		tC = tB - t3;
		tA = tC - t4;

		break;
	}

	// sector 4-5
	case 4: {
		// Vector on-times
		uint32_t t4 = (-alpha + ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;
		uint32_t t5 = (-TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tC = (PWMFullDutyCycle + t4 + t5) / 2;
		tB = tC - t5;
		tA = tB - t4;

		break;
	}

	// sector 5-6
	case 5: {
		// Vector on-times
		uint32_t t5 = (-alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;
		uint32_t t6 = (alpha - ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tC = (PWMFullDutyCycle + t5 + t6) / 2;
		tA = tC - t5;
		tB = tA - t6;

		break;
	}

	// sector 6-1
	case 6: {
		// Vector on-times
		uint32_t t6 = (-TWO_BY_SQRT3 * beta) * PWMFullDutyCycle;
		uint32_t t1 = (alpha + ONE_BY_SQRT3 * beta) * PWMFullDutyCycle;

		// PWM timings
		tA = (PWMFullDutyCycle + t6 + t1) / 2;
		tC = tA - t1;
		tB = tC - t6;

		break;
	}
	}

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}


// using math.h
void foc_svm1(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	/* Null = V0 SVPWM (reduces switching losses by up to 33%) */

	uint32_t sector;
	float angle, T1, T2, magnitude, temp, a;
	uint32_t tA, tB, tC;	// PWM timings

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__atan2.html#gaa7fbd2bb45df78860dd3a0293391096f
	// bound from 0 to 2*pi
	angle = atan2f(beta, alpha); // from math.h

	// convert from (-pi, pi) to (0, 2*pi) via branchless programming
	angle = angle + (angle < 0)*SIX_PI_OVER_3;

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__Euclidean.html#ga838f0a157454dfd95996691309c3828e
	magnitude = hypotf(alpha, beta); // from math.h
	a = fmodf(angle, PI_OVER_3); // from math.h

	// temporary variable for intermediate math
	temp = (float)PWMFullDutyCycle * magnitude;

	// future improvements could use onboard dsp for this (or lookup table)
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__sin.html#gae164899c4a3fc0e946dc5d55555fe541
	T1 = temp * sinf(PI_OVER_3 - a) * ONE_OVER_MAX_AMPLITUDE; // from math.h
	T2 = temp * sinf(a) * ONE_OVER_MAX_AMPLITUDE; // from math.h


	if (angle < PI_OVER_3) { // sector 1
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(T2);
		tC = (uint32_t)(0);
		sector = 1u;
	} else if (angle < TWO_PI_OVER_3) { // sector 2
		tA = (uint32_t)(T1);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(0);
		sector = 2u;
	} else if (angle < THREE_PI_OVER_3) { // sector 3
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(T2);
		sector = 3u;
	} else if (angle < FOUR_PI_OVER_3) { // sector 4
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1);
		tC = (uint32_t)(T1+T2);
		sector = 4u;
	} else if (angle < FIVE_PI_OVER_3) { // sector 5
		tA = (uint32_t)(T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1+T2);
		sector = 5u;
	} else { // sector 6
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1);
		sector = 6u;
	}

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}



// using utils_math.h
void foc_svm2(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	/* Null = V0 SVPWM (reduces switching losses by up to 33%) */

	uint32_t sector;
	float angle, T1, T2, magnitude, temp, a;
	uint32_t tA, tB, tC;	// PWM timings

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__atan2.html#gaa7fbd2bb45df78860dd3a0293391096f
	// bound from 0 to 2*pi
	angle = utils_fast_atan2(beta, alpha); // from utils_math.h (WARNING this is not as acurate as atan2f from math.h (off by as much as -0.33E-2), but it is faster..)

	// convert from (-pi, pi) to (0, 2*pi) via branchless programming
	angle = angle + (angle < 0)*SIX_PI_OVER_3;

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__Euclidean.html#ga838f0a157454dfd95996691309c3828e
	magnitude = NORM2_f(alpha, beta); // from utils_math.h
	a = fmodf(angle, PI_OVER_3); // from utils_math.h

	// temporary variable for intermediate math
	temp = (float)PWMFullDutyCycle * magnitude;

	// future improvements could use onboard dsp for this (or lookup table)
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__sin.html#gae164899c4a3fc0e946dc5d55555fe541
	T1 = temp * sinf(PI_OVER_3 - a) * ONE_OVER_MAX_AMPLITUDE; // from math.h
	T2 = temp * sinf(a) * ONE_OVER_MAX_AMPLITUDE; // from math.h


	if (angle < PI_OVER_3) { // sector 1
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(T2);
		tC = (uint32_t)(0);
		sector = 1u;
	} else if (angle < TWO_PI_OVER_3) { // sector 2
		tA = (uint32_t)(T1);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(0);
		sector = 2u;
	} else if (angle < THREE_PI_OVER_3) { // sector 3
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(T2);
		sector = 3u;
	} else if (angle < FOUR_PI_OVER_3) { // sector 4
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1);
		tC = (uint32_t)(T1+T2);
		sector = 4u;
	} else if (angle < FIVE_PI_OVER_3) { // sector 5
		tA = (uint32_t)(T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1+T2);
		sector = 5u;
	} else { // sector 6
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1);
		sector = 6u;
	}

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}


// using onboard dsp
void foc_svm3(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	/* Null = V0 SVPWM (reduces switching losses by up to 33%) */

	uint32_t sector;
	float angle, T1, T2, magnitude, temp, a;
	uint32_t tA, tB, tC;	// PWM timings

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__atan2.html#gaa7fbd2bb45df78860dd3a0293391096f
	// bound from 0 to 2*pi
	angle = atan2f(beta, alpha); // from math.h
	//(void)arm_atan2_f32(beta, alpha, &angle); // apparently this is only available in v1.10.0 of the cmsis lib, which I don't have

	// convert from (-pi, pi) to (0, 2*pi) via branchless programming
	angle = angle + (angle < 0)*SIX_PI_OVER_3;
	a = fmodf(angle, PI_OVER_3); // from math.h

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__Euclidean.html#ga838f0a157454dfd95996691309c3828e
	//magnitude = hypotf(alpha, beta); // from math.h
	//magnitude = arm_euclidean_distance_f32 (&alpha, &beta, 1u ); // not very accurate (or precise) for some reason...
	(void)arm_sqrt_f32(SQ(alpha)+SQ(beta), &magnitude);

	// temporary variable for intermediate math
	temp = (float)PWMFullDutyCycle * magnitude * ONE_OVER_MAX_AMPLITUDE;

	// future improvements could use onboard dsp for this (or lookup table)
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__sin.html#gae164899c4a3fc0e946dc5d55555fe541
	T1 = temp * arm_sin_f32(PI_OVER_3 - a); // from math.h
	T2 = temp * arm_sin_f32(a); // from math.h


	if (angle < PI_OVER_3) { // sector 1
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(T2);
		tC = (uint32_t)(0);
		sector = 1u;
	} else if (angle < TWO_PI_OVER_3) { // sector 2
		tA = (uint32_t)(T1);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(0);
		sector = 2u;
	} else if (angle < THREE_PI_OVER_3) { // sector 3
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(T2);
		sector = 3u;
	} else if (angle < FOUR_PI_OVER_3) { // sector 4
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1);
		tC = (uint32_t)(T1+T2);
		sector = 4u;
	} else if (angle <= FIVE_PI_OVER_3) { // sector 5 (this comparison is intentionally different than the rest)
		tA = (uint32_t)(T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1+T2);
		sector = 5u;
	} else { // sector 6
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1);
		sector = 6u;
	}

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}

// using onboard dsp w/ sector determination from the original implementation
void foc_svm4(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	/* Null = V0 SVPWM (reduces switching losses by up to 33%) */

	uint32_t sector;
	float angle, T1, T2, magnitude, temp, a;
	uint32_t tA, tB, tC;	// PWM timings

	if (beta >= 0.0f) {
		if (alpha >= 0.0f) {
			//quadrant I
			if (ONE_BY_SQRT3 * beta > alpha) {
				sector = 2;
			} else {
				sector = 1;
			}
		} else {
			//quadrant II
			if (-ONE_BY_SQRT3 * beta > alpha) {
				sector = 3;
			} else {
				sector = 2;
			}
		}
	} else {
		if (alpha >= 0.0f) {
			//quadrant IV
			if (-ONE_BY_SQRT3 * beta > alpha) {
				sector = 5;
			} else {
				sector = 6;
			}
		} else {
			//quadrant III
			if (ONE_BY_SQRT3 * beta > alpha) {
				sector = 4;
			} else {
				sector = 5;
			}
		}
	}
	
	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__atan2.html#gaa7fbd2bb45df78860dd3a0293391096f
	// bound from 0 to 2*pi
	angle = atan2f(beta, alpha); // from math.h
	//(void)arm_atan2_f32(beta, alpha, &angle); // apparently this is only available in v1.10.0 of the cmsis lib, which I don't have

	// convert from (-pi, pi) to (0, 2*pi) via branchless programming
	angle = angle + (angle < 0)*SIX_PI_OVER_3;
	a = fmodf(angle, PI_OVER_3); // from math.h

	// future improvements could use onboard dsp for this
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__Euclidean.html#ga838f0a157454dfd95996691309c3828e
	//magnitude = hypotf(alpha, beta); // from math.h
	//magnitude = arm_euclidean_distance_f32 (&alpha, &beta, 1u ); // not very accurate (or precise) for some reason...
	(void)arm_sqrt_f32(SQ(alpha)+SQ(beta), &magnitude);

	// temporary variable for intermediate math
	temp = (float)PWMFullDutyCycle * magnitude * ONE_OVER_MAX_AMPLITUDE;

	// future improvements could use onboard dsp for this (or lookup table)
	// https://www.keil.com/pack/doc/CMSIS/DSP/html/group__sin.html#gae164899c4a3fc0e946dc5d55555fe541
	T1 = temp * arm_sin_f32(PI_OVER_3 - a); // from math.h
	T2 = temp * arm_sin_f32(a); // from math.h

	switch(sector) {
	case 1: {
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(T2);
		tC = (uint32_t)(0);
		break;
	}
	
	case 2: {
		tA = (uint32_t)(T1);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(0);
		break;
	}
	
	case 3: {
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(T2);
		break;
	}
	
	case 4: {
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1);
		tC = (uint32_t)(T1+T2);
		break;
	}
	
	case 5: {
		tA = (uint32_t)(T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1+T2);
		break;
	}
	
	case 6: {
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1);
		break;
	}
	}


	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}


#define USE_ONBOARD_DSP
// using onboard dsp w/ sector determination from the original implementation
void foc_svm5(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, uint32_t *svm_sector) {
	/* Null = V0 SVPWM (reduces switching losses by up to 33%) */

	uint32_t sector;
	float angle, T1, T2, magnitude, temp, a;
	uint32_t tA, tB, tC;	// PWM timings

	// get angle
#ifdef USE_ONBOARD_DSP
	// requires at least v1.10.0 of the cmsis lib
	// https://github.com/ARM-software/CMSIS-DSP/releases
	(void)arm_atan2_f32(beta, alpha, &angle);
#else
	angle = atan2f(beta, alpha); // from math.h
#endif

	// convert from (-pi, pi) to (0, 2*pi) via branchless programming
	angle = angle + (angle < 0)*SIX_PI_OVER_3;
	a = fmodf(angle, PI_OVER_3); // from math.h
	sector = ((uint32_t)(a / PI_OVER_3) % 6u) + 1;

	// determine magnitude
#ifdef USE_ONBOARD_DSP
	(void)arm_sqrt_f32(SQ(alpha)+SQ(beta), &magnitude); // using onbaord dsp
#else
	magnitude = hypotf(alpha, beta); // from math.h
#endif

	// temporary variable for intermediate math
	temp = (float)PWMFullDutyCycle * magnitude * ONE_OVER_MAX_AMPLITUDE;

#ifdef USE_ONBOARD_DSP
	T1 = temp * arm_sin_f32(PI_OVER_3 - a); // using arm dsp
	T2 = temp * arm_sin_f32(a); // using arm dsp
#else
	T1 = temp * sinf(PI_OVER_3 - a) * ONE_OVER_MAX_AMPLITUDE; // from math.h
	T2 = temp * sinf(a) * ONE_OVER_MAX_AMPLITUDE; // from math.h
#endif

	
	switch(sector) {
	case 1: {
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(T2);
		tC = (uint32_t)(0);
		break;
	}
	
	case 2: {
		tA = (uint32_t)(T1);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(0);
		break;
	}
	
	case 3: {
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1+T2);
		tC = (uint32_t)(T2);
		break;
	}
	
	case 4: {
		tA = (uint32_t)(0);
		tB = (uint32_t)(T1);
		tC = (uint32_t)(T1+T2);
		break;
	}
	
	case 5: {
		tA = (uint32_t)(T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1+T2);
		break;
	}
	
	case 6: {
		tA = (uint32_t)(T1+T2);
		tB = (uint32_t)(0);
		tC = (uint32_t)(T1);
		break;
	}
	}

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = sector;
}