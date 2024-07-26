#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "heap.h"
#include "heapify.h"
#include "dyn_vec.h"

struct heap		
{		
	dyn_vec_t *dyn_vec;	
	size_t element_size;
	/* the element before is higher in the heap */		
	int      (*is_before)(const void *data1, const void *data2, void *params);		
	void      *params;		
};
	
/******************************************************************************* 
Create heap
********************************************************************************/
heap_t *HeapCreate(size_t element_size,
				   int (*is_before)(const void *data1, const void *data2, void *params),
				   void *params)
{
	heap_t *heap = NULL;
	
	assert(element_size != 0);
	assert(is_before != NULL);
	
	heap = (heap_t *)malloc(sizeof(*heap));
	if (NULL == heap)
	{
		return (NULL);
	}
	
	heap->element_size = element_size;
	heap->is_before = is_before;
	heap->params = params;
	heap->dyn_vec = DynVecCreate(element_size, 1);
	if (NULL == heap->dyn_vec)
	{
		free(heap); heap = NULL;
		return (NULL);
	}
	
	/* get an empty dyn_vec */
	DynVecPopBack(heap->dyn_vec);
	
	return (heap);
}		

/******************************************************************************* 
Destroy - O(1).
********************************************************************************/
void HeapDestroy(heap_t *heap)		
{
	assert(heap != NULL);
	
	DynVecDestroy(heap->dyn_vec);
	free(heap); heap = NULL;
}

/******************************************************************************* 
Returns size of heap - O(1).
********************************************************************************/
size_t HeapSize(const heap_t *heap)		
{
	assert(heap != NULL);
	
	return (DynVecSize(heap->dyn_vec));
}

/******************************************************************************* 
Returns 1 if empty, otherwise 0 - O(1).
********************************************************************************/
int HeapIsEmpty(const heap_t *heap)
{
	assert(heap != NULL);
	
	return (0 == HeapSize(heap));
}

/******************************************************************************* 
Push an element to heap - O(log(n)).
********************************************************************************/
int HeapPush(heap_t *heap, const void *data)	
{
	assert(heap != NULL);

	if (DynVecPushBack(heap->dyn_vec, data))
	{
		return (1);
	}
	
	HeapifyUp(HeapPeek(heap), heap->element_size, HeapSize(heap)-1, heap->is_before, heap->params);
	
	return (0);
}

/******************************************************************************* 
Pop Max element - O(log(n)).
********************************************************************************/
void HeapPop(heap_t *heap)	
{
	void *data_last = NULL;
	
	assert(heap != NULL);
	assert(HeapSize(heap) != 0);
	
	if (1 == HeapSize(heap))
	{
		DynVecPopBack(heap->dyn_vec);
		return;
	}
	
	/* get data of last element */
	data_last = DynVecGetItemAddress(heap->dyn_vec, HeapSize(heap)-1);

	/* copy data of last element to first element */
	memcpy(HeapPeek(heap), data_last, heap->element_size);
	
	/* pop last element */
	DynVecPopBack(heap->dyn_vec);
	
	/* move first element to its correct position */
	HeapifyDown(HeapPeek(heap), heap->element_size, HeapSize(heap), 0, heap->is_before, heap->params);
}

/******************************************************************************* 
Peek Max element - O(1).
********************************************************************************/
void *HeapPeek(const heap_t *heap)
{
	assert(heap != NULL);
	assert(HeapSize(heap) != 0);
	
	return (DynVecGetItemAddress(heap->dyn_vec, 0));
	
}		

/******************************************************************************* 
Removes an element from heap - O(n).
Return 0 in success or 1 if not found. 
********************************************************************************/		
int HeapRemove(heap_t *heap, const void *data, int (*is_match)(const void *data,		
	 		   const void *to_find, void *params), void *params, void *removed)
{
	size_t idx = 0UL;
	size_t size = 0UL;
	void *data_last = NULL;
	void *to_remove = NULL;
	
	assert(heap != NULL);
	assert(HeapSize(heap) != 0);
	assert(is_match != NULL);
	
	/* search for element to remove and get its data */
	to_remove = HeapPeek(heap);
	size = HeapSize(heap);
	
	while ((idx < size) && (!is_match(to_remove, data, params)))
	{
		++idx;
		to_remove = DynVecGetItemAddress(heap->dyn_vec, idx);
	}
	
	if (idx == size)
	{
		/* Out of range - match is not found */
		return (1);
	}

	if (removed != NULL)
	{
		/* copy to out param */
		memcpy(removed, to_remove, heap->element_size);
	}

	if (1 == HeapSize(heap))
	{
		/* check if only one element is left in the array */
		DynVecPopBack(heap->dyn_vec);
		return (0);
	}
	
	/* get data of last element */
	data_last = DynVecGetItemAddress(heap->dyn_vec, size-1);

	/* copy data of last element to 'to_remove' element */
	memmove(to_remove, data_last, heap->element_size);

	/* pop last element */
	DynVecPopBack(heap->dyn_vec);

	/* move 'to_remove' element to its correct position */
	HeapifyDown(HeapPeek(heap), heap->element_size, HeapSize(heap), idx, heap->is_before, heap->params);
	
	return (0);
}
