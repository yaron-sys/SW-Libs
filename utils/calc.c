#include <stdlib.h>   /* malloc, free, strtod */
#include <string.h>   /* strlen */
#include <ctype.h> /*isdigit */
#include <math.h>     /* pow */
#include <assert.h>   /* asserts */

#include "calc.h"
#include "stack.h"

#define NUM_STATES 3
#define NUM_EVENTS 256

/* calculates two nums from 'num stack' with one 'stack operand' */
static status_t TwoNumCalc(calc_t *calc, unsigned char stack_opr);
/* events init */
static void EventsInit(calc_t *calc);
/* operators init */
static void OpsInit(calc_t *calc);

typedef enum
{
	EXP_NUM = 0,
	EXP_OP = 1,
	ERROR = 2
}state_t;

typedef enum
{
	DUMMY = 0,
	ADD = 1,
	SUBTRACT = ADD,
	MULT = 2,
	DIV = MULT,
	POW = 3
}op_prio_t;

typedef struct oper
{
	op_prio_t prio;
	status_t (*op_func)(double *lhs, double rhs);
}operator_t;

typedef struct event
{
	status_t (*handler)(calc_t *calc);
	state_t next_state;
}event_t;

struct calc
{
	stack_t *num_stack;
	stack_t *op_stack;
	event_t events[NUM_STATES][NUM_EVENTS];
	const char *expr_iter;
	operator_t ops[NUM_EVENTS];
};

/*************************** Handle Functions *********************************/
static status_t HandleOper(calc_t *calc)
{
	status_t status = SUCCESS;
	char str_oper = 0;
	char top_stack_oper = 0;
	
	assert(calc != NULL);
	
	/* extract operator from str */
	str_oper = *(calc->expr_iter);
	
	/* peek operator from stack */
	top_stack_oper = *(char*)StackPeek(calc->op_stack);
	
	/* check dependencies in stack */
	if (str_oper != '^')
	{
		while ((calc->ops[(unsigned char)str_oper].prio) <= 
			   (calc->ops[(unsigned char)top_stack_oper].prio))
		{
			status = TwoNumCalc(calc, top_stack_oper);
			if (status != SUCCESS)
			{
				return (status);
			}	
			/* pop old oper from stack */
			StackPop(calc->op_stack);
		
			/* peek new operator from stack */
			top_stack_oper = *(char*)StackPeek(calc->op_stack);
		}
	}	
	/* enter str opr to stack */
	if (StackPush(calc->op_stack, calc->expr_iter))
	{
		return (ALLOCATION_ERR);
	}
	
	/* promote str pointer to next char */
	++(calc->expr_iter);
	
	return (status);
}

static status_t HandleNum(calc_t *calc)
{
	double num = 0.0;
	char *str = NULL;
	
	assert(calc != NULL);
	
	/* extract num from str */
	num = strtod(calc->expr_iter, &str);
	
	/* promote pointer after num */
	calc->expr_iter = str;
	
	/* push num to stack */
	if (StackPush(calc->num_stack, &num))
	{
		return (ALLOCATION_ERR);
	}
	return (SUCCESS);
}

static status_t HandleEndStr(calc_t *calc)
{
	status_t status = SUCCESS;
	char cur_top_oper = 0;
	
	assert(calc != NULL);
	
	cur_top_oper = *(char*)StackPeek(calc->op_stack);
	
	while ((cur_top_oper != '(') && (cur_top_oper != 'D'))
	{
		status = TwoNumCalc(calc, cur_top_oper);
		if (status != SUCCESS)
		{
			return (status);
		}

		StackPop(calc->op_stack);
		
		/* peek new operator from stack */
		cur_top_oper = *(char*)StackPeek(calc->op_stack);
	}
	
	if (cur_top_oper == '(')
	{
		return (SYNTAX_ERR);
	}

	return (status);
}

static status_t HandleError(calc_t *calc)
{
	assert(calc != NULL);
	
	return (SYNTAX_ERR);
}

static status_t HandleOpenBracket(calc_t *calc)
{
	assert(calc != NULL);
	
	if (StackPush(calc->op_stack, calc->expr_iter))
	{
		return (ALLOCATION_ERR);
	}
	
	/* promote str pointer to next char */
	++(calc->expr_iter);
	
	return (SUCCESS);
}

