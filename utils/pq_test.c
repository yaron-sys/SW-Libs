#include <stdio.h>

#include "pq.h"


int IsBefore(const void *data1, const void *data2, void *params)
{
	return (*(int *)data1 < *(int *)data2);
}


/* verify that particular data in pq is match to the given data */
int is_match(const void *node_data, const void *to_find, void *params)
{
	if(*(int*)node_data == *(int*)to_find)
	{
		return (1);
	}
	return (0);
}



int main(int argc, char *argv[])
{
	size_t i = 0;
	void *params = NULL;
	int x = 5;
	int y = 2;
	int z = 35;
	int r = 10;
	int m = 1;
	int f = -15;
	int k = 0;
	
	pq_t *myq = PQCreate(params, &IsBefore);
	
	0 == PQSize(myq) ? printf("1- :)\n") : printf("1- :(\n");
	
	1 == PQIsempty(myq) ? printf("2- :)\n") : printf("2- :(\n");
	
	0 == PQEnqueue(myq, &x) ? printf("3- :)\n") : printf("3- :(\n"); /* 5 */
	
	5 == *(int*)PQPeek(myq) ? printf("4- :)\n") : printf("4- :(\n");
	
	1 == PQSize(myq) ? printf("5- :)\n") : printf("5- :(\n");
	
	0 == PQIsempty(myq) ? printf("6- :)\n") : printf("6- :(\n");
	
	0 == PQEnqueue(myq, &y) ? printf("7- :)\n") : printf("7- :(\n"); /* 2, 5 */
	
	2 == *(int*)PQPeek(myq) ? printf("8- :)\n") : printf("8- :(\n");
	
	0 == PQEnqueue(myq, &z) ? printf("9- :)\n") : printf("9- :(\n"); /* 2, 5, 35 */
	
	2 == *(int*)PQPeek(myq) ? printf("10- :)\n") : printf("10- :(\n");
	
	3 == PQSize(myq) ? printf("11- :)\n") : printf("11- :(\n");
	
	0 == PQEnqueue(myq, &r) ? printf("12- :)\n") : printf("12- :(\n"); /* 2, 5, 10, 35 */
	
	0 == PQEnqueue(myq, &k) ? printf("12.1- :)\n") : printf("12.1- :(\n"); /* 0, 2, 5, 10, 35 */
	
	0 == *(int*)PQPeek(myq) ? printf("13- :)\n") : printf("13- :(\n");
	
	0 == *(int*)PQDequeue(myq) ? printf("14- :)\n") : printf("14- :(\n"); /* 2, 5, 10, 35 */
	
	4 == PQSize(myq) ? printf("15- :)\n") : printf("15- :(\n");
	
	0 == PQEnqueue(myq, &m) ? printf("16- :)\n") : printf("16- :(\n");  /* 1, 2, 5, 10, 35 */
	
	5 == PQSize(myq) ? printf("17- :)\n") : printf("17- :(\n");
	
	1 == *(int*)PQPeek(myq) ? printf("18- :)\n") : printf("18- :(\n");
	
	5 == *(int*)PQRemove(myq, &x, params, is_match) ? printf("19- :)\n") : printf("19- :(\n"); /* 1, 2, 10, 35 */
	
	1 == *(int*)PQDequeue(myq) ? printf("20- :)\n") : printf("20- :(\n"); /* 2, 10, 35 */
	
	2 == *(int*)PQPeek(myq) ? printf("21- :)\n") : printf("21- :(\n");
	
	2 == *(int*)PQDequeue(myq) ? printf("22- :)\n") : printf("22- :(\n"); /* 10, 35 */
	
	10 == *(int*)PQPeek(myq) ? printf("23- :)\n") : printf("23- :(\n");
	
	2 == PQSize(myq) ? printf("24- :)\n") : printf("24- :(\n");
	
	0 == PQEnqueue(myq, &f) ? printf("25- :)\n") : printf("25- :(\n"); /* -15, 10, 35 */
	
	0 == PQEnqueue(myq, &f) ? printf("26- :)\n") : printf("26- :(\n"); /* -15, -15, 10, 35 */
	
	0 == PQEnqueue(myq, &f) ? printf("27- :)\n") : printf("27- :(\n"); /* -15, -15, -15, 10, 35 */
	
	-15 == *(int*)PQPeek(myq) ? printf("28- :)\n") : printf("28- :(\n");
	
	PQClear(myq);
	
	0 == PQSize(myq) ? printf("29- :)\n") : printf("29- :(\n");
	
	1 == PQIsempty(myq) ? printf("30- :)\n") : printf("30- :(\n");
	
	for(i = 0; i < 100; ++i)
	{
		if (1 == PQEnqueue(myq, &x))
			break;
	}
	100 == PQSize(myq) ? printf("31- :)\n") : printf("31- :(\n");

	PQDestroy(myq);

	printf("\nNow check valgrind to finish test.\n");


	return (0);
}

