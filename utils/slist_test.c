#include <stdio.h>
#include "slist.h"


void print_data(slist_node_t *head)
{
	slist_node_t *iter = head;
	
	while(iter != NULL)
	{
		printf("%d ", *(int*)(iter->data));
		iter = iter->next;
	}
}


int main(int argc, char *argv[])
{
	int a = 1, b = 2, c = 3, d = 10, e = 50;
	size_t size = 0;
	
	slist_node_t *person1 = NULL;
	slist_node_t *person2 = NULL;
	slist_node_t *person3 = NULL;
	slist_node_t *person4_insert = NULL;
	slist_node_t *person5_insert = NULL;
	
	/* Create Nodes */
	person1 = SListCreateAndInitNode(&a,NULL);
	person2 = SListCreateAndInitNode(&b,person1);
	person3 = SListCreateAndInitNode(&c,person2);
	
	/* create nodes for insertions */
	person4_insert = SListCreateAndInitNode(&d,NULL);
	person5_insert = SListCreateAndInitNode(&e,NULL);
	
	/**************************************************************************/
	
	/* Prints nodes data */
	printf("Linked list data: ");
	print_data(person3);
	printf("\n");
	
	/**************************************************************************/
	
	/* list size */
	size = SListSize(person3);
	printf("Size is: %lu\n", size);
	
	/**************************************************************************/

	/* Insert node before */
	SListInsert(person4_insert, person2);
	
	/* Prints nodes data after insertion (before) */
	printf("\nInsertion new node before node 2.\nLinked list data: ");
	print_data(person3);
	printf("\n");
	
	/* size */
	size = SListSize(person3);
	printf("Size is: %lu\n", size);
	
	/**************************************************************************/
	
	/* Insert node after */
	SListInsertAfter(person5_insert, person4_insert);
	
	/* Prints nodes data after insertion (before) */
	printf("\nInsertion new node after node 3.\nLinked list data: ");
	print_data(person3);
	printf("\n");
	
	/* size */
	size = SListSize(person3);
	printf("Size is: %lu\n", size);
	
	/**************************************************************************/
	
	/* Flip list */
	person3 = SListFlip(person3);
	/* Prints nodes data */
	printf("\nReversed Linked list: ");
	print_data(person3);
	printf("\n");
	
	/* Free All */
	SListFreeAll(person3);

	return (0);
}

