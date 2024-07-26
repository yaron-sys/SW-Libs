#include <stdio.h>

#include "avl.h"

/*
static void PrintNodesInOrder(avl_node_t *node)
{
    if (NULL == node)
    {
        return;
    }
    PrintNodesInOrder(node->children[LEFT]);
    printf("data: %d | height: %lu\n", *(int*)node->data, node->height);
   	PrintNodesInOrder(node->children[RIGHT]);
}
*/

typedef struct avl_node avl_node_t;

struct avl_node
{
	void 	   *data;
	int 		height;
	avl_node_t *children[2];
};

struct avl
{
	avl_node_t *root;
	int	       (*cmp)(const void *data1, const void *data2, void *params);
	void	   *params;
};

enum {LEFT = 0, RIGHT = 1};	


static int Cmp(const void *data1, const void *data2, void *params)
{
	return (*(int *)data1 - *(int *)data2);
}


void printGivenLevel(avl_node_t *root, int level)
{
    if (root == NULL)
    {
    	return;
    }
    if (level == 0)
    {
    	printf("%d|%d  ", *(int*)root->data, root->height);
    }
    else if (level > 0)
    {
        printGivenLevel(root->children[0], level-1);
        printGivenLevel(root->children[1], level-1);
    }
}

void PrintTree(avl_t *tree)
{
    int h = 0;
    int i = 0;
    
    if (tree->root == NULL)
    {
    	return;
	}
	h = tree->root->height;
    for (i = 0; i <= h; i++)
    {
        printGivenLevel(tree->root, i);
        printf("\n");
    }
}

void destroy(void)
{
	avl_t *tree = AvlCreate(Cmp, NULL);
	int arr[100] = {0};
	int i = 0;
	for(i = 0; i < 100; ++i)
	{
		arr[i] = i + 1;
		if (AvlInsert(tree, &arr[i]))
		{
			printf("Failed!\n");
			break;
		}
	}
	AvlDestroy(tree);
}


int main(int argc, char *argv[])
{

	int a = 50;
	int b = 30;
	int c = 20;
	int d = 40;
	int e = 45;
	int f = 70;
	int g = 60;
	int h = 80;

	
	avl_t *tree = NULL;
	int ret = 0;
	
	tree = AvlCreate(Cmp, NULL);
	
	1 == AvlIsEmpty(tree) ? printf("Good\n") : printf("Failed\n");
	
	0 == AvlSize(tree) ? printf("Good\n") : printf("Failed\n");
	
	-1 == AvlHeight(tree) ? printf("Good\n") : printf("Failed\n");
	
	ret = AvlInsert(tree, &a);
	0 == ret ? printf("Good\n") : printf("Failed\n");

	ret = AvlInsert(tree, &b);
	0 == ret ? printf("Good\n") : printf("Failed\n");

	ret = AvlInsert(tree, &c);
	0 == ret ? printf("Good\n") : printf("Failed\n");

	ret = AvlInsert(tree, &d);
	0 == ret ? printf("Good\n") : printf("Failed\n");
	
	ret = AvlInsert(tree, &e);
	0 == ret ? printf("Good\n") : printf("Failed\n");
	
	ret = AvlInsert(tree, &f);
	0 == ret ? printf("Good\n") : printf("Failed\n");
	
	ret = AvlInsert(tree, &g);
	0 == ret ? printf("Good\n") : printf("Failed\n");
	
	ret = AvlInsert(tree, &h);
	0 == ret ? printf("Good\n") : printf("Failed\n");
	
	0 == AvlIsEmpty(tree) ? printf("Good\n") : printf("Failed\n");
	
	8 == AvlSize(tree) ? printf("Good\n") : printf("Failed\n");
	
	3 == AvlHeight(tree) ? printf("Good\n") : printf("Failed\n");
	
	printf("\nWhat you got: after all insertions and cases of balances:\n");
	PrintTree(tree);
	
printf("\nWhat you should expect:\n\
45|3  \n\
30|1  60|2 \n\
20|0  40|0  50|0  70|1 \n\
80|0"); 

	/* erase 50 */
	printf("\n*********************************************\n");
	AvlErase(tree, &a);
	printf("\nwhat you got: after removing 50 (root):\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
45|2  \n\
30|1  70|1 \n\
20|0  40|0  60|0  80|0"); 
printf("\n*********************************************\n");

	/* erase 70 */
	AvlErase(tree, &f);
	printf("\nwhat you got: after removing 70:\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
45|2  \n\
30|1  80|1 \n\
20|0  40|0  60|0"); 
printf("\n*********************************************\n");
	
	/* erase 60 */
	AvlErase(tree, &g);
	printf("\nwhat you got: after removing 60:\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
45|2  \n\
30|1  80|0 \n\
20|0  40|0"); 
printf("\n*********************************************\n");
	
	/* erase 80 */
	AvlErase(tree, &h);
	printf("\nwhat you got: after removing 80:\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
30|2  \n\
20|0  45|1 \n\
40|0"); 
printf("\n*********************************************\n");
	
	/* erase 20 */
	AvlErase(tree, &c);
	printf("\nwhat you got: after removing 20:\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
40|1  \n\
30|0  45|0"); 
printf("\n*********************************************\n");

	/* erase 40 */
	AvlErase(tree, &d);
	printf("\nwhat you got: after removing 40:\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
45|1  \n\
30|0"); 
printf("\n*********************************************\n");
	
	/* erase 80 (does not exist) */
	AvlErase(tree, &h);
	printf("\nwhat you got: after removing 80 (does not exist):\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n\
45|1  \n\
30|0"); 
printf("\n*********************************************\n");

	/* erase 30 */
	AvlErase(tree, &b);
	printf("\nwhat you got: after removing 30:\n");
	PrintTree(tree);
printf("\nWhat you should expect:\n45|0"); 
printf("\n*********************************************\n");
	
	/* erase 45 */
	AvlErase(tree, &e);
	printf("\nwhat you got: after removing 45:\n");
	PrintTree(tree);
printf("\nWhat you should expect: nada\n"); 
printf("\n*********************************************\n");	

	AvlDestroy(tree);
	
	destroy();
	
	printf("\n\n-check valgrind-\n");
	
	
	
	return (0);
}

