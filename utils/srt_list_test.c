#include <stdio.h>

#include "srt_list.h"

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


int IsBefore(const void *data1, const void *data2, void *params)
{
	printf("bla.\n");
	return (0);
}



int main(int argc, char *argv[])
{
	void *params = NULL;	
	
	/* Creating a list */
	srt_list_t *sorted_list = SrtListCreate(params, IsBefore);
	
	/* Creating an iter struct */
	srt_list_iter_t my_iter = {0};
	
	/* size 0 */
	0 == SrtListSize(sorted_list) ? printf("1. Good\n") : printf("1. Bad\n");
	
	/* empty - true */
	1 == SrtListIsEmpty(sorted_list) ? printf("2. Good\n") : printf("2. Bad\n");
	
	/* iter is begin */
	my_iter = SrtListBegin(sorted_list);
	
	/* begin == end (empty list) */
	SrtListBegin(sorted_list).info == SrtListEnd(sorted_list).info ? printf("3. Good\n") : printf("3. Bad\n");
	
	
	/* insert into list */
	/*my_iter = SrtListInsert(sorted_list, void *data)*/
/*
	srt_list_iter_t iter = {0};

	assert(srt_list != NULL);
	assert(srt_list->list != NULL);
	assert(srt_list->is_before != NULL);
	
	iter = SrtListBegin(srt_list);
	while ((0 == SrtListIsSameIter(iter, SrtListEnd(srt_list))) &&
	       (1 == srt_list->is_before(SrtListGetData(iter), data, srt_list->params)))
	{
		iter = SrtListNext(iter);
	}

	iter.info = (struct srt_list_iter_info*) DlistInsert(srt_list->list, (dlist_iter_t)iter.info, data);
	return (iter);

}
*/
	
	/* Destroy list */
	SrtListDestroy(sorted_list);
																	


	return (0);
}

