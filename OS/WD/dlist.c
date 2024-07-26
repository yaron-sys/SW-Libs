#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "dlist.h"


typedef struct dlist_node
{
	void *data;
	dlist_iter_t prev; /* pointer to prev dlist_node */
	dlist_iter_t next; /* pointer to next dlist_node */
}dlist_node_t;


struct dlist
{
	dlist_node_t d_begin; /* "dummy begin" node */
	dlist_node_t d_end;   /* "dummy end" node */
};


/****************************
  Creates a new dlist - O(1)
****************************/
dlist_t *DlistCreate(void)
{
	dlist_t *dlist = (dlist_t *) malloc (sizeof(*dlist));

	if (NULL == dlist)
	{
		return (NULL);
	}
	
	/* initialize fields */
	dlist->d_begin.data = NULL;
	dlist->d_begin.prev = NULL;
	dlist->d_begin.next = &dlist->d_end;
	
	dlist->d_end.data = NULL;
	dlist->d_end.prev = &dlist->d_begin;
	dlist->d_end.next = NULL;


	return (dlist);
}


/***************************************
  Destroys a list and its content - O(n)
**************************************/
void DlistDestroy(dlist_t *dlist)
{
	dlist_iter_t begin = NULL;
	dlist_iter_t next = NULL;
	
	assert(dlist != NULL);
	
	/* begin and next gets first node in list */
	begin = next = dlist->d_begin.next;
	
	while (begin != DlistEnd(dlist))
	{
		next = DlistNext(begin);
		free(begin);
		begin = next;
	}
	begin = next = NULL;
	free(dlist); dlist = NULL;
}


/**************************
  returns list size - O(n)
**************************/
size_t DlistSize(const dlist_t *dlist)
{
	dlist_iter_t next = NULL;
	size_t count = 0;
	
	assert(dlist != NULL);
	
	/* first node in list */
	next = dlist->d_begin.next;
	
	while (next != &dlist->d_end)
	{
		++count;
		next = DlistNext(next);
	}
	
	return (count);
}



/************************************************
  returns 1 if list is empty, 0 otherwise - O(1)
************************************************/
int DlistIsEmpty(const dlist_t *dlist)
{
	assert(dlist != NULL);	
	
	return (dlist->d_begin.next == &dlist->d_end);
}


/**************************************************
  returns iterator of first node in the list - O(1)
**************************************************/
dlist_iter_t DlistBegin(dlist_t *dlist)
{
	assert(dlist != NULL);
	
	return (dlist->d_begin.next);
}


/**************************************************
  returns iterator of end node (out of range) - O(1)
**************************************************/
dlist_iter_t DlistEnd(dlist_t *dlist)
{
	assert(dlist != NULL);
	
	return (&dlist->d_end);
}	


/**************************************************
  returns next node iterator - O(1)
  it is forbiden to get next after "end dummy" node
**************************************************/	
dlist_iter_t DlistNext(dlist_iter_t current)
{
	assert(current != NULL);
	assert(current->next != NULL);
	
	return (current->next);
}		

		
/*****************************************************
  returns prev node iterator - O(1)
  it is forbiden to get prev before "first dummy" node
*****************************************************/
dlist_iter_t DlistPrev(dlist_iter_t current)
{
	assert(current != NULL);
	assert(current->prev != NULL);
	
	return (current->prev);
}


/******************************
  returns data of node - O(1)
******************************/	
void *DlistGetData(dlist_iter_t iter)
{
	assert(iter != NULL);
	
	return (iter->data);
}


/************************************************************************
returns iterator to inserted node on sucess or "end" iterator on faliure
complexity of O(1)
************************************************************************/	
dlist_iter_t DlistInsert(dlist_t *dlist, dlist_iter_t where, void *data)
{
	dlist_iter_t node = NULL;
	
	assert(dlist != NULL);
	assert(where != NULL);
	
	node = (dlist_node_t *) malloc (sizeof(*node));
	if (NULL == node)
	{
		return (DlistEnd(dlist));
	}
	
	/* initiate fields of new node */
	node->data = data;
	node->next = where;
	node->prev = where->prev;
	
	/* change next and prev directions to new node */
	where->prev = node;
	node->prev->next = node;
	
	return (node);
}


