#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


/****************
BLACK BOX TESTING
*****************/


int main(int argc, char **argv)
{

	size_t i = 0;
	/*
	int *peek;
	*/
	
	size_t count = 0;
	
	size_t elements_1 = 20;
	size_t elements_2 = 30;
	size_t elements_3 = 50;
	size_t elements_4 = 60;

	
	char element_char = '9';
	int element_int = 20;
	double element_double = 50.5;
	long element_long = 999999;

	
	/* initiate 5 stacks of different types */
	stack_t *my_stack_char = StackCreate(1, elements_1);
	stack_t *my_stack_int = StackCreate(4, elements_2);
	stack_t *my_stack_double = StackCreate(8, elements_3);
	stack_t *my_stack_long = StackCreate(8, elements_4);

	
	/* push loops */
	while(!StackPush(my_stack_char, &element_char))
	{
		++count;
	}
	printf("Pushed %lu times\n", count);
	count = 0;
	
	while(!StackPush(my_stack_int, &element_int))
	{
		++count;
	}
	printf("Pushed %lu times\n", count);
	count = 0;
	
	while(!StackPush(my_stack_double, &element_double))
	{
		++count;
	}
	printf("Pushed %lu times\n", count);
	count = 0;

	while(!StackPush(my_stack_long, &element_long))
	{
		++count;
	}
	printf("Pushed %lu times\n", count);
	count = 0;



	printf("\n");



	/* pop loops */
	while(i < 20)
	{
		StackPop(my_stack_char);
		++count; ++i;
	}
	printf("Poped %lu times\n", count);
	count = 0; i = 0;
	
	
	while(i < 30)
	{
		StackPop(my_stack_int);
		++count; ++i;
	}
	printf("Poped %lu times\n", count);
	count = 0; i = 0;
	
	while(i < 50)
	{
		StackPop(my_stack_double);
		++count; ++i;
	}
	printf("Poped %lu times\n", count);
	count = 0; i = 0;

	while(i < 60)
	{
		StackPop(my_stack_long);
		++count; ++i;
	}
	printf("Poped %lu times\n", count);
	count = 0; i = 0;




	
	/* free memories */
	StackDestroy(my_stack_char);
	StackDestroy(my_stack_int);
	StackDestroy(my_stack_double);
	StackDestroy(my_stack_long);

	
	return (0);
}
