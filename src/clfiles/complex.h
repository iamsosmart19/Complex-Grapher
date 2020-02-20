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
	return (cplx)( (n.x*m.x + n.y*m.y)/(pow(n.x, 2) + pow(n.y, 2)), (n.y*m.x - n.x*m.y)/(pow(n.x, 2) + pow(n.y, 2)) );
}

inline double creal(cplx n) {
	return n.x;
}

inline double cimag(cplx n) {
	return n.y;
}

inline cplx conj(cplx n) {
	return (cplx)(n.x, n.y);
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
	return (cplx)( sqrt(cabs(n))*(cos(carg(n)/2)), sqrt(cabs(n))*(sin(carg(n)/2)));
}

inline cplx csin(cplx n) {
	return (cplx)(sin(n.x) * cosh(n.y), cos(n.x) * sinh(n.y));
}

inline cplx ccos(cplx n) {
	return (cplx)(cos(n.x) * cosh(n.y), sin(n.x) * sinh(n.y));
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
	return clog(cadd(n, csqrt(cadd((cplx)(1, 0), cpow(n, 2))) ) );
}

inline cplx cacosh(cplx n) {
	return clog(cadd(n, cmult(csqrt(cadd(n, (cplx)(1,0))), csqrt(csub(n, (cplx)(1, 0))))));
}

inline cplx catanh(cplx n) {
	return cdiv(csub(clog(cadd(n, (cplx)(1, 0))), clog(csub(n, (cplx)(1, 0)))) ,(cplx)(2, 0));
}

inline cplx casin(cplx n) {
	return cmult( (cplx)(0, -1), casinh(cmult((cplx)(0, 1), n))); 
}

inline cplx cacos(cplx n) {
	return csub( (cplx)(M_PI/2), casin(n));
}

inline cplx catan(cplx n) {
	return cmult( (cplx)(0, -1), catanh(cmult((cplx)(0, 1), n)));
}