static status_t HandleCloseBracket(calc_t *calc)
{
	status_t status = SUCCESS;
	char cur_top_oper = 0;
	
	assert(calc != NULL);
	
	cur_top_oper = *(char*)StackPeek(calc->op_stack);
	
	while ((cur_top_oper != '(') && ((cur_top_oper != 'D')))
	{
		status = TwoNumCalc(calc, cur_top_oper);
		if (status != SUCCESS)
		{
			return (status);
		}
		
		StackPop(calc->op_stack);
		
		/* get new oper from stack */
		cur_top_oper = *(char*)StackPeek(calc->op_stack);
	}
	
	if(cur_top_oper != '(')
	{
		return (SYNTAX_ERR);
	}
	/* remove open bracket */
	StackPop(calc->op_stack);
	
	/* promote str pointer to next char */
	++(calc->expr_iter);
	
	return (status);
}

static status_t HandleSpace(calc_t *calc)
{
	assert(calc != NULL);
	
	/* promote str pointer to next char */
	++(calc->expr_iter);
	
	return (SUCCESS);
}

static status_t HandleUnary(calc_t *calc)
{
	char next_char = 0;
	
	assert(calc != NULL);
	
    next_char = *(char*)(calc->expr_iter + 1);
    if (!isdigit(next_char))
    {
        return (SYNTAX_ERR);
    }
    
    return (HandleNum(calc));
}

/***************************** Oper Functions *********************************/
static status_t OpMult(double *lhs, double rhs)
{
	assert(lhs != NULL);
	
	*lhs *= rhs;
	
	return (SUCCESS);
}

static status_t OpDiv(double *lhs, double rhs)
{
	assert(lhs != NULL);
	
	if (0 == rhs)
	{
		return (MATH_ERR);
	}
	*lhs /= rhs;
	
	return (SUCCESS);
}

static status_t OpAdd(double *lhs, double rhs)
{
	assert(lhs != NULL);
	
	*lhs += rhs;
	
	return (SUCCESS);
}

static status_t OpSub(double *lhs, double rhs)
{
	assert(lhs != NULL);
	
	*lhs -= rhs;
	
	return (SUCCESS);
}

static status_t OpPow(double *lhs, double rhs)
{
	assert(lhs != NULL);
	
	*lhs = pow(*lhs, rhs);
	
	return (SUCCESS);
}

static status_t OpDummy(double *lhs, double rhs)
{
	assert(lhs != NULL);
	
	return (SYNTAX_ERR);
}


/*******************************************************************************
						EVENTS INITIALIZE - side function
*******************************************************************************/
static void EventsInit(calc_t *calc)
{
	int i,j;
	/* Initialize array of events structs */
	for (i = 0; i < NUM_STATES; ++i)
	{
		for (j = 0; j < NUM_EVENTS; ++j)
		{
			calc->events[i][j].handler = &HandleError;
			calc->events[i][j].next_state = ERROR;
		}
	}
	/***************************************************************************
								State 1: EXP_NUM 
	***************************************************************************/
	/* Event: HandleNum ('0' - '9') */	
	for (j = '0'; j <= '9'; ++j)
	{
		calc->events[EXP_NUM][j].handler = &HandleNum;
		calc->events[EXP_NUM][j].next_state = EXP_OP;
	}

	/* Event: HandleUnary ('-' , '+') */	
	calc->events[EXP_NUM]['-'].handler = &HandleUnary;
	calc->events[EXP_NUM]['-'].next_state = EXP_OP;
	
	calc->events[EXP_NUM]['+'].handler = &HandleUnary;
	calc->events[EXP_NUM]['+'].next_state = EXP_OP;
	
	/* Event: HandleSpace (' ') */	
	calc->events[EXP_NUM][' '].handler = &HandleSpace;
	calc->events[EXP_NUM][' '].next_state = EXP_NUM;
	
	/* Event: HandleOpenBracet ('(') */	
	calc->events[EXP_NUM]['('].handler = &HandleOpenBracket;
	calc->events[EXP_NUM]['('].next_state = EXP_NUM;
	
	/***************************************************************************
								State 2: EXP_OP 
	***************************************************************************/	
	/* Event: HandleOper ('^', '+', '-', '*', '/') */
	calc->events[EXP_OP]['+'].handler = &HandleOper;
	calc->events[EXP_OP]['+'].next_state = EXP_NUM;
	
	calc->events[EXP_OP]['-'].handler = &HandleOper;
	calc->events[EXP_OP]['-'].next_state = EXP_NUM;
	
	calc->events[EXP_OP]['*'].handler = &HandleOper;
	calc->events[EXP_OP]['*'].next_state = EXP_NUM;
	
	calc->events[EXP_OP]['/'].handler = &HandleOper;
	calc->events[EXP_OP]['/'].next_state = EXP_NUM;
	
	calc->events[EXP_OP]['^'].handler = &HandleOper;
	calc->events[EXP_OP]['^'].next_state = EXP_NUM;
	
	/* Event: HandleCloseBracket (')') */
	calc->events[EXP_OP][')'].handler = &HandleCloseBracket;
	calc->events[EXP_OP][')'].next_state = EXP_OP;	
	
	/* Event: HandleSpace (' ') */
	calc->events[EXP_OP][' '].handler = &HandleSpace;
	calc->events[EXP_OP][' '].next_state = EXP_OP;
	
	/* Event: HandleEndStr ('\0') */
	calc->events[EXP_OP]['\0'].handler = &HandleEndStr;
	calc->events[EXP_OP]['\0'].next_state = ERROR;
}