/**************************************************
  returns iterator to next node in the list - O(1)
  -This is forbiden to erase when list is empty- 
*************************************************/	
dlist_iter_t DlistErase(dlist_iter_t whom)
{
	dlist_iter_t whom_next = NULL;
	
	assert(whom != NULL);
	assert(whom->next != NULL);

	/* save next whom */
	whom_next = whom->next;
	
	whom->prev->next = whom_next;
	whom_next->prev = whom->prev;
	
	free(whom);
	whom = NULL;
	
	return (whom_next);
}


/*****************************************
  returns iterator to the new node - O(1) 
*****************************************/	
dlist_iter_t DlistPushBack(dlist_t *dlist, void *data)
{
	assert(dlist != NULL);
	
	/* insert before end */
	return (DlistInsert(dlist, DlistEnd(dlist), data));
}
		

/*****************************************
  returns iterator to the new node - O(1) 
*****************************************/
dlist_iter_t DlistPushFront(dlist_t *dlist, void *data)		
{
	assert(dlist != NULL);
	
	/* insert before begin */
	return (DlistInsert(dlist, DlistBegin(dlist), data));
}	


/*****************************************
returns data of the poped out node - O(1) 
*****************************************/
void *DlistPopBack(dlist_t *dlist)	
{
	void *pop_data = NULL;
	
	assert(dlist != NULL);
	assert(DlistIsEmpty(dlist) != 1);

	/* save data */
	pop_data = dlist->d_end.prev->data;

	/* erase last node */
	DlistErase(dlist->d_end.prev);

	return (pop_data);
}


/*****************************************
returns data of the poped out node - O(1) 
*****************************************/
void *DlistPopFront(dlist_t *dlist)
{
	void *pop_data = NULL;
	
	assert(dlist != NULL);
	assert(DlistIsEmpty(dlist) != 1);

	/* save data */
	pop_data = dlist->d_begin.next->data;

	/* erase first node */
	DlistErase(dlist->d_begin.next);

	
	return (pop_data);
}


/****************************************
	 returns 1 if same, 0 if not - O(1) 
****************************************/
int DlistIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	return (iter1 == iter2);
}



/*******************************************************************************
	iterates through list and returns return value of do_func.		
	if return value is a non-zero, iteration is stoped
	complexity of O(n)
********************************************************************************/	
int DlistForEach(dlist_iter_t from, dlist_iter_t to, int (*do_func)(void *data, void *params), void *params)
{
	int status = 0; /* success */
	dlist_iter_t iter = from;
	
	assert(from != NULL);
	assert(to != NULL);
	assert(do_func != NULL);
	
	while ((iter != to) && (0 == status))
	{
		status = do_func(iter->data, params);
		iter = DlistNext(iter);
	}
	
	return (status);
}


/*******************************************************************************	
    Returns iterator to the found node. if not found, returns "to".
    (sub function 'is_match' returns 1 if match is found and 0 otherwise).
    complexity of O(n)
********************************************************************************/	
dlist_iter_t DlistFind(dlist_iter_t from, dlist_iter_t to, const void *to_find, void *params,
		int (*is_match)(const void *node_data, const void *to_find, void *params))
{
	dlist_iter_t iter = from;
	
	assert(from != NULL);
	assert(to != NULL);
	assert(is_match != NULL);
	
	while ((iter != to) && (0 == is_match(iter->data, to_find, params)))
	{
		/* move to next element */
		iter = DlistNext(iter);
	}
	
	return (iter);
}
	


/*******************************************************************************
   cut and pase range: "from"(including) - "to"(excluding) right before "where".
   complexity of O(1)
********************************************************************************/
void DlistSplice(dlist_iter_t where, dlist_iter_t from, dlist_iter_t to)
{
	dlist_iter_t node_holder = NULL;	
	
	assert(where != NULL);
	assert(from != NULL);
	assert(to != NULL);	
	
	if (from != to)
	{
		/* saving the spliced node */
		node_holder = to->prev;
	
		/* changing directions of prev and next */
		to->prev = from->prev;
		from->prev->next = to;
		from->prev = where->prev;
		where->prev->next = from;
		where->prev = node_holder;
		node_holder->next = where;
	}
}


