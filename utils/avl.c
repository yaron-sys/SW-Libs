#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "avl.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) >= (0) ? (a) : (-a))
#define SIDE (res > 0)

typedef struct avl_node avl_node_t;

struct avl_node
{
	void 	   *data;
	int 		height;
	avl_node_t *children[2];
};

struct avl
{
	avl_node_t  *root;
	int	       (*cmp)(const void *data1, const void *data2, void *params);
	void	    *params;
};

typedef enum {LEFT, RIGHT} direction_t;			

static int GetHeight(avl_node_t *node)
{
	return ((node != NULL) ? (node->height) : (-1));
}

static int CalcHeight(avl_node_t *node)
{
	assert(node != NULL);
	
	return (MAX(GetHeight(node->children[LEFT]), GetHeight(node->children[RIGHT])) + 1);
}

static int GetBalance(avl_node_t *node)
{
	if (NULL == node)
	{
		return (0);
	}
	
	return (GetHeight(node->children[LEFT]) - GetHeight(node->children[RIGHT]));
}

static avl_node_t *Rotate(avl_node_t *root, direction_t dir)
{
    direction_t opp_dir = !dir;
    avl_node_t *new_root = NULL;
	
	assert(root != NULL);
	
	/* set new root */
    new_root = root->children[opp_dir];
   
    /* rotation */
    root->children[opp_dir] = new_root->children[dir];
    new_root->children[dir] = root;
    
    /* update heights */
    root->height = CalcHeight(root);
    new_root->height = CalcHeight(new_root);
    
    return (new_root);
}

static avl_node_t *Balance(avl_node_t *node)
{
	/* get balance factor */	
	int bf = GetBalance(node);
	
	assert(node != NULL);

	/* LL / LR cases */
	if (bf > 1)
    {
    	if ((GetBalance(node->children[LEFT])) < 0)
    	{
    		/* LR case */
    		node->children[LEFT] = Rotate(node->children[LEFT], LEFT);
    	}
    	
    	return (Rotate(node, RIGHT));
 	}
 	
	/* RR / RL cases */
	else if (bf < -1)
    {
    	if ((GetBalance(node->children[RIGHT])) > 0)
    	{
    		/* RL case */
    		node->children[RIGHT] = Rotate(node->children[RIGHT], RIGHT);
    	}
    	
    	return (Rotate(node, LEFT));
 	}
	
 	return (node);
}

/*******************************************************************************
Create an Avl tree
********************************************************************************/
avl_t *AvlCreate(int (*cmp)(const void *data1, const void *data2, void *params),					
				 void *params)
{
	avl_t *avl = NULL;
	
	assert(cmp != NULL);
	
	avl = (avl_t *)malloc(sizeof(*avl));
	if (NULL == avl)
	{
		return (NULL);
	}
	
	/* Initialize fields */
	avl->root = NULL;
	avl->cmp = cmp;
	avl->params = params;
	
	return (avl);
}	

/*******************************************************************************
Destroy tree - post order traversal - O(n)
********************************************************************************/
static void AvlDestroyImpl(avl_node_t *node)
{
    if (node != NULL)
    {
		AvlDestroyImpl(node->children[LEFT]);    
		AvlDestroyImpl(node->children[RIGHT]);    
		free(node); node = NULL;
    }
}
									
void AvlDestroy(avl_t *tree)					
{
    assert(tree != NULL);
    
    AvlDestroyImpl(tree->root);   
    free(tree); tree = NULL;
}

/*******************************************************************************
Returns tree size - pre order traversal - O(n)
********************************************************************************/
static size_t AvlSizeImpl(avl_node_t *node)
{
    if (NULL == node)
    {
        return (0);
    }
    
    return (1 + AvlSizeImpl(node->children[LEFT]) + AvlSizeImpl(node->children[RIGHT]));
}
							
size_t AvlSize(const avl_t *tree)					
{
    assert(tree != NULL);
    
    return (AvlSizeImpl(tree->root));
}

/*******************************************************************************
Return 1 if avl is empty, 0 otherwise
********************************************************************************/
int AvlIsEmpty(const avl_t *tree)					
{
	assert(tree != NULL);
	
	return (NULL == tree->root);
}

/*******************************************************************************
					Insertion complexity - O(log(n))

1. Find place for insertion recursively
2. Return pointer of new node to calling function
3. link between parent and new node
4. calculate new heights for all nodes up to root
5. balance tree
********************************************************************************/																	
static avl_node_t *AvlInsertImpl(avl_node_t *root, 
						 		 avl_node_t *node_to_insert,
						 		 int (*cmp)(const void *data1, const void *data2, void *params),
						 		 void *params)
{
    int res = 0;
    
    if (NULL == root)
    {
        return (node_to_insert);
    }
   
    if ((res = cmp(node_to_insert->data, root->data, params)) != 0)
    {
    	root->children[SIDE] = AvlInsertImpl(root->children[SIDE], node_to_insert, cmp, params);
    }
    
	/* Update height of tree */
	root->height = CalcHeight(root);

	/* check if balance is neccessary */
	if (ABS(GetBalance(root)) > 1)
	{
		return (Balance(root));
	}
	
	return (root);
}

