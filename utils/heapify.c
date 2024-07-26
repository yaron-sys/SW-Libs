#include <assert.h>

#include "heapify.h"


static void SwapElements(void *val1, void *val2, size_t size)
{
	char *ch_val1 = (char*)val1;
	char *ch_val2 = (char*)val2;
	char tmp = 0;
	size_t i = 0;
	
	for (i = 0; i < size; ++i)
	{
		tmp = *(ch_val1 + i);
		*(ch_val1 + i) = *(ch_val2 + i);
		*(ch_val2 + i) = tmp;
	}
}

/*******************************************************************************
Heapify Up - bubble up an element to its place - O(log(n))
********************************************************************************/
void HeapifyUp(void  *arr, size_t element_size, size_t index,
			   int  (*is_before)(const void *data1, const void *data2, void *params),
			   void  *params)
{
	assert(arr != NULL);
	assert(is_before != NULL);
	assert(element_size != 0);
	
	if (index != 0)
	{
		size_t parent_idx = (index - 1) / 2;
		void *child = (char*)arr + (index * element_size);
		void *parent = (char*)arr + (parent_idx * element_size);
		
		if (is_before(child, parent, params))
		{
			SwapElements(parent, child, element_size);
			HeapifyUp(arr, element_size, parent_idx, is_before, params);
		}
	}
}

/*******************************************************************************
Help function for Heapify Down
Returns biggest child or NULL if parent has no childs.
********************************************************************************/
static void *GetBiggerChild(void *arr, size_t element_size, size_t len, size_t parent_idx, size_t *big_child_idx,
							int  (*is_before)(const void *data1, const void *data2, void *params),void *params)
{
	void *right_child = NULL;
	void *big_child = NULL;
	size_t left_child_idx = (2 * parent_idx) + 1;
	size_t right_child_idx = (2 * parent_idx) + 2;
	
	assert(arr != NULL);
	assert(is_before != NULL);
	assert(element_size != 0);
	
	/* check if left child exist */
	if (left_child_idx < len)
	{
		/* make left child as big child */
		big_child = (char*)arr + ((left_child_idx) * element_size);
		*big_child_idx = left_child_idx;
		
		/* check if right child exist */
		if (right_child_idx < len)
		{
			right_child = (char*)arr + ((right_child_idx) * element_size);
			
			if (is_before(right_child, big_child, params))
			{
				/* right child becomes big child if greater than left */
				big_child = right_child;
				*big_child_idx = right_child_idx;
			}
		}
	}
	
	return (big_child);
}

/*******************************************************************************
Heapify Down - bubble down an element to its place - O(log(n))
********************************************************************************/
void HeapifyDown(void  *arr, size_t element_size, size_t len, size_t index,
				 int  (*is_before)(const void *data1, const void *data2, void *params),
				 void *params)
{
	size_t big_child_idx = 0;
	
	assert(arr != NULL);
	assert(is_before != NULL);
	assert(element_size != 0);
	
	if (index < len)
	{
		void *parent = (char*)arr + (index * element_size);
		/* get big child (or null if doesn't exist) and update big_child_idx */
		void *big_child = GetBiggerChild(arr, element_size, len, index, &big_child_idx, is_before, params);
		
		if ((big_child != NULL) && (is_before(big_child, parent, params)))
		{
			SwapElements(big_child, parent, element_size);
			HeapifyDown(arr, element_size, len, big_child_idx, is_before, params);
		}
	}
}
