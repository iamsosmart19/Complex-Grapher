#include "stack.h"
//#include "complex.h"
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

cplx evalFunc(__constant cplx *op, int opnum, cplx val);
float3 hsv2rgb(float H, float S, float V);

__kernel void graph( __global float *a, __global float *b, __constant cplx *op, const int opnum, const double zoom, const float zoomc, const unsigned int n) {
	int id = get_global_id(0);

	if (id < n) {
		float2 input = vload2(0, &a[id*2]);
		cplx ret = evalFunc(op, opnum, (cplx)(zoom*input.x, zoom*input.y));
		float3 DBG = (float3)(ret.x, ret.y, 1);
		float3 HSV = (float3)((float)carg(ret), 1.0 - pow(zoomc, (float)cabs(ret)), 1.0);
		float3 RGB = hsv2rgb(HSV.x, HSV.y, HSV.z);
		vstore3(RGB, 0, &b[id*3]);
		//vstore3(DBG, 0, &b[id*3]);
	}
}

cplx evalFunc(__constant cplx *op, int opnum, cplx val) {
	stack s;
	initStack(&s);
	cplx oprnd1, oprnd2;
	for(int i = 0; i < opnum; i++) {
		if( op[i].y == DBL_MAX) {
			if( op[i].x == DBL_MAX) {
				s_push(&s, val);
			}
			else {
				oprnd1 = s_pop(&s);
				switch((int)op[i].x) {
					case 0:
						s_push(&s, csqrt(oprnd1));
						break;

					case 1:
						oprnd2 = s_pop(&s);
						s_push(&s, cpow(oprnd2, cdiv((cplx)(1,0), oprnd1)));
						break;

					case 2:
						s_push(&s, clog(oprnd1));
						break;

					case 3:
						oprnd2 = s_pop(&s);
						s_push(&s, cdiv(clog(oprnd1), clog(oprnd2)));
						break;

					case 4:
						s_push(&s, cabs(oprnd1));
						break;

					case 5:
						s_push(&s, (cplx)(floor(oprnd1.x), floor(oprnd1.y)));
						break;

					case 6:
						s_push(&s, (cplx)(ceil(oprnd1.x), ceil(oprnd1.y)));
						break;

					case 7:
						s_push(&s, casin(oprnd1));
						break;

					case 8:
						s_push(&s, cacos(oprnd1));
						break;

					case 9:
						s_push(&s, catan(oprnd1));
						break;

					case 10:
						s_push(&s, csinh(oprnd1));
						break;

					case 11:
						s_push(&s, ccosh(oprnd1));
						break;

					case 12:
						s_push(&s, ctanh(oprnd1));
						break;

					case 13:
						s_push(&s, ccosh(cdiv((cplx)(1,0), oprnd1)));
						break;

					case 14:
						s_push(&s, csinh(cdiv((cplx)(1,0), oprnd1)));
						break;

					case 15:
						s_push(&s, ctanh(cdiv((cplx)(1,0), oprnd1)));
						break;

					case 16:
						s_push(&s, csin(oprnd1));
						break;

					case 17:
						s_push(&s, ccos(oprnd1));
						break;

					case 18:
						s_push(&s, ctan(oprnd1));
						break;

					case 19:
						s_push(&s, ccos(cdiv((cplx)(1,0), oprnd1)));
						break;

					case 20:
						s_push(&s, csin(cdiv((cplx)(1,0), oprnd1)));
						break;

					case 21:
						s_push(&s, ctan(cdiv((cplx)(1,0), oprnd1)));
						break;

					default:
						s_push(&s, oprnd1);
						break;
				}
			}
		}
		else if( op[i].y == -DBL_MAX ) {
			switch((int)op[i].x) {
				case 0:
					s_push(&s, cadd(s_pop(&s), s_pop(&s)) );
					break;

				case 1:
					oprnd1 = s_pop(&s);
					oprnd2 = s_pop(&s);
					s_push(&s, csub(oprnd2, oprnd1));
					break;

				case 2:
					s_push(&s, cmult(s_pop(&s), s_pop(&s)) );
					break;

				case 3:
					oprnd1 = s_pop(&s);
					oprnd2 = s_pop(&s);
					s_push(&s, cdiv(oprnd2, oprnd1) );
					// return oprnd1;
					// return oprnd2;
					// return cdiv(oprnd2, oprnd1);
					break;

				case 4:
					oprnd1 = s_pop(&s);
					oprnd2 = s_pop(&s);
					s_push(&s, cpow(oprnd2, oprnd1) );
					// return cpow(oprnd2, oprnd1);
					break;

				case 5:
					s_push(&s, csub((cplx)(0, 0), s_pop(&s)) );
					break;
			}
		}
		else {
			s_push(&s, op[i]);
		}
	}
	return (cplx)(s_pop(&s));
	// return cpow((cplx)(M_E, 0), (cplx)(1.5, 1.5));
	// return cpow((cplx)(M_E, 0), val );
}

float3 hsv2rgb(float H, float S, float V) {
	if(H < -M_PI || H > M_PI + 0.001) {
		return (float3)(0, 0, 0);
	}
		
	if(S < 0 || S > 1) {
		return (float3)(0, 0, 0);
	}

	if(V < 0 || V > 1) {
		return (float3)(0, 0, 0);
	}
	float C = S * V;
	float Hs = ((H * 180)/M_PI) > 0 ? (H*180)/M_PI: 360 + (H*180)/M_PI;
	float X = C * (1 - fabs(fmod(Hs/ 60.0, 2) - 1));
	float m = V - C;
	float Rs, Gs, Bs;

	if(H >= 0 && H < M_PI/3) {
		Rs = C;
		Gs = X;
		Bs = 0;	
	}
	else if(H >= M_PI/3 && H < 2*M_PI/3) {	
		Rs = X;
		Gs = C;
		Bs = 0;	
	}
	else if(H >= 2*M_PI/3 && H < M_PI) {
		Rs = 0;
		Gs = C;
		Bs = X;	
	}
	else if(H >= -M_PI && H < -2*M_PI/3) {
		Rs = 0;
		Gs = X;
		Bs = C;	
	}
	else if(H >= -2*M_PI/3 && H < -M_PI/3) {
		Rs = X;
		Gs = 0;
		Bs = C;	
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;	
	}

	return (float3)(Rs+m, Gs+m, Bs+m);
}
