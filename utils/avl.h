#ifndef AVL_H_					
#define AVL_H_					
					
#include <stddef.h> /* size_t */					
					
typedef struct avl avl_t;

												
avl_t *AvlCreate(int (*cmp)(const void *data1, const void *data2, void *params),					
				 void *params);	
					
/* tree traversal - post order */					
void AvlDestroy(avl_t *tree);					
					
/* tree traversal - pre order */					
size_t AvlSize(const avl_t *tree);					

int AvlIsEmpty(const avl_t *tree);					
																	
int AvlInsert(avl_t *tree, void *data);					
					
/* returns erased element data */					
void AvlErase(avl_t *tree, const void *to_remove);						

/* NULL if not found */					
void *AvlFind(const avl_t *tree, const void *to_find);					

/* stops on first failure, tree traversal - in order  */					
int AvlForEach(avl_t *tree,		
			   int (*do_func)(void *data, void *params),			
			   void *params);				
		

int AvlHeight(const avl_t *tree);	
							
#endif /* AVL_H_ */		
