#ifndef CALC_H_
#define CALC_H_

typedef struct calc calc_t;

typedef enum
{
	SUCCESS = 0,
	SYNTAX_ERR = 1,
	MATH_ERR = 2,
	ALLOCATION_ERR = 3
}status_t;


calc_t *CalcInit(void);

void CalcDestroy(calc_t *calc);

status_t CalcCalculate(const char *expr_iter, double *ret, calc_t *calc);

#endif /* CALC_H_ */
