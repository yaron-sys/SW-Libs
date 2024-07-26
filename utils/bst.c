#include <stdlib.h>
#include <assert.h>

#include "bst.h"
#include "dyn_vec.h"

typedef struct bst_node bst_node_t;

struct bst_node
{
	void 	   *data;
	bst_node_t *parent;
	bst_node_t *children[2];
};

struct bst
{
	bst_node_t end;
	int	       (*is_before)(const void *data1, const void *data2, void *params);
	void	   *params;
};

enum {LEFT = 0, RIGHT = 1};


static int RightOrLeftSon(bst_node_t *son)
{
    return (son->parent->children[RIGHT] == son);
}

bst_t *BstCreate(int (*is_before)(const void *data1, const void *data2, void *params), void *params)
{
	bst_t *bst = NULL;
	
	assert(is_before != NULL);
	
	bst = (bst_t *)malloc(sizeof(*bst));
	if (NULL == bst)
	{
		return (NULL);
	}
	
	/* Initialize fields */
	bst->is_before = is_before;
	bst->params = params;
	bst->end.data = NULL;
	bst->end.parent = NULL;
	bst->end.children[LEFT] = NULL;
	bst->end.children[RIGHT] = NULL;
	
	return (bst);
}

/***********************************************************
Returns an iterator to smallest element in the tree - O(n) 
***********************************************************/					
bst_iter_t BstBegin(const bst_t *tree)
{
	const bst_node_t *cur = NULL;
	bst_iter_t ret = {0};
	
	assert(tree != NULL);
	
	cur = &tree->end;
	while(cur->children[LEFT] != NULL)
	{
		cur = cur->children[LEFT];
	}
	
	ret.data = (void*)cur;
	return (ret);
}				

/***********************************************************
Returns an iterator to next last element in the tree - O(1) 
***********************************************************/								
bst_iter_t BstEnd(const bst_t *tree)					
{
	bst_iter_t end = {0};
	
	assert(tree != NULL);
	
	end.data = (void*)&tree->end;
	return (end);
}
								
/*******************************************************************
Returns an iterator to next node. undefined behaviour on End - O(n) 
*******************************************************************/
bst_iter_t BstNext(bst_iter_t current)					
{
	bst_iter_t next = {0};
	bst_node_t *cur = (bst_node_t *)current.data;
	bst_node_t *parent = cur->parent;
	
	assert(current.data != NULL);
	
	/* node has right son - search for left node in right sub-tree */
	if (cur->children[RIGHT] != NULL)
	{
		cur = cur->children[RIGHT];
		while (cur->children[LEFT] != NULL)
		{
			cur = cur->children[LEFT];
		}
		next.data = (void *)cur;
	}
	else
	{
		/* search for first right parent */
		while (parent->children[LEFT] != cur)
		{
			cur = parent;
			parent = cur->parent;
		}		
		next.data = (void *)parent;
	}
	return (next);
}

/************************************************************************
Returns an iterator to previous node. undefined behaviour on Begin - O(n) 
************************************************************************/					
bst_iter_t BstPrev(bst_iter_t current)
{
	bst_iter_t next = {0};
	bst_node_t *cur = (bst_node_t *)current.data;
	bst_node_t *parent = cur->parent;
	
	assert(current.data != NULL);
	
	/* node has left son - search for right node in left sub-tree */
	if (cur->children[LEFT] != NULL)
	{
		cur = cur->children[LEFT];
		while (cur->children[RIGHT] != NULL)
		{
			cur = cur->children[RIGHT];
		}
		next.data = (void *)cur;
	}
	else
	{
		/* search for first left parent */
		while (parent->children[RIGHT] != cur)
		{
			cur = parent;
			parent = cur->parent;
		}		
		next.data = (void *)parent;
	}
	return (next);
}

/********************************************
Returns 1 if iterators are same. 0 otherwise 
********************************************/
int BstIsSameIter(bst_iter_t iter1, bst_iter_t iter2)
{
	return (iter1.data == iter2.data);
}

/********************
Returns data of node 
*********************/
void *BstGetData(bst_iter_t iter)
{
	assert(iter.data != NULL);
	
	return (((bst_node_t *)iter.data)->data);
}

