#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#include <stdint.h>
#include <math.h>
#include <arm_math.h> // for dsp

#define M_PI 3.14159265358979323846


#define SQ(x)				((x) * (x))
#define NORM2_f(x,y)		(sqrtf(SQ(x) + SQ(y)))
#define UTILS_IS_NAN(x)		((x) != (x))
#define UTILS_NAN_ZERO(x)	(x = UTILS_IS_NAN(x) ? 0.0 : x)

float utils_fast_atan2(float y, float x);
float fmodf_custom( float x, float y );


#endif