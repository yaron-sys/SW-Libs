#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "dlist.h"
#include "srt_list.h"



struct srt_list
{
	dlist_t *list;
	void *params;
	int (*is_before)(const void *data1, const void *data2, void *params);
};										


/*********************************
  Creates a new Sorted list - O(1)
**********************************/
srt_list_t *SrtListCreate(void *params, int (*is_before)(const void *data1,
						  				   				 const void *data2,
						  				                 void *params))
{
	srt_list_t *srt_list = NULL;
	
	assert (is_before != NULL);
	
	srt_list = (srt_list_t *)malloc(sizeof(*srt_list));
	if (NULL == srt_list)
	{
		return (NULL);
	}
	
	/* Initialize fields of srt_list */
	srt_list->list = DlistCreate();
	if (NULL == srt_list->list)
	{
		free(srt_list); srt_list = NULL;
		return (NULL);
	}
	srt_list->params = params;
	srt_list->is_before = is_before;
	
	return (srt_list);
}


/*********************************************
  Destroys an Srt list and its content - O(n)
*********************************************/
void SrtListDestroy(srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	
	DlistDestroy(srt_list->list);
	free(srt_list); srt_list = NULL;
}


/**************************
  returns list size - O(n)
**************************/
size_t SrtListSize(const srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	
	return (DlistSize(srt_list->list));
}


/************************************************
  returns 1 if list is empty, 0 otherwise - O(1)
************************************************/
int SrtListIsEmpty(const srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	
	return (DlistIsEmpty(srt_list->list));
}


/**************************************************
  returns iterator to next node in the list - O(1)
  -This is forbiden to erase when list is empty- 
*************************************************/
srt_list_iter_t SrtListRemove(srt_list_iter_t whom)
{
	assert(whom.info != NULL);
	
	whom.info = (struct srt_list_iter_info*)DlistErase((dlist_iter_t) whom.info);
	return (whom);
}


/**************************************************
  returns iterator of first node in the list - O(1)
**************************************************/
srt_list_iter_t SrtListBegin(const srt_list_t *srt_list)
{
	srt_list_iter_t begin = {0};
	
	assert(srt_list != NULL);
	
	begin.info = (struct srt_list_iter_info*)DlistBegin(srt_list->list);
	return (begin);
}


/**************************************************
  returns iterator of end node (out of range) - O(1)
***************************************************/
srt_list_iter_t SrtListEnd(const srt_list_t *srt_list)
{
	srt_list_iter_t end = {0};
	
	assert(srt_list != NULL);
	
	end.info = (struct srt_list_iter_info*)DlistEnd(srt_list->list);
	return (end);
}								


/***********************************
  returns prev node iterator - O(1)
************************************/										
srt_list_iter_t SrtListPrev(srt_list_iter_t current)										
{
	assert(current.info != NULL);
	
	current.info = (struct srt_list_iter_info*)DlistPrev((dlist_iter_t) current.info);
	return (current);
}


/***********************************
  returns next node iterator - O(1)
************************************/									
srt_list_iter_t SrtListNext(srt_list_iter_t current)									
{
	assert(current.info != NULL);
	
	current.info = (struct srt_list_iter_info*)DlistNext((dlist_iter_t) current.info);
	return (current);
}


/******************************
  returns data of node - O(1)
******************************/
void *SrtListGetData(srt_list_iter_t iter)
{
	assert(iter.info != NULL);
	
	return (DlistGetData((dlist_iter_t)iter.info));
}					


/****************************************
	 returns 1 if same, 0 if not - O(1) 
****************************************/
int SrtListIsSameIter(srt_list_iter_t iter1, srt_list_iter_t iter2)
{
	return (iter1.info == iter2.info);
}


/*****************************************
returns data of the poped out node - O(1) 
*****************************************/
void *SrtListPopBack(srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	
	return (DlistPopBack(srt_list->list));
}								