/********************************************
Returns 1 if empty. 0 otherwise - O(1)
********************************************/
int BstIsEmpty(const bst_t *tree)
{
	assert(tree != NULL);
	
	return (NULL == tree->end.children[LEFT]);
}

/****************************************************
Returns the iterator which holds the requested data.
If data is not found, returns an end iterator.
Complexity: O(n)
*****************************************************/
bst_iter_t BstFind(const bst_t *tree, void *to_find)
{
	bst_iter_t ret = {0};
	const bst_node_t *cur = NULL;
	
	assert(tree != NULL);
	
	/* tree's root */
	cur = tree->end.children[LEFT];
	
	while (cur != NULL)
	{
		if (0 == tree->is_before(cur->data, to_find, tree->params))
		{
			if (0 == tree->is_before(to_find, cur->data, tree->params))
			{
				/* match found */
				ret.data = (void*)cur;
				return (ret);
			}
			/* go to left sub-tree */
			cur = cur->children[LEFT];
		}
		else
		{
			/* go to right sub-tree */
			cur = cur->children[RIGHT];
		}
	}
	return (BstEnd(tree));
}


/************************************************
Returns end iter if fails. Complexity: O(n)
************************************************/
bst_iter_t BstInsert(bst_t *tree, void *data)
{
	bst_iter_t ret = {0};
	bst_node_t *cur = NULL;
	bst_node_t *parent = NULL;
	bst_node_t *new_node = NULL;
	int before_res = 0;
	
	assert(tree != NULL);
	assert(BstIsSameIter(BstEnd(tree), BstFind(tree, data)));
	
	new_node = (bst_node_t *)malloc(sizeof(*new_node));
	if (NULL == new_node)
	{
		return (BstEnd(tree));
	}
	/* tree's root */
	cur = tree->end.children[LEFT];
	parent = &tree->end;
	
	while (cur != NULL)
	{
		parent = cur;
		before_res = tree->is_before(cur->data, data, tree->params);
		cur = cur->children[before_res];
	}
	new_node->data = data;
	new_node->parent = parent;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	
	parent->children[before_res] = new_node;
	
	ret.data = (void*)new_node;
	return (ret);
}

/*********************************************************
Once do_func fails, it stops and return a failure status.
Complexity: O(n)
*********************************************************/					
int BstForEach(bst_iter_t from,			
			   bst_iter_t to,			
			   int (*do_func)(void *data, void *params),			
			   void *params)
{
	int status = 0; /* success */
	bst_iter_t iter = from;
	
	assert(do_func != NULL);
	
	while (!BstIsSameIter(iter, to) && (0 == status))
	{
		status = do_func(BstGetData(iter), params);
		iter = BstNext(iter);
	}
	return (status);
}

/*********************************************************
Destroy tree. post order traversal
Complexity: O(n)
*********************************************************/
void BstDestroy(bst_t *tree)
{
    bst_node_t *cur = NULL;
    bst_node_t *parent = NULL;
    int pos = 0;
    
    assert(tree != NULL);
   
    cur = tree->end.children[LEFT];
    
    while (!BstIsEmpty(tree))
    {
        /* visit left subtree */
        if ((cur->children[LEFT] != NULL))
        {
        	cur = cur->children[LEFT];
        }
        /* visit right subtree */
        else if (cur->children[RIGHT] != NULL)                 
        {
        	cur = cur->children[RIGHT];
        }
        else
        {
            pos = RightOrLeftSon(cur);
            parent = cur->parent;
            parent->children[pos] = NULL;
            
            /* delete node */
            free(cur);
            cur = parent;
        }
    }    
    free(tree); tree = NULL;
}


