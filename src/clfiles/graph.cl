/* #include "complex.h" */
typedef double2 cplx;

//ELEMENTARY FUNCTIONS
inline cplx cadd(cplx n, cplx m) {
	return (cplx)(n.x + m.x, n.y + m.y);
}

inline cplx csub(cplx n, cplx m) {
	return (cplx)(n.x - m.x, n.y - m.y);
}

inline cplx cmult(cplx n, cplx m) {
	return (cplx)(n.x * m.x - n.y * m.y, n.x * m.y + n.y * m.x);
}

inline cplx cdiv(cplx m, cplx n) {
	return (cplx)( (n.x*m.x + n.y*m.y)/(pow(n.x, 2) + pow(n.y, 2)), (n.x*m.y - n.y*m.x)/(pow(n.x, 2) + pow(n.y, 2)) );
}

inline double creal(cplx n) {
	return n.x;
}

inline double cimag(cplx n) {
	return n.y;
}

inline cplx conj(cplx n) {
	return (cplx)(n.x, 0-n.y);
}

inline double carg(cplx n) {
	if(n.y != 0) {
		return 2 * atan((sqrt(pow(n.x, 2) + pow(n.y, 2)) - n.x)/(n.y));
	}
	else if(n.x > 0 && n.y == 0) {
		return 0;
	}
	else if(n.x < 0 && n.y == 0) {
		return M_PI;
	}
	return 0;
	/* if(n.x != 0) { */
	/* 	return atan(n.y/n.0); */
	/* } */
	/* return 0; */
}

inline cplx cproj(cplx n) {
	if(n.x != INFINITY && n.y != INFINITY) {
		return n;
	}
	return (cplx)(INFINITY, 0);
}

inline double cabs(cplx n) {
	return sqrt(pow(n.x, 2) + pow(n.y, 2));
}

//EXPONENTIAL FUNCTIONS

inline cplx cexp(cplx n) {
	return (cplx)(exp(n.x)*cos(n.y), exp(n.x)*sin(n.y));
}

inline cplx clog(cplx n) {
	return(cplx)(log(sqrt(pow(n.x, 2) + pow(n.y, 2))), carg(n));
}

inline cplx cpow(cplx m, cplx n) {
	return cexp(cmult(n, clog(m)));
}

inline cplx csqrt(cplx n) {
	return (cplx)( sqrt(cabs(n))*(cos(carg(n)/2.0)), sqrt(cabs(n))*(sin(carg(n)/2.0)));
	/* return (cplx)(sqrt((cabs(n) + n.x)/2.0), sqrt((cabs(n) - n.x)/2.0)); */
}

inline cplx csin(cplx n) {
	return (cplx)(sin(n.x) * cosh(n.y), cos(n.x) * sinh(n.y));
}

inline cplx ccos(cplx n) {
	return (cplx)(cos(n.x) * cosh(n.y), -sin(n.x) * sinh(n.y));
	/* return ccosh(cmult((cplx)(0,1), n)); */
	/* return cdiv(cadd(cexp(cmult((cplx)(1,0), n)), cexp(cmult((cplx)(-1,0), n))), (cplx)(2,0)); */
}

inline cplx ctan(cplx n) {
	/* return cmult(csub((cplx)(0,0), (cplx)(0,1)), ctanh(cmult((cplx)(0, 1), n)) ); */
	return cdiv(csin(n), ccos(n));
}

inline cplx csinh(cplx n) {
	return (cplx)(sinh(n.x)*cos(n.y), cosh(n.x)*sin(n.y));
}

inline cplx ccosh(cplx n) {
	return (cplx)(cosh(n.x)*cos(n.y), sinh(n.x)*sin(n.y));
}

inline cplx ctanh(cplx n) {
	return cdiv( csub(cexp(n), cexp(csub((cplx)(0, 0), n))), cadd(cexp(n), cexp(csub((cplx)(0, 0), n))) );
}

