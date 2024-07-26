#include <stdio.h>
#include <stdlib.h>

#include "dyn_vec.h"


/****************
BLACK BOX TESTING
*****************/


int main(int argc, char **argv)
{

	size_t item_size = sizeof(int);
	size_t num_items = 5;

	int num = 1;
	int i = 0;
	int n = 30;
	
	
	/* create */
	dyn_vec_t *vec = DynVecCreate(item_size, num_items);
	
	
	printf("After create: current size of array (num of elements): %lu\n", DynVecSize(vec));
	
	printf("Push %d times:\n", n);
	
	/* 30 pushes */
	for(i = 0; i < n; ++i)
	{
		 
		if(!DynVecPushBack(vec, &num))
		{
			printf("pushed: %d  |  size: %lu\n", num, DynVecSize(vec));
		}
		else
		{
			printf("Error.\n");
		}
		
		++num;
	}
	
	
	printf("\nPop %d times:\n", n);
	
	/* 30 pops */
	num = 1;
	for(i = 0; i < n; ++i)
	{

		DynVecPopBack(vec);
		
		printf("poped: %d  |  size: %lu\n", num, DynVecSize(vec));
		
		++num;
	}
	
	
	
	/* destroy */
	DynVecDestroy(vec);
	
	return (0);
}