/*****************************************
returns data of the poped front node - O(1) 
*****************************************/										
void *SrtListPopFront(srt_list_t *srt_list)
{
	assert(srt_list != NULL);
	
	return (DlistPopFront(srt_list->list));	
}


/************************************************************************
Returns iterator to inserted node on sucess or iterator to 'end' on failure
sub-function 'is before' returns: 1 - true, 0 - false
complexity of O(n)
************************************************************************/
srt_list_iter_t SrtListInsert(srt_list_t *srt_list, void *data)
{
	srt_list_iter_t iter = {0};

	assert(srt_list != NULL);
	
	iter = SrtListBegin(srt_list);
	
	while ((0 == SrtListIsSameIter(iter, SrtListEnd(srt_list))) &&
	       (1 == srt_list->is_before(SrtListGetData(iter), data, srt_list->params)))
	{
		iter = SrtListNext(iter);
	}
	
	iter.info = (struct srt_list_iter_info*)DlistInsert(srt_list->list, (dlist_iter_t)iter.info, data);
	
	return (iter);
}


/************************************************************************
returns iterator to found node on sucess or iterator to 'to' if not found
sub-function 'is before' returns: 1 - true, 0 - false
complexity of O(n)
************************************************************************/
srt_list_iter_t SrtListFind(srt_list_t *srt_list, srt_list_iter_t from, srt_list_iter_t to, const void *to_find)
{
	srt_list_iter_t iter = from;
	
	assert(srt_list != NULL);
	assert(from.info != NULL);
	assert(to.info != NULL);
	assert(1 == srt_list->is_before(SrtListGetData(from), SrtListGetData(SrtListPrev(to)), srt_list->params));
	
	/* As long as iter!=end and iter is before data */
	while (0 == SrtListIsSameIter(iter, to))
	{
		if (0 == srt_list->is_before(SrtListGetData(iter), to_find, srt_list->params))
		{
			if(0 == srt_list->is_before(to_find, SrtListGetData(iter), srt_list->params))
			{
				return (iter);
			}
			
			else
			{
				return (to);
			}
		}
		
		iter = SrtListNext(iter);
	}
	
	return (iter);	
}
							

/************************************************************************
returns iterator to found node on sucess or iterator to 'to' if not found
sub-function 'is match' returns: 1 - true, 0 - false
complexity of O(n)
************************************************************************/						
srt_list_iter_t SrtListFindIf(srt_list_iter_t from,										
							  srt_list_iter_t to,			
							  const void *to_find,			
							  void *params,			
						      int (*is_match)(const void *node_data,const void *to_find, void *params))
{
	srt_list_iter_t iter = from;
	
	assert(from.info != NULL);
	assert(to.info != NULL);
	assert(is_match != NULL);
	
	while ((0 == SrtListIsSameIter(iter, to)) && (0 == is_match(SrtListGetData(iter), to_find , params)))
	{
		iter = SrtListNext(iter);
	}
	
	return (iter);	
}
							

/****************************************************************************
Iterates through sorted list.
Returns the value of do_func: 0 on success. any other value in case of failure
complexity of O(n)
****************************************************************************/										
int SrtListForEach(srt_list_iter_t from, srt_list_iter_t to, void *params,
				   int(*do_func)(void *data, void *params))
{
	assert(from.info != NULL);
	assert(to.info != NULL);
	assert(do_func != NULL);
	
	return (DlistForEach((dlist_iter_t)from.info, (dlist_iter_t)to.info, do_func, params));	
}

				
/*************************************
Merge together two sorted lists.
No handling in case of un-sorted lists
complexity of O(n)
**************************************/	
void SrtListMerge(srt_list_t *dest, srt_list_t *src)
{
	
	assert(dest != NULL);
	assert(src != NULL);
	
	while(0 == SrtListIsEmpty(src))
	{
		SrtListInsert(dest, SrtListPopFront(src));
	}
}		
						