/*****************************
returns erased element data
******************************/	
void *BstErase(bst_iter_t to_remove)
{
    bst_node_t *node = (bst_node_t *)(to_remove.data);
    bst_node_t *next_node = NULL;
    void *data = BstGetData(to_remove);
    int pos = RightOrLeftSon(node);
    
    assert(to_remove.data != NULL);
    
    /* Removed node is a leaf */
    if((NULL == node->children[LEFT]) && (NULL == node->children[RIGHT]))
    {
        node->parent->children[pos] = NULL;
    }
    
    /* Removed node has two sons */
    else if((node->children[LEFT] != NULL) && (node->children[RIGHT] != NULL))
    {
        /* get Next node */
        bst_iter_t to_be_removed_iter = BstNext(to_remove);
        /* copy data from next node to original node */
        node->data = BstGetData(to_be_removed_iter); 
        next_node = (bst_node_t *)(to_be_removed_iter.data);
        /* check if next node is right or left */
        pos = RightOrLeftSon(next_node);
        /* 'next node' parent is pointing to 'next node' right child */
        next_node->parent->children[pos] = next_node->children[RIGHT];
        if(next_node->children[RIGHT] != NULL)
        {
        	next_node->children[RIGHT]->parent = next_node->parent;
        }
        node = next_node;
    }
    
    /* Removed node has one son */
    else
    {
        /* find if removed node has right or left son */
        int dirc = (node->children[LEFT] != NULL) ? (LEFT) : (RIGHT);        
        /* parent of removed node is pointing to removed node's child */
        node->parent->children[pos] = node->children[dirc];        
        /* update child's new parent */
        node->children[dirc]->parent = node->parent;
    }
    
    free(node); node = NULL;
    
    return (data);
}

/*********************************************
returns tree size. pre order traversal - O(n)
*********************************************/
size_t BstSize(const bst_t *tree)
{
	bst_node_t *cur = NULL;
	bst_node_t *parent = NULL;
    int pos = 0;
    size_t count = 0;
    
    assert(tree != NULL);
   
    if (BstIsEmpty(tree))
    {
    	/* in case of an empty tree */
    	return (count);
    }
    
    cur = tree->end.children[LEFT];
    ++count;
    
    while (cur != &tree->end)
    {
        /* has left child - visit left subtree */
        if (cur->children[LEFT] != NULL)
        {
        	cur = cur->children[LEFT];
        	++count;
        }
        /* has right child - visit right subtree */
        else if (cur->children[RIGHT] != NULL)                 
        {
        	cur = cur->children[RIGHT];
        	++count;
        }
        else
        {
            /* save position of node (left or right) */
            pos = RightOrLeftSon(cur);
            
            /* go up to parent */
            cur = cur->parent;
			
			/* if parent doesn't have right child or already have been there */
			/* search for first prev parent that has right child that was not counted yet */
            while ((cur != &tree->end) && ((NULL == cur->children[RIGHT]) || (pos == RIGHT)))
 			{
				pos = RightOrLeftSon(cur);
	        	parent = cur->parent;
	        	if ((parent->children[RIGHT] != cur) && (parent->children[RIGHT] != NULL))
	        	{
	        		cur = parent;
	        		break;
	        	}
	        	cur = parent;
 			}
			/* parent has right child - move to it */
            if (cur != &tree->end)
            {
            	cur = cur->children[RIGHT];
        		++count;
            }
        }
    }
    return (count);
}


/************************************************************
Size of tree - dyn_vec technique. pre order traversal - O(n)
*************************************************************/
size_t BstSizeDynVec(const bst_t *tree)
{
    dyn_vec_t *vec = NULL;
    bst_node_t *cur = NULL;
    size_t count = 0;
   
    assert(tree != NULL);
   
    if (BstIsEmpty(tree))
    {
    	/* in case of an empty tree */
    	return (count);
    }
 
    vec = DynVecCreate(sizeof(cur), 1);
    if (NULL == vec)
    {
        return (1);
    }
	
	cur = tree->end.children[LEFT];
	DynVecPushBack(vec, &cur);
	
	while (0 != (DynVecSize(vec)-1)) 
    {
    	cur = *(bst_node_t **)DynVecGetItemAddress(vec, DynVecSize(vec)-1);
    	DynVecPopBack(vec);
    	++count;
  
    	/* add children to dyn vec */
    	if (cur->children[RIGHT] != NULL)
    	{
    		DynVecPushBack(vec, &(cur->children[RIGHT]));
    	}
    	
    	if (cur->children[LEFT] != NULL)
    	{
    		DynVecPushBack(vec, &(cur->children[LEFT]));
    	}
    
    }
	
	DynVecDestroy(vec);
	
	return (count);
}
