#include <stdio.h>

#include "dlist.h"


/* verify that particular data in dlist is found in array */
int Do_func(void* data, void* params)
{
	static int i = 0;
	int *node_data = (int*)data;
	int *my_param = (int*)params;
	
	if(*node_data == *(my_param+i))
	{
		++i;
		return(0); /* success */
	}
		
	else
	{
		return (1); /* fail */
	}
	
}


/* verify that particular data in dlist is match to the given data */
int is_match(const void *node_data, const void *to_find, void *params)
{
	if(*(int*)node_data == *(int*)to_find)
	{
		return (1);
	}
	return (0);
}


void PrintList(dlist_t *dlist)
{
	dlist_iter_t iter = NULL;
	
	printf("\nDList: ");
	
	for (iter = DlistBegin(dlist);  iter != DlistEnd(dlist);  iter = DlistNext(iter))
	{
		printf("%d ", *(int*)(DlistGetData(iter)));
	}
	
	printf("\n");
}



int main(int argc, char *argv[])
{
	int params[] = {28,17,-12,5,10};
	dlist_iter_t iter_1 = NULL;
	
	int data_1 = 5;
	int data_2 = 10;
	int data_3 = 17;
	int data_4 = 28;
	int data_5 = -12;
	int data_6 = 50;
	

	/* creates dlist */
	dlist_t *dlist_1 = DlistCreate();
	
	/* dlist is empty */
	DlistIsEmpty(dlist_1) == 1 ? printf("0. Good.\n") : printf("0. Bad.\n");
	
	/* size is 0 */
	DlistSize(dlist_1) == 0 ? printf("1. Good.\n") : printf("1. Bad.\n");
	
	/***************************************************************************************/
	
	/* insert 5 before end */
	iter_1 = DlistBegin(dlist_1);
	iter_1 = DlistInsert(dlist_1, iter_1, &data_1);
		
	/* dlist is not empty */
	DlistIsEmpty(dlist_1) == 0 ? printf("5. Good.\n") : printf("5. Bad.\n");
	
	/* size is 1 */
	DlistSize(dlist_1) == 1 ? printf("6. Good.\n") : printf("6. Bad.\n");
	
	/* get data */
	*(int*)(DlistGetData(iter_1)) == 5 ? printf("7. Good.\n") : printf("7. Bad.\n");
	
	
	/* insert 10 before 5:  should be 10, 5 */
	iter_1 = DlistInsert(dlist_1, iter_1, &data_2);
	
	/* size is 2 */
	DlistSize(dlist_1) == 2 ? printf("8. Good.\n") : printf("8. Bad.\n");
	
	/* get data */
	*(int*)(DlistGetData(iter_1)) == 10 ? printf("9. Good.\n") : printf("9. Bad.\n");
	
	
	/* insert 17 between 10 and 5 :  should be 10, 17, 5 */
	iter_1 = DlistNext(iter_1); /* iter is pointing to 5 */
	
	iter_1 = DlistInsert(dlist_1, iter_1, &data_3); /* insert before 5 */
	
	/* size is 3 */
	DlistSize(dlist_1) == 3 ? printf("10.Good.\n") : printf("10.Bad.\n");
	
	/* get data */
	*(int*)(DlistGetData(iter_1)) == 17 ? printf("11.Good.\n") : printf("11.Bad.\n");
	
	/***************************************************************************************/
	
	/* erase 17 : should be 10, 5 */
	iter_1 = DlistErase(iter_1);
	
	/* next element is 5 */
	*(int*)(DlistGetData(iter_1)) == 5 ? printf("12.Good.\n") : printf("12.Bad.\n");
	
	/* size is 2 */
	DlistSize(dlist_1) == 2 ? printf("13.Good.\n") : printf("13.Bad.\n");
	
	/***************************************************************************************/
	
	/* push back 28: should be 10, 5, 28 */
	iter_1 = DlistPushBack(dlist_1, &data_4);
	
	/* element data is 28 */
	*(int*)(DlistGetData(iter_1)) == 28 ? printf("14.Good.\n") : printf("14.Bad.\n");
	
	/* size is 3 */
	DlistSize(dlist_1) == 3 ? printf("15.Good.\n") : printf("15.Bad.\n");
	
	/***************************************************************************************/
	
	/* push front -12: should be -12, 10, 5, 28 */
	iter_1 = DlistPushFront(dlist_1, &data_5);
	
	/* element data is -12 */
	*(int*)(DlistGetData(iter_1)) == -12 ? printf("15.Good.\n") : printf("15.Bad.\n");
	
	/* size is 4 */
	DlistSize(dlist_1) == 4 ? printf("16.Good.\n") : printf("16.Bad.\n");
	
	/***************************************************************************************/
	
	/* pop back 28: should be -12, 10, 5 */

	/* removed node data is 28 */
	*(int*)(DlistPopBack(dlist_1)) == 28 ? printf("17.Good.\n") : printf("17.Bad.\n");
	
	/* size is 3 */
	DlistSize(dlist_1) == 3 ? printf("18.Good.\n") : printf("18.Bad.\n");
	
	/***************************************************************************************/
	
	/* pop front -12: should be 10, 5 */

	/* removed node data is -12 */
	*(int*)(DlistPopFront(dlist_1)) == -12 ? printf("19.Good.\n") : printf("19.Bad.\n");
	
	/* size is 2 */
	DlistSize(dlist_1) == 2 ? printf("20.Good.\n") : printf("20.Bad.\n");
	
	/***************************************************************************************/
	
	/* Erase all list until getting empty */
	for (iter_1 = DlistBegin(dlist_1);  iter_1 != DlistEnd(dlist_1);  iter_1 = DlistErase(iter_1));
	
	iter_1 == DlistEnd(dlist_1) ? printf("21.Good.\n") : printf("21.Bad.\n");
	
	/***************************************************************************************/
	
	/* Build list again */
	DlistPushBack(dlist_1, &data_1); /* 5 */
	DlistPushBack(dlist_1, &data_2); /* 5 10 */
	DlistPushFront(dlist_1, &data_3); /* 17 5 10 */
	DlistPushFront(dlist_1, &data_4); /* 28 17 5 10 */
	
	/* find 5 and return iter to it - should success */
	iter_1 = DlistFind(DlistBegin(dlist_1), DlistEnd(dlist_1), &data_1, params, is_match);
	iter_1 != DlistEnd(dlist_1) ? printf("22.Good.\n") : printf("22.Bad.\n");
	
	/* insert -12 before 5 (found from before). should be 28 17 -12 5 10 */
	DlistInsert(dlist_1, iter_1 ,&data_5); 
	
	/* using "for-each" to verify that our series is indeed 28 17 -12 5 10  - should success */
	if (0 == DlistForEach(DlistBegin(dlist_1), DlistEnd(dlist_1), Do_func, params))
	{
		printf("23.Good.\n");
	}
	else
	{
		printf("23.Bad.\n");
	}
	
	/* find 50 and return iter to it - should fail */
	iter_1 = DlistFind(DlistBegin(dlist_1), DlistEnd(dlist_1), &data_6, params, is_match);
	iter_1 == DlistEnd(dlist_1) ? printf("24.Good.\n") : printf("24.Bad.\n");
	
	/* insert 50 before to the end. should be 28 17 -12 5 10 50 */
	DlistInsert(dlist_1, iter_1 ,&data_6); 
	
	
	/* print */
	PrintList(dlist_1);
	
	
	/* destroy dlist_1 */
	DlistDestroy(dlist_1);
	
	printf("You have reached to end! now it's time to test with Valgrind :)\n");
	
	return (0);
}

