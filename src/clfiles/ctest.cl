#include "complex.h"
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void graph( __global float *a, __global float *b, __constant cplx *op, const int opnum, const double zoom, const float zoomc, const unsigned int n) {
	int id = get_global_id(0);

	if (id < 28) {
		cplx sample = (cplx)(-1.23, 2.01);
		switch(id) {
			case 0:
				sample = cadd(sample, sample);
				break;

			case 1:
				sample = csub(sample, sample);
				break;

			case 2:
				sample = cmult(sample, sample);
				break;

			case 3:
				sample = cdiv(sample, (cplx)(12.3, -3));
				break;

			case 4:
				sample = (cplx)(creal(sample), 0);
				break;

			case 5:
				sample = (cplx)(cimag(sample), 0);
				break;

			case 6:
				sample = conj(sample);
				break;

			case 7:
				sample = (cplx)(carg(sample), 0);
				break;

			case 8:
				sample = cproj(sample);
				break;

			case 9:
				sample = (cplx)(cabs(sample), 0);
				break;

			case 10:
				sample = cexp(sample);
				break;

			case 11:
				sample = clog(sample);
				break;

			case 12:
				sample = cpow(sample, sample);
				break;

			case 13:
				sample = csqrt(sample);
				break;

			case 14:
				sample = csin(sample);
				break;

			case 15:
				sample = ccos(sample);
				break;

			case 16:
				sample = ctan(sample);
				break;

			case 17:
				sample = csinh(sample);
				break;

			case 18:
				sample = ccosh(sample);
				break;

			case 19:
				sample = ctanh(sample);
				break;

			case 20:
				sample = casinh(sample);
				break;

			case 21:
				sample = cacosh(sample);
				break;

			case 22:
				sample = catanh(sample);
				break;

			case 23:
				sample = casin(sample);
				break;

			case 24:
				sample = cacos(sample);
				break;

			case 25:
				sample = catan(sample);
				break;

			case 26:
				sample = (cplx)(1);
				break;

			case 27:
				sample = (cplx)(1);
				break;

		}
		vstore3((float3)(sample.x, sample.y, 1), 0, &b[id*3]);
	}
}
