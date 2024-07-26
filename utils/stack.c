#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include <assert.h>


/* Manager Struct Decleration */
struct stack
{
	size_t element_size;
	size_t max_elements;
	void *top;
	char start[1];
};


/* creates a new stack */
stack_t *StackCreate(size_t element_size, size_t max_elements)
{
	
	stack_t *stack = NULL;
	
	assert(0 != max_elements);
	assert(0 != element_size);
	
	/* Creating a stack instance */
	stack = (stack_t *) malloc (sizeof(*stack) + (max_elements * element_size));
	if(!stack)
	{
		return (NULL);
	}
	
	/* initialize stack fields */
	stack -> top = stack -> start;
	stack -> element_size = element_size;
	stack -> max_elements = max_elements;

	return (stack);
}


/* deletes a stack */
void StackDestroy(stack_t *stack)
{
	assert(stack != NULL);

	/* free stack memory */
	if (stack)
	{
		free(stack);
		stack = NULL;
	}
}



/* push an element onto stack */
int StackPush(stack_t *stack, const void *element)
{
	
	assert(element != NULL);
	assert(stack != NULL);
	
	
	/* stack is full */
	if (stack -> max_elements == StackSize(stack))
	{
		return (1);
	}
	
	/* push element on stack */
	memcpy(stack -> top, element, stack -> element_size);


	/* promote top pointer */
	stack -> top = (void*)((size_t)(stack -> top) + (stack -> element_size));
	
	return (0);
}


/* pop element from stack */
void StackPop(stack_t *stack)
{
	assert(stack != NULL);
	
	/* stack is empty */
	assert(stack -> top != (stack -> start));
	
	/* pop element from stack by decreasing top pointer */
	stack -> top = (void*)((size_t)(stack -> top) - (stack -> element_size));
}



/* returns element on top */
void *StackPeek(const stack_t *stack)
{
	assert(stack != NULL);
	
	/* returns a top pointer referenced to last element */
	return ((void*)((size_t)(stack -> top) - stack -> element_size));
}



/* return num of elements */
size_t StackSize(const stack_t *stack)
{
	assert(stack != NULL);
	
	/* amount of occupied bytes divided by element size */
	return (((size_t)(stack -> top) - (size_t)(stack -> start)) / (stack -> element_size));
}


