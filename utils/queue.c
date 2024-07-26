#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */

#include "queue.h"
#include "slist.h"


/* queue struct manager */
struct queue
{
	slist_node_t *last; /* points to last element */
	slist_node_t *head; /* points to dummy */
};


/* Creates a queue instance */
queue_t *QueueCreate(void)
{
	queue_t *queue = (queue_t *) malloc(sizeof(*queue));

	if (NULL == queue)
	{
		return (NULL);
	}
	
	/* initiate head field */
	queue->head = SListCreateAndInitNode(NULL, NULL);
	
	if (NULL == queue->head)
	{
		free(queue);
		queue = NULL;
		
		return (NULL);
	}

	/* initiate last field */
	queue->last = queue->head;

	return (queue);
}

/* Destroys a queue instance - O(n) */
void QueueDestroy(queue_t *queue)
{
	/* destroy all nodes in queue */
	SListFreeAll(queue->head);
	
	/* destroy queue */
	free(queue);
	queue = NULL;
}


/* Queue size - O(n) */
size_t QueueSize(const queue_t *queue)
{
	assert(queue != NULL);
	
	/* return size without dummy node */
	return (SListSize(queue->head) - 1);
}


/* Is empty - O(1) */
int QueueIsEmpty(const queue_t *queue)
{
	assert(queue != NULL);
	
	return (queue->head == queue->last);
}


/* push element to the end of the queue returns 0 on sucess or 1 on failure - O(1) */
int QueueEnqueue(queue_t *queue, void *data)
{
	slist_node_t *node = NULL;
	
	assert(queue != NULL);
	
	/* creates a node */
	node = SListCreateAndInitNode(data, NULL);
	
	if (NULL == node)
	{
		return (1);
	}
	
	/* Insert after last element and get new tail */
	queue->last = SListInsertAfter(node, queue->last);

	return (0);
}


/* Removes first element in queue and returns its data - O(1) */
void *QueueDequeue(queue_t *queue)
{
	void *data = NULL;
	slist_node_t *deq_node = NULL;
	
	assert(queue != NULL);
	
	if (1 == QueueIsEmpty(queue))
	{
		return (NULL);
	}
	
	/* saves dequeued node and data */
	deq_node = SListRemoveAfter(queue->head);
	data = deq_node->data;

	if (queue->last == deq_node)
	{
		/* queue becomes empty if last element is removed */
		queue->last = queue->head;
	}
	
	/* free dequeued node */
	free(deq_node);
	deq_node = NULL;
	
	return (data);
}


/* Returns first element's data in queue or NULL if empty - O(1) */
void *QueuePeek(queue_t *queue)
{	
	assert(queue != NULL);
	
	if (1 == QueueIsEmpty(queue))
	{
		return (NULL);
	}
	
	return (queue->head->next->data);
}


/* Append all elments from src to destination's tail. finally src becomes empty - O(1) */
void QueueAppend(queue_t *queue_dest, queue_t *queue_src)
{	
	assert(queue_dest != NULL);
	assert(queue_src != NULL);

	if (QueueIsEmpty(queue_src) != 1)
	{
		/* dest tail points to src head */
		queue_dest->last->next = queue_src->head->next;
	
		/* dest gets new tail */
		queue_dest->last = queue_src->last;
	
		/* src queue becomes empty */
		queue_src->last = queue_src->head;
		queue_src->head->next = NULL;
	}	
}