/*******************************************************************************
1. create new node with height 0 (leaf)
2. insert node to tree
3. return 0 in success. otherwise 1. 
********************************************************************************/	
int AvlInsert(avl_t *tree, void *data)					
{
	avl_node_t *node_to_insert = NULL;
	
	assert(tree != NULL);
	assert(NULL == AvlFind(tree, data));
	
	/* Create new node */
    node_to_insert = (avl_node_t *)calloc(1, sizeof(*node_to_insert));
    if (NULL == node_to_insert)
	{
		return (1);
	}
    node_to_insert->data = data;
	
	tree->root = AvlInsertImpl(tree->root, node_to_insert, tree->cmp, tree->params);
		
	return (0);
}


/*******************************************************************************
Find next node - go left until leaf is found
********************************************************************************/					
static avl_node_t *NextNode(avl_node_t *node)
{
	if (NULL == node->children[LEFT])
	{
		return (node);
	}
	
	return (NextNode(node->children[LEFT]));
}

/*******************************************************************************
					Deletion complexity - O(log(n))

1. Find node to delete in recursive order
2. once found: handle with 2 cases: 0,1 child or 2 childs
3. re-link between parents and childs
4. calculate new heights for all nodes up to root
5. balance tree
********************************************************************************/
static avl_node_t *AvlEraseImpl(avl_node_t *node, 
						 		const void *to_remove,
						 		int (*cmp)(const void *data1, const void *data2, void *params),
						 		void *params)
{
    int res = 0;
    
    if (NULL == node)
    {
		return (NULL);
    }

   	/* find node to erase */ 
   	if ((res = cmp(to_remove, node->data, params)) != 0)
	{
		node->children[SIDE] = AvlEraseImpl(node->children[SIDE], to_remove, cmp, params);
   	}
	/* found node to erase */
	else
	{
		/* if have one child or none */
		if ((NULL == node->children[LEFT]) || (NULL == node->children[RIGHT]))
		{
			avl_node_t *child = (node->children[LEFT] != NULL) ? (node->children[LEFT]) : (node->children[RIGHT]);
			free(node); node = NULL;
			return (child);
		}
		else
		{
			/* have 2 sons - find next node */
			avl_node_t *next = NextNode(node->children[RIGHT]);
			node->data = next->data;
			node->children[RIGHT] = AvlEraseImpl(node->children[RIGHT], next->data, cmp, params);
		}
	}
	
	/* Update height of node */
	node->height = CalcHeight(node);
	
	/* check if balance is neccessary */
	if (ABS(GetBalance(node)) > 1)
	{
		return (Balance(node));
	}
	
	return (node);
}


void AvlErase(avl_t *tree, const void *to_remove)						
{
	assert(tree != NULL);
	
	tree->root = AvlEraseImpl(tree->root, to_remove, tree->cmp, tree->params);
}

/*******************************************************************************
Returns found data. return NULL if not found
Complexity O(log(n))
********************************************************************************/
static void *AvlFindImpl(avl_node_t *node,
						 const void *to_find,
						 int (*cmp)(const void *data1, const void *data2, void *params),
						 void *params)
{
    int res = 0;
    
    if (NULL == node)
    {
        return (NULL);
    }

	res = cmp(to_find, node->data, params);
	
	if (0 == res)
	{	
		return (node->data);
	}
	
	return (AvlFindImpl(node->children[SIDE], to_find, cmp, params));
}
				
void *AvlFind(const avl_t *tree, const void *to_find)				
{
	assert(tree != NULL);
	
	return (AvlFindImpl(tree->root, to_find, tree->cmp, tree->params));
}

/*******************************************************************************
stops on first failure and return 1. otherwise return 0.
in order traversal - O(n)
********************************************************************************/
static int AvlForEachImpl(avl_node_t *node,
						  int (*do_func)(void *data, void *params),					
						  void *params)
{
    int status = 0;
    
    if (NULL == node)
    {
        return (0);
    }

	status = AvlForEachImpl(node->children[LEFT], do_func, params);
	
	if (0 == status)
	{
		if ((status = do_func(node->data, params)) != 0)
		{
			return (status);
		}
		status = AvlForEachImpl(node->children[RIGHT], do_func, params);
	}
	
	return (status);
}
					
int AvlForEach(avl_t *tree,					
			   int (*do_func)(void *data, void *params),					
			   void *params)		
{
	assert(tree != NULL);
	assert(do_func != NULL);
	
	return (AvlForEachImpl(tree->root, do_func, params));
}

/*******************************************************************************
Returns Avl height. Return -1 if tree is empty.
********************************************************************************/
int AvlHeight(const avl_t *tree)
{
    assert(tree != NULL);

    return (GetHeight(tree->root));
}