/*******************************************************************************
					OPERATION INITIALIZE - side function
*******************************************************************************/
static void OpsInit(calc_t *calc)
{
	int i;
	/* Initialize array of operation structs */
	for (i = 0; i < NUM_EVENTS; ++i)
	{
		calc->ops[i].prio = DUMMY;
		calc->ops[i].op_func = &OpDummy;
	}

	calc->ops['+'].prio = ADD;
	calc->ops['+'].op_func = &OpAdd;
		
	calc->ops['-'].prio = SUBTRACT;
	calc->ops['-'].op_func = &OpSub;
	
	calc->ops['*'].prio = MULT;
	calc->ops['*'].op_func = &OpMult;
	
	calc->ops['/'].prio = DIV;
	calc->ops['/'].op_func = &OpDiv;
	
	calc->ops['^'].prio = POW;
	calc->ops['^'].op_func = &OpPow;
}

/*******************************************************************************
					TWO NUM CALC - side function
*******************************************************************************/
static status_t TwoNumCalc(calc_t *calc, unsigned char stack_opr)
{
	double *lhs = NULL;
	double rhs = 0.0;
	
	assert(calc != NULL);
	
	rhs = *(double*)StackPeek(calc->num_stack);
	StackPop(calc->num_stack);
	lhs = (double*)StackPeek(calc->num_stack);
	
	return (calc->ops[stack_opr].op_func(lhs, rhs));
}

/*******************************************************************************
							CALC INITIALIZATION
*******************************************************************************/
calc_t *CalcInit(void)
{
	calc_t *calc = (calc_t*)calloc(1, sizeof(*calc));
	if (NULL == calc)
	{
		return (NULL);
	}
	EventsInit(calc);
	OpsInit(calc);

	return (calc);
}

/*******************************************************************************
								CALC DESTROY
*******************************************************************************/
void CalcDestroy(calc_t *calc)
{
	free(calc); calc = NULL;
}

/*******************************************************************************
								CALC CALCULATE
*******************************************************************************/
status_t CalcCalculate(const char *expr_iter, double *ret, calc_t *calc)
{
	state_t cur_state = EXP_NUM;
	status_t status = SUCCESS;
	char cur_char;
	char dummy = 'D';
	*ret = 0.0;
	
	assert(calc != NULL);
	assert(expr_iter != NULL);
	assert(ret != NULL);
	
	calc->num_stack = StackCreate(sizeof(double), strlen(expr_iter));
	if (NULL == calc->num_stack)
	{
		return (ALLOCATION_ERR);
	}
	
	calc->op_stack = StackCreate(sizeof(char), strlen(expr_iter)+1);
	if (NULL == calc->op_stack)
	{
		free(calc->num_stack);
		calc->num_stack = NULL;
		return (ALLOCATION_ERR);
	}
	
	calc->expr_iter = expr_iter;
	
	/* push dummy to op stack */
	if (StackPush(calc->op_stack, &dummy))
	{
		assert(0);
		return (ALLOCATION_ERR);
	}
	
	/* Iterate each char in string until ERR state */
	while (cur_state != ERROR)
	{
		/* get next char from str */
		cur_char = *(calc->expr_iter);
		
		/* invoke event according to current char and current state */
		status = calc->events[cur_state][(unsigned char)cur_char].handler(calc);
		if (status != SUCCESS)
		{
			break;
		}	
		/* move to next state */
		cur_state = calc->events[cur_state][(unsigned char)cur_char].next_state;
	}
	
	if (SUCCESS == status)
	{
		/* pull from stack the result */
		*ret = *(double*)StackPeek(calc->num_stack);
	}
	
	/* Destroy stacks */
	StackDestroy(calc->num_stack); calc->num_stack = NULL;
	StackDestroy(calc->op_stack);  calc->op_stack = NULL;
	
	return (status);
}


