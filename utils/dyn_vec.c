#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "dyn_vec.h"


/* Struct dyn_vec */
struct dyn_vec
{
	size_t item_size;
	size_t capacity;
	char *top;  	 /* pointer to top of array */
	char *vector;	/* pointer to start of array */
};



/****************************
 creates a new dynamic vector
 ****************************/
dyn_vec_t *DynVecCreate(size_t item_size, size_t num_items)
{
	
	dyn_vec_t *vec = NULL;
	
	assert(item_size != 0);
	assert(num_items != 0);
	
	/* Creating an instance of dyn_vec */
	vec = (dyn_vec_t *) malloc (sizeof(*vec));
	
	if(NULL == vec)
	{
		return (NULL);
	}
	
	/* creating a vector */
	vec -> vector = (char *) malloc (item_size * num_items);
	
	if(NULL == vec -> vector)
	{
		return (NULL);
	}
	
	
	/* initialize dyn_vec fields */
	vec -> top = vec -> vector + (num_items * item_size);
	vec -> item_size = item_size;
	vec -> capacity = num_items;
	
	return (vec);
}




/***********************
	deletes a dyn_vec
************************/
void DynVecDestroy(dyn_vec_t *vec)
{
	assert(vec != NULL);

	/* free dyn_vec memory */
	free(vec -> vector);
	free(vec);
	
	vec = NULL;
	
}



/********************************
	number of occupied elements
********************************/
size_t DynVecSize(const dyn_vec_t *vec)
{
	assert(vec != NULL);
	
	/* num of elements in dyn_vec */
	return (((vec -> top) - (vec -> vector)) / (vec -> item_size));
}




/**********************************************
	push an element and allocate if neccesary
**********************************************/
int DynVecPushBack(dyn_vec_t *vec, const void *item)
{
	
	assert(vec != NULL);
	assert(item != NULL);
	
	
	/* if dyn_vec is full - allocate double space */
	if (vec -> capacity == DynVecSize(vec))
	{
		if(DynVecReserve(vec, 2 * (vec -> capacity)))
		{
			/* allocation failed */
			return (1);
		}
	}
		
	/* push item onto dyn_vec */
	memcpy(vec -> top, item, vec -> item_size);

	/* promote top pointer */
	vec -> top += (vec -> item_size);
	
	
	return (0);
}




/***********************
	pop an element
************************/
void DynVecPopBack(dyn_vec_t *vec)
{
	assert(vec != NULL);
	
	/* dyn_vec is empty */
	assert(vec -> top != (vec -> vector));
	
	/* pop element from dyn_vec by decreasing top pointer */
	vec -> top -= (vec -> item_size);
}




/**************************************
	return item address of index
***************************************/
void *DynVecGetItemAddress(const dyn_vec_t *vec, size_t index)
{
	assert(vec != NULL);
	/*assert(index < (vec -> capacity));*/

	return ((vec -> vector) + (index * vec -> item_size));
}




/**************************************
	return total capacity of array 
***************************************/
size_t DynVecCapacity(const dyn_vec_t *vec)
{
	assert(vec != NULL);
	return (vec -> capacity);
}




/**************************************
	reserve new space to dynamic array
***************************************/
int DynVecReserve(dyn_vec_t *vec, size_t new_capacity)
{
	size_t num_of_items;
	void *tmp_arr = NULL;
	
	assert(vec != NULL);
	assert(vec -> capacity < new_capacity);
	
	/* num of occupied elements in array */
	num_of_items = DynVecSize(vec);
	
	/* allocate new capacity space */
	tmp_arr = realloc(vec -> vector, new_capacity * (vec -> item_size));
		
	if(NULL == tmp_arr)
	{		
		/* allocation failed */
		return (1);
	}
	
	/* dyn_vec gets new allocated space */
	vec -> vector = tmp_arr;
	tmp_arr = NULL;
	
	/* update members of dyn_vec */
	vec -> top = (char*)(vec -> vector) + (vec -> item_size * num_of_items);
	vec -> capacity = new_capacity;
	
	
	return (0);
}

