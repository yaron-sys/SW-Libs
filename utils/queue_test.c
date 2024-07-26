#include <stdio.h>

#include "queue.h"
#include "slist.h"


struct queue
{
	slist_node_t *last;
	slist_node_t *head;
};



int do_func(void *params, void *data)
{
	printf("Queue: %d \n", *(int*)data);
	return(0);
}


int main(int argc, char *argv[])
{
	void *peak = NULL;
	void *deq = NULL;
	void *params = NULL;
	int a = 5;
	int b = 10;
	int c = 15;
	
	int e = 100;
	int f = 200;
	int g = 300;
	int h = 400;
	
	/* create a queue */
	queue_t *q1 = QueueCreate();
	queue_t *q2 = QueueCreate();
	
	printf("is q1 empty? %d\n", QueueIsEmpty(q1));
	printf("is q2 empty? %d\n\n", QueueIsEmpty(q2));
	
	QueueEnqueue(q1, &a);
	QueueEnqueue(q1, &b);
	QueueEnqueue(q1, &c);
	
	QueueEnqueue(q2, &e);
	QueueEnqueue(q2, &f);
	QueueEnqueue(q2, &g);
	QueueEnqueue(q2, &h);

	SListForEach(q1->head->next, do_func, params);
	printf("\n");
	SListForEach(q2->head->next, do_func, params);
	
	printf("\nsize q1: %lu\n", QueueSize(q1));
	printf("size q2: %lu\n", QueueSize(q2));
	
	printf("\nis q1 empty? %d\n", QueueIsEmpty(q1));
	printf("is q2 empty? %d\n", QueueIsEmpty(q2));
		

	printf("\nMOVING q2 to q1:\n");
	QueueAppend(q2, q1);
	
	printf("\nsize q1: %lu\n", QueueSize(q1));
	printf("size q2: %lu\n", QueueSize(q2));
	
	printf("\nis q1 empty? %d\n", QueueIsEmpty(q1));
	printf("is q2 empty? %d\n", QueueIsEmpty(q2));
	
	printf("\nq1:\n");
	SListForEach(q1->head->next, do_func, params);
	printf("\nand q2:\n");
	SListForEach(q2->head->next, do_func, params);
	
	
	peak = QueuePeek(q2);
	printf("\nq2 peek: %d\n", *(int*)peak);
	
	deq = QueueDequeue(q2);
	printf("\nq2 dequeue data: %d\n", *(int*)deq);

	printf("\nNew q2:\n");
	SListForEach(q2->head->next, do_func, params);

	peak = QueuePeek(q2);
	printf("\nq2 peek: %d\n", *(int*)peak);

	QueueDestroy(q1);
	QueueDestroy(q2);


	return (0);
}
