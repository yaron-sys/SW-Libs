#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "srt_list.h"
#include "pq.h"


struct pq
{
	srt_list_t *pq_list;
};


/**************************
Creates new priority queue 
***************************/
pq_t *PQCreate(void *params, int (*is_before)(const void *data1,
											  const void *data2,
											  void *params))
{
	pq_t *new_pq = NULL;
	
	assert (is_before != NULL);
	
	new_pq = (pq_t *)malloc(sizeof(*new_pq));
	if (NULL == new_pq)
	{
		return (NULL);
	}
	
	new_pq->pq_list = SrtListCreate(params, is_before);
	if(NULL == new_pq->pq_list)
	{
		free(new_pq); new_pq = NULL;
	}
	
	return (new_pq);
}


/**************************************
Destroy queue - Complexity of O(n) 
***************************************/
void PQDestroy(pq_t *pq)
{
	assert(pq != NULL);
	
	SrtListDestroy(pq->pq_list);
	free(pq); pq = NULL;
}


/**************************************
Returns size - Complexity of O(n) 
***************************************/						
size_t PQSize(const pq_t *pq)
{
	assert(pq != NULL);
	
	return(SrtListSize(pq->pq_list));
}


/**************************************
Returns 1 if empty, otherwise return 0.
Complexity of O(1) 
***************************************/						
int PQIsempty(const pq_t *pq)
{
	assert(pq != NULL);
	
	return(SrtListIsEmpty(pq->pq_list));
}

/**************************************************************
Returns 0 on success.
Returns 1 on failure: itertor of inserted node equals
 					  to iterator of END node (out of range).
Complexity of O(n)
**************************************************************/						
int PQEnqueue(pq_t *pq, void *data)
{
	assert(pq != NULL);
	
	return (SrtListIsSameIter(SrtListInsert(pq->pq_list, data), SrtListEnd(pq->pq_list)));
}
	
/*********************************************************
returns data and pop out the highest priority node.
Complexity of O(1) 
**********************************************************/							
void *PQDequeue(pq_t *pq)			
{
	assert(pq != NULL);
	assert(PQIsempty(pq) != 1);
	
	return (SrtListPopFront(pq->pq_list));
}

/*********************************************************
returns data of the highest priority node.
Complexity of O(1) 
**********************************************************/				
void *PQPeek(pq_t *pq)
{
	assert(pq != NULL);

	return (SrtListGetData(SrtListBegin(pq->pq_list)));
}
		

/************************************
Deletes all nodes in priority queue
Complexity of O(n) 
*************************************/							
void PQClear(pq_t *pq)
{
	srt_list_iter_t iter = {0};
	
	assert(pq != NULL);
	
	for(iter = SrtListBegin(pq->pq_list); 0 == SrtListIsEmpty(pq->pq_list); iter = SrtListRemove(iter));	
}


/*************************************************
Removes a specific node and return its data.
Returns NULL in case that node has not been found.
Complexity of O(n) 
**************************************************/	
void *PQRemove(pq_t *pq,
			   const void *to_find,
			   void *params,
			   int (*is_match)(const void *data, const void *to_find, void *params))
{
	srt_list_iter_t node_iter = {0};
	void *node_data = NULL;
	
	assert(pq != NULL);
	assert(is_match != NULL);
	
	node_iter = SrtListFindIf(SrtListBegin(pq->pq_list), SrtListEnd(pq->pq_list), to_find, params, is_match);
	
	if(0 == SrtListIsSameIter(node_iter , SrtListEnd(pq->pq_list)))
	{
		node_data = SrtListGetData(node_iter);
		SrtListRemove(node_iter);
	}
	
	return (node_data);
}

