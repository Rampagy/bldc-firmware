#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define SQRT3_BY_2				(0.866025403784439f)	// max alpha and beta magnitude
#define PI_OVER_180				(0.01745329251f)		// pi/180

typedef enum
{
	ars_svm_e, // default (previous implementation)
	null_v0_e, // flat bottom
	null_v7_e, // flat top
	v7_odd_v0_even_e, // v7 in [1,3,5] v0 in [2,4,6]
	v0_odd_v7_even_e, // v0 in [1,3,5] v7 in [2,4,6]
} commutation_technique_t;


const uint8_t sector_LUT[6] = {6u, 2u, 1u, 4u, 5u, 3u};


void foc_svm(float alpha, float beta, uint32_t PWMFullDutyCycle,
				uint32_t* tAout, uint32_t* tBout, uint32_t* tCout, 
				uint32_t *svm_sector, commutation_technique_t comm_tech) {
	uint8_t sector;
	uint32_t tA, tB, tC, N;	// PWM timings
	float T0, T1, T2, a, b, c, temp = 0.0f;

	a = SQRT3_BY_2*alpha - 0.5f*beta;
	b = beta;

	temp = (float)PWMFullDutyCycle/SQRT3_BY_2;

	a *= temp;
	b *= temp;
	c = -1.0f*(a+b); // kirchhoff current law (current in = current out)

	N = (((int32_t)a)>=0) + 2u*(((int32_t)b) >= 0) + 4u*(((int32_t)c) >= 0);
	N = (N > 6) ? 3 : N; // if a, b, c are all zero set it to sector 1 (alpha & beta are zero)
	sector = sector_LUT[N-1];

	switch(sector) {
	case 1: {
		T1 = a;
		T2 = b;
		T0 = (float)PWMFullDutyCycle - T1 - T2;

		switch (comm_tech) {
		case ars_svm_e:
		default:
			tA = (uint32_t)(T1+T2+0.5f*T0);
			tB = (uint32_t)(T2+0.5f*T0);
			tC = (uint32_t)(0.5f*T0);
			break;

		case v0_odd_v7_even_e:
		case null_v0_e:
			tA = (uint32_t)(T1+T2);
			tB = (uint32_t)(T2);
			tC = (uint32_t)(0);
			break;

		case v7_odd_v0_even_e:
		case null_v7_e:
			tA = (uint32_t)(PWMFullDutyCycle);
			tB = (uint32_t)(T0+T2);
			tC = (uint32_t)(T0);
			break;
		}
		break;
	}

	case 2: {
		T1 = -c;
		T2 = -a;
		T0 = (float)PWMFullDutyCycle - T1 - T2;

		switch (comm_tech) {
		case ars_svm_e:
		default:
			tA = (uint32_t)(T1+0.5f*T0);
			tB = (uint32_t)(T1+T2+0.5f*T0);
			tC = (uint32_t)(0.5f*T0);
			break;

		case v7_odd_v0_even_e:
		case null_v0_e:
			tA = (uint32_t)(T1);
			tB = (uint32_t)(T1+T2);
			tC = (uint32_t)(0);
			break;

		case v0_odd_v7_even_e:
		case null_v7_e:
			tA = (uint32_t)(T0+T1);
			tB = (uint32_t)(PWMFullDutyCycle);
			tC = (uint32_t)(T0);
			break;
		}
		break;
	}

	case 3: {
		T1 = b;
		T2 = c;
		T0 = (float)PWMFullDutyCycle - T1 - T2;

		switch (comm_tech) {
		case ars_svm_e:
		default:
			tA = (uint32_t)(0.5f*T0);
			tB = (uint32_t)(T1+T2+0.5f*T0);
			tC = (uint32_t)(T2+0.5f*T0);
			break;

		case v0_odd_v7_even_e:
		case null_v0_e:
			tA = (uint32_t)(0);
			tB = (uint32_t)(T1+T2);
			tC = (uint32_t)(T2);
			break;

		case v7_odd_v0_even_e:
		case null_v7_e:
			tA = (uint32_t)(T0);
			tB = (uint32_t)(PWMFullDutyCycle);
			tC = (uint32_t)(T0+T2);
			break;
		}
		break;
	}

	case 4: {
		T1 = -a;
		T2 = -b;
		T0 = (float)PWMFullDutyCycle - T1 - T2;

		switch (comm_tech) {
		case ars_svm_e:
		default:
			tA = (uint32_t)(0.5f*T0);
			tB = (uint32_t)(T1+0.5f*T0);
			tC = (uint32_t)(T1+T2+0.5f*T0);
			break;

		case v7_odd_v0_even_e:
		case null_v0_e:
			tA = (uint32_t)(0);
			tB = (uint32_t)(T1);
			tC = (uint32_t)(T1+T2);
			break;

		case v0_odd_v7_even_e:
		case null_v7_e:
			tA = (uint32_t)(T0);
			tB = (uint32_t)(T0+T1);
			tC = (uint32_t)(PWMFullDutyCycle);
			break;
		}
		break;
	}

	case 5: {
		T1 = c;
		T2 = a;
		T0 = (float)PWMFullDutyCycle - T1 - T2;

		switch (comm_tech) {
		case ars_svm_e:
		default:
			tA = (uint32_t)(T2+0.5f*T0);
			tB = (uint32_t)(0.5f*T0);
			tC = (uint32_t)(T1+T2+0.5f*T0);
			break;

		case v0_odd_v7_even_e:
		case null_v0_e:
			tA = (uint32_t)(T2);
			tB = (uint32_t)(0);
			tC = (uint32_t)(T1+T2);
			break;

		case v7_odd_v0_even_e:
		case null_v7_e:
			tA = (uint32_t)(T0+T2);
			tB = (uint32_t)(T0);
			tC = (uint32_t)(PWMFullDutyCycle);
			break;
		}
		break;
	}

	case 6: {
		T1 = -b;
		T2 = -c;
		T0 = (float)PWMFullDutyCycle - T1 - T2;

		switch (comm_tech) {
		case ars_svm_e:
		default:
			tA = (uint32_t)(T1+T2+0.5f*T0);
			tB = (uint32_t)(0.5f*T0);
			tC = (uint32_t)(T1+0.5f*T0);
			break;

		case v7_odd_v0_even_e:
		case null_v0_e:
			tA = (uint32_t)(T1+T2);
			tB = (uint32_t)(0);
			tC = (uint32_t)(T1);
			break;

		case v0_odd_v7_even_e:
		case null_v7_e:
			tA = (uint32_t)(PWMFullDutyCycle);
			tB = (uint32_t)(T0);
			tC = (uint32_t)(T0+T1);
			break;
		}
		break;
	}
	}

	*tAout = tA;
	*tBout = tB;
	*tCout = tC;
	*svm_sector = (uint32_t)sector;
}


