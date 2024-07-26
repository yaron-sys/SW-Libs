#include <stdlib.h> /* malloc, free */
#include <assert.h> /* asserts */

#include "hash.h"
#include "dlist.h"

struct hash		
{		
	size_t   hash_size;		
	size_t  (*hash_function)(const void *data);		
	int 	(*is_match)(const void *data1, const void *data2, void *params);		
	dlist_t **dlist_table;		
};

/*******************************************************************************
 create hash table
*******************************************************************************/
hash_t *HashCreate(size_t hash_size,		
				   size_t (*hash_function)(const void *data),		
				   int (*is_match)(const void *data1, const void *data2, void *params))
{
	hash_t *hash = NULL;
	
	assert(hash_size != 0);
	assert(hash_function != NULL);
	assert(is_match != NULL);
	
	hash = (hash_t *)malloc(sizeof(*hash));
	if (NULL == hash)
	{
		return (NULL);
	}
	
	hash->hash_size = hash_size;
	hash->hash_function = hash_function;
	hash->is_match = is_match;
	hash->dlist_table = (dlist_t **)calloc(hash_size, sizeof(dlist_t*));
	if (NULL == hash->dlist_table)
	{
		free(hash);
		return (NULL);
	}
	
	return (hash);
}

/*******************************************************************************
 Destroy - O(n)
*******************************************************************************/
void HashDestroy(hash_t *hash_table)
{
	size_t i = 0UL;
	
	assert(hash_table != NULL);
	
	for (i = 0; i < hash_table->hash_size; ++i)
	{
		if (hash_table->dlist_table[i] != NULL)
		{
			DlistDestroy(hash_table->dlist_table[i]);
		}
	}
	free(hash_table->dlist_table); hash_table->dlist_table = NULL;
	free(hash_table); hash_table = NULL;
}		

/*******************************************************************************
 Insert node to list. returns 0 in success. otherwise 1 - O(1)
*******************************************************************************/	
int HashInsert(hash_t *hash_table, void *to_insert)
{
	dlist_iter_t to_insert_iter = {0};
	size_t key = 0UL;
	
	assert(hash_table != NULL);
	
	/* get key */
	key = hash_table->hash_function(to_insert);

	assert(key < hash_table->hash_size);

	if (NULL == hash_table->dlist_table[key])
	{
		/* create a list if empty */
		hash_table->dlist_table[key] = DlistCreate();
		if (NULL == hash_table->dlist_table[key])
		{
			/* Allocation failed */
			return (1);
		}
	}
	
	/* insert node */
	to_insert_iter = DlistPushFront(hash_table->dlist_table[key], to_insert);
	if (DlistIsSameIter(to_insert_iter, DlistEnd(hash_table->dlist_table[key])))
	{
		/* failed to insert */
		return (1);
	}
	
	return (0);
}		

/*******************************************************************************
 Find iter - help function - O(n)
*******************************************************************************/
static dlist_iter_t FindIter(const hash_t *hash_table,const void *to_find, void *params, int *is_valid_itr)
{
	dlist_iter_t to_find_iter = {0};
	size_t key = 0UL;
	*is_valid_itr = 0;
	
	assert(hash_table != NULL);
	
	/* get key */
	key = hash_table->hash_function(to_find);

	assert(key < hash_table->hash_size);
	
	if (NULL == hash_table->dlist_table[key])
	{	
		return (to_find_iter);
	}
	
	to_find_iter = DlistFind(DlistBegin(hash_table->dlist_table[key]),
							 DlistEnd(hash_table->dlist_table[key]),
							 to_find, params, hash_table->is_match);
	
	/* verify that iter is found */
	if (!DlistIsSameIter(to_find_iter, DlistEnd(hash_table->dlist_table[key])))
	{
		*is_valid_itr = 1;
	}
	
	return (to_find_iter);
}

/*******************************************************************************
 Remove node from the correct list and return its data.
 if node/list does not exist return NULL - O(n)
*******************************************************************************/	
void *HashRemove(hash_t *hash_table, const void *to_remove, void *params)		
{
	dlist_iter_t to_remove_iter = {0};
	int is_valid_itr = 0;
	void *data = NULL;
	
	assert(hash_table != NULL);
		
	to_remove_iter = FindIter(hash_table, to_remove, params, &is_valid_itr);
	if (is_valid_itr)
	{
		/* get data */
		data = DlistGetData(to_remove_iter);
	
		/* remove */
		DlistErase(to_remove_iter);
	}
	
	return (data);	
}		

/*******************************************************************************
 Return the data if found, other return NULL - O(n)
*******************************************************************************/
void *HashFind(const hash_t *hash_table, const void *to_find, void *params)		
{
	dlist_iter_t to_find_iter = {0};
	int is_valid_itr = 0;
	void *data = NULL;
	
	assert(hash_table != NULL);
	
	to_find_iter = FindIter(hash_table, to_find, params, &is_valid_itr);
	if (is_valid_itr)
	{
		data = DlistGetData(to_find_iter);
	}
	
	return (data);
}	

/*******************************************************************************
 For Each - O(n)
 Returns 0 in success or any other value in failure.
*******************************************************************************/
int HashForEach(const hash_t *hash_table, int (*do_func)(void *data, void *params), void *params)		
{
	size_t i = 0UL;
	int status = 0;
	
	assert(hash_table != NULL);
	assert(do_func != NULL);
	
	while ((i < hash_table->hash_size) && (0 == status))
	{
		if (hash_table->dlist_table[i] != NULL)
		{
			status = DlistForEach(DlistBegin(hash_table->dlist_table[i]),
								  DlistEnd(hash_table->dlist_table[i]),
								  do_func, params);
		}
		++i;
	}
	return (status);
}

/*******************************************************************************
 Is empty - O(n)
*******************************************************************************/		
int HashIsEmpty(const hash_t *hash_table)
{
	size_t i = 0UL;
	
	assert(hash_table != NULL);
	
	for (i = 0; i < hash_table->hash_size; ++i)
	{
		if (hash_table->dlist_table[i] != NULL)
		{
			if (!DlistIsEmpty(hash_table->dlist_table[i]))
			{
				return (0);
			}
		}
	}
	return (1);
}	

/*******************************************************************************
 Return hash Size - O(n)
*******************************************************************************/
size_t HashSize(const hash_t *hash_table)
{
	size_t size = 0UL;
	size_t i = 0;
	
	assert(hash_table != NULL);
	
	for (i = 0; i < hash_table->hash_size; ++i)
	{
		if (hash_table->dlist_table[i] != NULL)
		{
			size += DlistSize(hash_table->dlist_table[i]);
		}
	}
	
	return (size);
}

