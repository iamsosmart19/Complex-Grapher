#include "parse.h"
#include "scan.h"
#include "stack.h"
#include "complex.h"
#include "tgmath.h"

cplx evalFunc(cplx* arr, int tknCnt, cplx val);

typedef void (*Handler)(stack *s);

/* void eval_SQRT(stack *s); */ 
/* void eval_ROOT(stack *s); */ 
/* void eval_LN(stack *s); */ 
/* void eval_LOG(stack *s); */ 
/* void eval_ABS(stack *s); */ 
/* void eval_FLOOR(stack *s); */ 
/* void eval_CEIL(stack *s); */ 
/* void eval_ASIN(stack *s); */ 
/* void eval_ACOS(stack *s); */ 
/* void eval_ATAN(stack *s); */ 
/* void eval_SINH(stack *s); */ 
/* void eval_COSH(stack *s); */ 
/* void eval_TANH(stack *s); */ 
/* void eval_SECH(stack *s); */ 
/* void eval_CSCH(stack *s); */ 
/* void eval_COTH(stack *s); */ 
/* void eval_SIN(stack *s); */ 
/* void eval_COS(stack *s); */ 
/* void eval_TAN(stack *s); */ 
/* void eval_SEC(stack *s); */ 
/* void eval_CSC(stack *s); */ 
/* void eval_COT(stack *s); */

extern char funcTable[25][16];
