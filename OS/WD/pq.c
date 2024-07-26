#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "heap.h"
#include "pq.h"


typedef struct params
{
	int (*is_before)(const void *data1, const void *data2, void *params);
	void *params;
}params_t;

struct pq
{
	heap_t  *pq_heap;
	params_t params;
};

static int FuncWraper(const void *data1, const void *data2, void *params)
{
	return (((params_t *)params)->is_before(*(void **)data1, *(void **)data2,
			((params_t *)params)->params));
}

/*******************************************************************************
Creates new priority queue 
********************************************************************************/
pq_t *PQCreate(void *params, int (*is_before)(const void *data1,
											  const void *data2,
											  void *params))
{
	pq_t *pq = NULL;
	
	assert(is_before != NULL);
	
	pq = (pq_t *)malloc(sizeof(*pq));
	if (NULL == pq)
	{
		return (NULL);
	}
	
	pq->params.is_before = is_before;
	pq->params.params = params;
	pq->pq_heap = HeapCreate(sizeof(void*), FuncWraper, &pq->params);
	if (NULL == pq->pq_heap)
	{
		free(pq); pq = NULL;
	}
	
	return (pq);
}

/******************************************************************************* 
Destroy - O(1).
********************************************************************************/
void PQDestroy(pq_t *pq)
{
	assert(pq != NULL);
	
	HeapDestroy(pq->pq_heap);
	free(pq); pq = NULL;
}

/******************************************************************************* 
Returns size of heap - O(1).
********************************************************************************/						
size_t PQSize(const pq_t *pq)
{
	assert(pq != NULL);

	return (HeapSize(pq->pq_heap));
}


/******************************************************************************* 
Returns 1 if empty, otherwise 0 - O(1).
********************************************************************************/						
int PQIsempty(const pq_t *pq)
{
	assert(pq != NULL);
	
	return (HeapIsEmpty(pq->pq_heap));
}

/******************************************************************************* 
Push an element to queue by order - O(log(n)).
********************************************************************************/				
int PQEnqueue(pq_t *pq, void *data)
{
	assert(pq != NULL);
	
	return (HeapPush(pq->pq_heap, &data));
}
	
/******************************************************************************* 
Pop Max element - O(log(n)).
********************************************************************************/							
void *PQDequeue(pq_t *pq)			
{
	void *data = NULL;
	
	assert(pq != NULL);
	assert(PQIsempty(pq) != 1);
	
	data = PQPeek(pq);
	HeapPop(pq->pq_heap);
	
	return (data);
}

/*******************************************************************************
returns data of the highest priority element (max) - O(1) 
********************************************************************************/				
void *PQPeek(pq_t *pq)
{
	assert(pq != NULL);
	assert(PQIsempty(pq) != 1);
	
	return (*(void**)HeapPeek(pq->pq_heap));
}	

/*******************************************************************************
Deletes all elements in priority queue - O(nlog(n)).
********************************************************************************/						
void PQClear(pq_t *pq)
{	
	assert(pq != NULL);
	
	while (!HeapIsEmpty(pq->pq_heap))
	{
		HeapPop(pq->pq_heap);
	}
}

/******************************************************************************* 
Removes an element from queue - O(n).
Return removed element in success or NULL in failure. 
********************************************************************************/		
void *PQRemove(pq_t *pq,
			   const void *to_find,
			   void *params,
			   int (*is_match)(const void *data, const void *to_find, void *params))
{
	void *removed = NULL;
	params_t params_func = {0};
	
	assert(pq != NULL);
	assert(is_match != NULL);
	
	params_func.is_before = is_match;
	params_func.params = params;
	
	if (HeapRemove(pq->pq_heap, &to_find, FuncWraper, &params_func, &removed))
	{
		return (NULL);
	}
	 		   
	return (removed);
}

