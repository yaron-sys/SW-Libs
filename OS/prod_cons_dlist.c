#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

#define DLIST_SIZE 10
#define NUM_OF_THREADS 3
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
              
typedef struct params_thread 
{ 
	dlist_t *dlist;
	int data[DLIST_SIZE];
	sem_t semaphore;
	pthread_mutex_t lock;
		
}params_thread_t;

/*******************************************************************************
	Function for consumer thread
*******************************************************************************/
static void *Reader(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	void *data = NULL;
	int i = 0;
	
	for (i = 0; i < DLIST_SIZE; ++i)
	{
		/************** start critical section **************/
		sem_wait(&(p->semaphore));
	
		/* start critical section */
		pthread_mutex_lock(&(p->lock));
			
		/* Remove node from list and print data */
		data = DlistPopFront(p->dlist);
		printf("%d ", *(int*)data);
	
		pthread_mutex_unlock(&(p->lock));
		/************** end critical section **************/
	}
		
	return (NULL);  
}

/*******************************************************************************
	Function for producer thread 
*******************************************************************************/
static void *Writer(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	int i = 0;
	
	for (i = 0; i < DLIST_SIZE; ++i)
	{
		/************** start critical section **************/
		pthread_mutex_lock(&(p->lock));
	
		/* Append node to list */
		DlistPushBack(p->dlist, &(p->data[i]));

		pthread_mutex_unlock(&(p->lock));
		/************** end critical section **************/
		
		/* sem post */
		sem_post(&(p->semaphore));
	}

	return (NULL);  
}

/* n producers , n consumers - using 1 mutex & 1 semaphore */
int main(int argc, char *argv[])  
{ 
	int i = 0;
	pthread_t producers[NUM_OF_THREADS] = {0};
	pthread_t consumers[NUM_OF_THREADS] = {0};
	params_thread_t pa_thread = {0};
	
	/* params struct initialization */
	pa_thread.dlist = DlistCreate();
	if (pa_thread.dlist == NULL)
	{
		handle_error("malloc");
	}
	for (i = 0; i < DLIST_SIZE; ++i)
	{
		pa_thread.data[i] = i + 1;
	}
	if (pthread_mutex_init(&pa_thread.lock, NULL))
	{
		DlistDestroy(pa_thread.dlist);
		handle_error("pthread_mutex_init");
	}
	if (sem_init(&pa_thread.semaphore, 0, 0))
	{
		DlistDestroy(pa_thread.dlist);
		pthread_mutex_destroy(&pa_thread.lock);
		handle_error("sem_init");
	}
	
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_create (&producers[i], NULL, &Writer, &pa_thread); 
		pthread_create (&consumers[i], NULL, &Reader, &pa_thread);
	}
	
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join (producers[i], NULL);  
		pthread_join (consumers[i], NULL);  
	}

	DlistDestroy(pa_thread.dlist);
	pthread_mutex_destroy(&pa_thread.lock);
	sem_destroy(&pa_thread.semaphore);
	
	return (0);  
}