int main (int argc, char* args[]) {
	(void)argc;
	(void)args;

	float throttle_cmd = 0.0f;

	FILE* ars_stream = fopen("svm_ars.csv", "w");
	FILE* null_v0_stream = fopen("svm_null_v0.csv", "w");
	FILE* null_v7_stream = fopen("svm_null_v7.csv", "w");
	FILE* v7_odd_v0_even_stream = fopen("svm_v7_odd_v0_even.csv", "w");
	FILE* v0_odd_v7_even_stream = fopen("svm_v0_odd_v7_even.csv", "w");

	for (float i = 0.0f; i < 1800.0f; i += 0.05f) {
		uint32_t tAout, tBout, tCout, sector = 0u;
		float alpha, beta = 0.0f;

		// determine the alpha-beta vectors from angle-magnitude
		beta = throttle_cmd*sinf( i*PI_OVER_180 );
		alpha = throttle_cmd*cosf( i*PI_OVER_180 );

		// magnitude is always 1000 (for this test)
		foc_svm(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector, ars_svm_e);
		fprintf(ars_stream, "%d,%d,%d,%d\n", (int)tAout, (int)tBout, (int)tCout, (int)sector); // write to file

		foc_svm(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector, null_v0_e);
		fprintf(null_v0_stream, "%d,%d,%d,%d\n", (int)tAout, (int)tBout, (int)tCout, (int)sector); // write to file

		foc_svm(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector, null_v7_e);
		fprintf(null_v7_stream, "%d,%d,%d,%d\n", (int)tAout, (int)tBout, (int)tCout, (int)sector); // write to file

		foc_svm(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector, v7_odd_v0_even_e);
		fprintf(v7_odd_v0_even_stream, "%d,%d,%d,%d\n", (int)tAout, (int)tBout, (int)tCout, (int)sector); // write to file

		foc_svm(alpha, beta, (uint32_t)1000u, &tAout, &tBout, &tCout, &sector, v0_odd_v7_even_e);
		fprintf(v0_odd_v7_even_stream, "%d,%d,%d,%d\n", (int)tAout, (int)tBout, (int)tCout, (int)sector); // write to file

		if (throttle_cmd < SQRT3_BY_2) {
			throttle_cmd += 0.00005f;
		}
		else {
			throttle_cmd = SQRT3_BY_2;
		}
	}

	(void)fclose( ars_stream );
	(void)fclose( null_v0_stream );
	(void)fclose( null_v7_stream );
	(void)fclose( v7_odd_v0_even_stream );
	(void)fclose( v0_odd_v7_even_stream );
}