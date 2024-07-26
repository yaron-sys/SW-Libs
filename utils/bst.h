#ifndef BST_H_					
#define BST_H_					
					
#include <stddef.h> /* size_t */					
					
struct bst_iter					
{					
	void *data;				
};					
					
typedef struct bst_iter bst_iter_t;					
typedef struct bst bst_t;					
									
					
bst_t *BstCreate(int (*is_before)(const void *data1, const void *data2, void *params),					
				void *params);	
					
/* tree traversal - post order */					
void BstDestroy(bst_t *tree);					
					
/* tree traversal - pre order */					
size_t BstSize(const bst_t *tree);					

int BstIsEmpty(const bst_t *tree);					
								
/* smallest element in the tree */					
bst_iter_t BstBegin(const bst_t *tree);					
					
/* next of last element in the tree */					
bst_iter_t BstEnd(const bst_t *tree);					
					
/* undefined behaviour on End */					
bst_iter_t BstNext(bst_iter_t current);					
					
/* undefined behaviour on Begin */					
bst_iter_t BstPrev(bst_iter_t current);					
					
int BstIsSameIter(bst_iter_t iter1, bst_iter_t iter2);					
					
void *BstGetData(bst_iter_t iter);					
					
bst_iter_t BstInsert(bst_t *tree, void *data);					
					
/* returns erased element data */					
void *BstErase(bst_iter_t to_remove);					
					
/* stops on first failure, tree traversal - in order  */					
int BstForEach(					
		bst_iter_t from,			
		bst_iter_t to,			
		int (*do_func)(void *data, void *params),			
		void *params);			
					
bst_iter_t BstFind(const bst_t *tree, void *to_find);	

/* size of tree - method 2 (dyn vec) */
size_t BstSizeDynVec(const bst_t *tree);				
					
#endif /* BST_H_ */	
