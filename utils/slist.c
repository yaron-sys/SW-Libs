#include <assert.h>  /* assert */
#include <stdlib.h>  /* malloc free */

#include "slist.h"


/* Create a new node - O(1) */
slist_node_t *SListCreateAndInitNode(void *data, slist_node_t *next)
{
	slist_node_t *node = (slist_node_t *) malloc(sizeof(*node));
	
	if(NULL == node)
	{
		return (NULL);
	}
	
	/* initiate fields */
	node->data = data;
	node->next = next;
	
	return (node);
}


/* Free all nodes in slist - O(n) */
void SListFreeAll(slist_node_t *head)
{
	slist_node_t *iter = head;
	
	assert(0 == SListHasLoop(head));
	
	while(iter != NULL)
	{
		iter = iter->next;
		free(head);
		head = iter;
	}
}


/* Insert before an element - O(1) */
slist_node_t *SListInsert(slist_node_t *insert_node, slist_node_t *where)
{
	void *tmp_data = NULL;
	slist_node_t *tmp_node = NULL;
	
	assert(insert_node != NULL);
	assert(where != NULL);
	
	/* temp storing of data and next node of "where" */
	tmp_data = where->data;
	tmp_node = where->next;
	
	/* swap data between insert and where */
	where->data = insert_node->data;
	insert_node->data = tmp_data;
	
	/* change next pointers of insert and where */
	where->next = insert_node;
	insert_node->next = tmp_node;
	
	return (where);
}


/* Insert After an element - O(1) */
slist_node_t *SListInsertAfter(slist_node_t *insert_node, slist_node_t *where)
{
	slist_node_t *tmp_node = NULL;
	
	assert(insert_node != NULL);
	assert(where != NULL);
	
	/* temp storing data of "where" */
	tmp_node = where->next;
	
	/* change next pointers of insert and where */
	where->next = insert_node;
	insert_node->next = tmp_node;
	
	return (insert_node);
}


/* Remove an element - O(1) */
slist_node_t *SListRemove(slist_node_t *where)
{
	void *tmp_data = NULL;
	slist_node_t *removed_node = NULL;
	
	assert(where != NULL);
	assert(where->next != NULL);
	
	/* temp storing of data and next node of "where" */
	tmp_data = where->data;
	removed_node = where->next;
	
	/* swap data between "where" and "next where" */
	where->data = removed_node->data;
	removed_node->data = tmp_data;
	
	/* change "where" pointer */
	where->next = removed_node->next;
	
	/* removed_node->next = NULL; */
	
	return (removed_node);
}


/* Remove after an element - O(1) */
slist_node_t *SListRemoveAfter(slist_node_t *where)
{
	slist_node_t *removed_node;
	
	assert(where != NULL);
	assert(where->next != NULL);
	
	/* getting removed node */
	removed_node = where->next;
	
	/* "where" is pointing to next element */
	where->next = removed_node->next;
	
	/* removed_node->next = NULL; */
	
	return (removed_node);
}


/* Finding an element for gettin a match - O(n) */
/* 1-match , 0-no match. When found a match - returns desired node otherwise NULL */
slist_node_t *SListFind(slist_node_t *head,
						const void *to_find,
						void *params,
						int (*is_match)(const void *node_data, const void *to_find, void *params))
{
	slist_node_t *iter = head;
	
	assert(0 == SListHasLoop(head));
	
	while((iter != NULL) && (0 == is_match(iter->data, to_find, params)))
	{
		/* move to next element */
		iter = iter->next;
	}
	
	return (iter);
}


/* Iterating each element end envoking do_func - O(n) */
/* returns 0 for success. any number in case of failure */
int SListForEach(slist_node_t *head, int (*do_func)(void *params, void *data), void *params)
{
	int status = 0; /* success */
	slist_node_t *iter = head;
	
	assert(0 == SListHasLoop(head));
	
	while(iter != NULL && 0 == status)
	{
		status = do_func(params, iter->data);
		iter = iter->next;
	}
	
	return (status);
}


/* Fliping a list - O(n) */
slist_node_t *SListFlip(slist_node_t *head)
{
	slist_node_t *prev = NULL;
	slist_node_t *fwd = NULL;

	assert(head != NULL);
	assert(0 == SListHasLoop(head));
	
	/* fwd becomes next */
	fwd = head->next;
	
	while(fwd != NULL)
	{
		/* swap point direction to prev */
		head->next = prev;
		
		/* prev becomes current */
		prev = head; 
		
		/* current becomes next */
		head = fwd; 
		
		/* fwd becomes next */
		fwd = head->next; 
	}
	/* swap point direction to prev */
	head->next = prev; 
	
	return (head);
}


/* Returns 1 if has loop and 0 otherwise - O(n) */
int SListHasLoop(const slist_node_t *head)
{
	const slist_node_t *one_step_iter = head;
	const slist_node_t *two_step_iter = head;
	
	while(two_step_iter != NULL && two_step_iter->next != NULL)
	{
		one_step_iter = one_step_iter->next; /* one step ahead */
		two_step_iter = two_step_iter->next->next; /* two steps ahead */
		
		if(one_step_iter == two_step_iter)
		{	
			/* Loop found */
			return (1);
		}
	}
	
	return (0);
}


/* Return number of nodes in list - O(n) */
size_t SListSize(const slist_node_t *head)
{
	size_t count = 0;
	const slist_node_t *iter = head;
	
	assert(0 == SListHasLoop(head));
	
	while(iter != NULL)
	{
		/* count and get next element */
		iter = iter->next;
		++count;
	}
	
	return (count);
}


/* Return Intersection node if exist or NULL if doesn't - O(n) */
slist_node_t *SListFindIntersection(slist_node_t *head1, slist_node_t *head2)
{
	slist_node_t *longer = head2; /* list2 is longer */
	slist_node_t *shorter = head1; /* list 1 is shorter */
	size_t len1 = SListSize(head1); /* len of list1 */
	size_t len2 = SListSize(head2); /* len of list2 */
	size_t diff = (len1 > len2) ? (len1-len2) : (len2-len1); /* difference of lens */
	
	assert(head1 != NULL);
	assert(head2 != NULL);
	
	if (len1 > len2)
	{
		/* List1 is bigger */
		longer = head1;
		shorter = head2;
	}
	
	while(diff > 0)
	{
		/* set longer to be as same size as shorter  */
		longer = longer->next;
		--diff;
	}
	
	while(longer != NULL && (longer != shorter))
	{
		/* move to next element */
		longer = longer->next;
		shorter = shorter->next;
	}
	
	return (longer);
}