inline cplx casinh(cplx n) {
	return clog(cadd(n, csqrt(cadd((cplx)(1, 0), cpow(n, (cplx)(2,0)))) ) );
	/* return clog(cadd(n, cmult(csqrt(cadd(n, (cplx)(0,1))), csqrt(csub(n, (cplx)(0, 1)))))); */
}

inline cplx cacosh(cplx n) {
	return clog(cadd(n, cmult(csqrt(cadd(n, (cplx)(1,0))), csqrt(csub(n, (cplx)(1, 0))))));
}

inline cplx catanh(cplx n) {
	return cdiv(csub(clog(cadd(n, (cplx)(1, 0))), clog(csub((cplx)(1, 0), n))), (cplx)(2, 0));
}

inline cplx casin(cplx n) {
	return cmult( (cplx)(0, -1), casinh(cmult((cplx)(0, 1), n))); 
	/* return carg(cadd(cmult((cplx)(0,1), n), csqrt(csub((cplx)(1,0), cpow(n, (cplx)(2,0)) )) )); */
}

inline cplx cacos(cplx n) {
	/* return cmult( (cplx)(0, -1), cacosh(cmult((cplx)(0, 1), n))); */ 
	/* return carg(cadd(n, cmult((cplx)(0,1), csqrt(csub((cplx)(1,0), cpow(n, (cplx)(2,0)) )) ))); */
	return cadd(cdiv((cplx)(M_PI,0), (cplx)(2,0)), cmult((cplx)(0,1), clog(cadd(cmult((cplx)(0,1), n), csqrt(csub((cplx)(1,0), cpow(n, (cplx)(2,0)) ))))));
}

inline cplx catan(cplx n) {
	return cmult( (cplx)(0, -1), catanh(cmult((cplx)(0, 1), n)));
}
//#include "stack.h"
typedef struct stackStruct {
	int cur;
	cplx arr[128];
} stack;

inline void initStack(stack *s) {
	s->cur = 0;
}

inline cplx s_pop(stack *s) {
	return s->arr[s->cur--];
}

inline void s_push(stack *s, cplx val) {
	s->arr[++(s->cur)] = val;
}
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

cplx evalFunc(cplx z);
float3 hsv2rgb(float H, float S, float V);

__kernel void graph( __global float *a, __global float *b, __constant cplx *op, const int opnum, const double zoom, const float zoomc, const float2 offset, const unsigned int n, const unsigned int gridOn, const unsigned int shadOn, const unsigned axesOn) {
	int id = get_global_id(0);

	if (id < n) {
		float3 RGB;
		float gap = log(sqrt((float)2));
		float2 input = vload2(0, &a[id*2]);
		if(((input.x + offset.x/zoom < 3*zoomc && input.x + offset.x/zoom > -3*zoomc) || (input.y + offset.y/zoom < 3*zoomc && input.y + offset.y/zoom > -3*zoomc)) && axesOn) {
			RGB = (float3)(0, 0, 0);
			vstore3(RGB, 0, &b[id*3]);
		}
		else {
			cplx ret = evalFunc((cplx)(zoom*(input.x)+offset.x, zoom*(input.y)+offset.y));
			// float3 DBG = (float3)(ret.x, ret.y, 1);

			if(shadOn) {
				RGB = (float3)(carg(ret), 1.0 - pow(zoomc, (float)cabs(ret)), fmod((float)log(cabs(ret)), gap) + 1 - gap);
			}
			else {
				RGB = (float3)(carg(ret), 1.0 - pow(zoomc, (float)cabs(ret)), 1);
			}
			RGB = hsv2rgb(RGB.x, RGB.y, RGB.z);
			if(gridOn) {
				RGB -= (float3)(fmod(abs((int)((ceil(ret.x/5) + ceil(ret.y/5)))), 2.0)/10);
			}
			vstore3(RGB, 0, &b[id*3]);
			//vstore3(DBG, 0, &b[id*3]);
		}
	}
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

inline cplx evalFunc(cplx z) { return 
