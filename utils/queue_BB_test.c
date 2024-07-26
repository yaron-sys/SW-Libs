#include <stdio.h>

#include "queue.h"


int main(int argc, char *argv[])
{
	void *peak = NULL;
	void *deq = NULL;

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
	

	QueueEnqueue(q1, &a);
	QueueEnqueue(q1, &b);
	QueueEnqueue(q1, &c);
	
	QueueEnqueue(q2, &e);
	QueueEnqueue(q2, &f);
	QueueEnqueue(q2, &g);
	QueueEnqueue(q2, &h);


	QueueAppend(q1, q2);
	
	printf("\nsize q1: %lu\n", QueueSize(q1));
	printf("size q2: %lu\n", QueueSize(q2));
	
	printf("\nis q1 empty? %d\n", QueueIsEmpty(q1));
	printf("is q2 empty? %d\n", QueueIsEmpty(q2));
	

	peak = QueuePeek(q1);
	printf("\nq1 peek: %d\n", *(int*)peak);
	
	deq = QueueDequeue(q1);
	printf("\nq1 dequeue data: %d\n", *(int*)deq);


	peak = QueuePeek(q1);
	printf("\nq1 peek: %d\n", *(int*)peak);



	QueueDestroy(q1);
	QueueDestroy(q2);


	return (0);
}
