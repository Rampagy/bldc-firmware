#include <stdint.h>
#include <math.h>
#include <stdio.h>


#define ONE_OVER_MAX_AMPLITUDE	(1.0f/0.866025403784439f)
#define PI_OVER_3				(1.0471975512f)  // 60 degrees
#define TWO_PI_OVER_3			(2.0f*PI_OVER_3) // 120 degrees
#define THREE_PI_OVER_3			(3.0f*PI_OVER_3) // 180 degrees
#define FOUR_PI_OVER_3			(4.0f*PI_OVER_3) // 240 degrees
#define FIVE_PI_OVER_3			(5.0f*PI_OVER_3) // 300 degrees
#define SIX_PI_OVER_3			(6.0f*PI_OVER_3) // 360 degrees

// https://opensource.apple.com/source/Libm/Libm-315/Source/ARM/fmodf.c.auto.html
float fmodf_custom( float x, float y )
{
	union{ float f; uint32_t u; }ux = {x};
	union{ float f; uint32_t u; }uy = {y};

	uint32_t absx = ux.u & 0x7fffffffU;
	uint32_t absy = uy.u & 0x7fffffffU;
	if( absx - 1U >= 0x7f7fffffU || absy - 1U >= 0x7f7fffffU )
	{
		float fabsx = __builtin_fabsf(x);	
		float fabsy = __builtin_fabsf(y);

		// deal with NaN
		if( x != x || y != y )
			return x + y;

		//x = Inf or y = 0, return Invalid per IEEE-754
		if( fabsx == __builtin_inff() || 0.0f == y )
		{
#ifdef ARMLIBM_SET_FLAGS
			return required_add_float( __builtin_inff(), -__builtin_inff() );	//set invalid
#else
			return __builtin_nan("");
#endif
		}

		//handle trivial case
		if( fabsy == __builtin_inff() || 0.0f == x )
		{
#ifdef ARMLIBM_SET_FLAGS
			required_add_float( fabsx, 0.0f );	// signal underflow (that is, no flag set,
												// but exception occurs if unmasked) if x is denorm
#endif
			return x;
		}
	}
 
	if( absy >= absx )
	{
		if( absy == absx )
		{
			ux.u &= 0x80000000;
			return ux.f;
		}
		return x;
	}
 
	//extract exponent, mantissa
	int32_t expx = absx >> 23;
	int32_t expy = absy >> 23;
	int32_t sx = absx & 0x007fffff;			// x significand
	int32_t sy = absy & 0x007fffff;			// y significand
	if( 0 == expx )		//denormal x
	{
		//calculate shift to move leading bit to exponents field
		uint32_t shift = __builtin_clzl( absx ) - (8*sizeof(long) - 24);
		sx <<= shift;	//do the shift
		expx = 1-shift;	//adjust the biased exponent accordingly, -1 here for the implicit shift to make implicit denorm leading bit explicit
	}
	if( 0 == expy )		//denormal y
	{
		//calculate shift to move leading bit to exponents field
		uint32_t shift = __builtin_clzl( absy ) - (8*sizeof(long) - 24);
		sy <<= shift;	//do the shift
		expy = 1-shift;	//adjust the biased exponent accordingly, -1 here for the implicit shift to make implicit denorm leading bit explicit
	}
	//make implicit bits explicit
	sx |= 0x00800000;
	sy |= 0x00800000;
	
		
	// The idea here is to iterate until the exponent of x is the same as y
	// Calculate the number of bits we can safely shave off before we reach parity
	int32_t idiff = expx - expy;	
	int32_t shift = 0;
	int32_t mask;	

	//since idiff is always >= 0 here, it is safe to enter
	//We always shift by shift+1 here, so in the first iteration, the worst that can happen
	do
	{
		// move the leading bit of x to the 23rd bit
		sx <<= shift;

		//Keep track that we did that
		idiff += ~shift;			// idiff -= shift + 1, +1 for the shift below
	
		//The two values both have the 23rd bit set as the leading bit  (24 bit unsigned number)
		//subtract one from the other. This gives us a signed 23 bit number in the range { -0x00ffffff ... 0x00ffffff }
		sx -= sy;		

		//record the sign
		mask = sx >> 31;

		//shift x left 1 to restore a 24 bit quantity
		sx += sx;		//this is potentially 1 shift too far, which we patch up later

		//if negative, we add back sy to restore to postiveness. This is the same as x - y + 0.5y = - 0.5y 
		// instead of x-y. We've effectively hoisted the subtract that would have appeared in the next loop
		// iteration here, and saved a test+branch in exchange for a shift and and. (The add happens either way.)
		sx += sy & mask;	

		//figure out how far we need to shift sx to get the leading bit into the 23rd position
		shift = __builtin_clzl( sx ) - (8*sizeof(long) - 24);
	}
	while( idiff >= shift && sx != 0);
	
	//We may have gone too far 
	if( idiff < 0 )	
	{
		//If so, rewind a little.
		// if sx - sy succeeded, it was the right thing to do, the only thing we did wrong was the shift
		// if sx - sy yielded a negative number, then we shouldn't have done that either
		sx += sy & mask;
		sx >>= 1;
//debug code to make sure we didn't do something dumb here
		idiff = 0;
	}

	//We may have some bits left over to shift in. 
	sx <<= idiff;
	
//convert back to float
	if( 0 == sx )
	{
		ux.u &= 0x80000000;
		return ux.f;
	}
			
	//figure out how far we need to shift in order to move leading bit into exponent field
	shift = __builtin_clzl( sx ) - (8*sizeof(long) - 24);
	sx <<= shift;				// move leading bit into exponent field
	expy -= shift;				// account for the shift in the exponent
	sx &= 0x007fffff;			// remove leading bit
	sx |= ux.u & 0x80000000;	//or in sign
	if( expy > 0 )
	{
		ux.u = sx | (expy << 23);
		return ux.f;
	}
	
	//denormal
	expy += 126;
	ux.u = sx | (expy << 23);
	return ux.f * 0x1.0p-126f;
}


void foc_svm(float alpha, float beta, uint32_t PWMFullDutyCycle,
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


int main (int argc, char* args[]) {
	(void)argc;
	(void)args;

	FILE* stream = fopen("vedderb_foc.csv", "w");
	for (int i = 0; i < 360; ++i) {
		uint32_t tAout, tBout, tCout, sector = 0u;
		float alpha, beta = 0.0f;

		// determine the alpha-beta vectors from angle-magnitude
		// magnitude is always 1000
		beta = 0.866025403784439f*sinf( (float)i*0.01745329251f );
		alpha = 0.866025403784439f*cosf( (float)i*0.01745329251f );

		// do the math
		foc_svm(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector );

		// write to file
		fprintf(stream, "%d,%d,%d\n", tAout, tBout, tCout);
	}
	(void)fclose( stream );
}