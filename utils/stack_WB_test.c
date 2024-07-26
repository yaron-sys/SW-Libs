#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


/****************
WHITE BOX TESTING
*****************/

/* Manager Struct Decleration */
struct stack
{
	size_t element_size;
	size_t max_elements;
	void *top;
	char start[1];
};



int main(int argc, char **argv)
{

	size_t elements = 3;
	size_t size = sizeof(int);
	int *peek;
	int num = 10;
	int num2 = 20;
	int num3 = 30;
	
	stack_t *my_stack = StackCreate(size, elements);
	
	printf("status:\n");
	printf("element_size: %lu\n", my_stack->element_size);
	printf("max_elements: %lu\n", my_stack->max_elements);
	printf("\ncreating empty stack:\n");
	
	printf("stack size: %lu\n", StackSize(my_stack));

	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);

	printf("-----------\n");
	

	/* push 10 */
	printf("push 10:\n");
	if (StackPush(my_stack, &num) == 0)
	{
		printf("stack size: %lu\n", StackSize(my_stack));
		peek = (int*)StackPeek(my_stack);
		printf("stack peek: %d\n", *peek);
	}
	else
	{
		printf("failed at push\n");
	}


	printf("\n");


	/* push 20 */
	printf("push 20:\n");
	if (StackPush(my_stack, &num2) == 0)
	{
		printf("stack size: %lu\n", StackSize(my_stack));
		peek = (int*)StackPeek(my_stack);
		printf("stack peek: %d\n", *peek);
	}
	else
	{
		printf("failed at push\n");
	}
	
	
	printf("\n");


	/* push 30 */
	printf("push 30:\n");
	if (StackPush(my_stack, &num3) == 0)
	{
		printf("stack size: %lu\n", StackSize(my_stack));
		peek = (int*)StackPeek(my_stack);
		printf("stack peek: %d\n", *peek);

	}
	else
	{
		printf("failed at push\n");
	}


	printf("\n");
	

	/* push 30 */
	printf("push 30:\n");
	if (StackPush(my_stack, &num3) == 0)
	{
		printf("stack size: %lu\n", StackSize(my_stack));
		peek = (int*)StackPeek(my_stack);
		printf("stack peek: %d\n", *peek);
	}
	else
	{
		printf("failed at push\n");
	}


	printf("\n");



	/* pop 20 */
	printf("pop 30:\n");
	StackPop(my_stack);
	printf("stack size: %lu\n", StackSize(my_stack));
	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);

	
	
	printf("\n");
	

	/* pop 20 */
	printf("pop 30:\n");
	StackPop(my_stack);
	printf("stack size: %lu\n", StackSize(my_stack));
	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);
	

	printf("\n");
	

	/* pop 10 */
	printf("pop 10:\n");
	StackPop(my_stack);
	printf("stack size: %lu\n", StackSize(my_stack));
	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);
	
	
	
	/* free memory */
	StackDestroy(my_stack);
	
	
	return (0);
}
