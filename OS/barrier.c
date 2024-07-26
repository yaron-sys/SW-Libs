#include <semaphore.h> /* semaphore */
#include <pthread.h>   /* threads */
#include <stdio.h>     /* printf */
#include <stdlib.h>    /* perror */

#define NUM_OF_CONS (100)
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
              
typedef struct params_thread 
{
	const char **buffer;
	size_t buff_size;
	char *str;
	size_t ctr;
	pthread_cond_t cv;
	sem_t can_write;
	pthread_mutex_t lock;
		
}params_thread_t;

/*******************************************************************************
	Function for n consumer threads
*******************************************************************************/
void *ConsumersFunc(void *par)
{
	params_thread_t *p = (params_thread_t *)par;
	
	while (1)
	{
		pthread_mutex_lock(&(p->lock));
		while (p->ctr == 0)
		{	
			pthread_cond_wait(&(p->cv), &(p->lock));	
		}
		pthread_mutex_unlock(&(p->lock));

		printf("%s - %lu\n", p->str, pthread_self());
		
		pthread_mutex_lock(&(p->lock));
		--(p->ctr);
		if (p->ctr > 0)
		{
			pthread_cond_wait(&(p->cv), &(p->lock));
		}
		else
		{
			/* unlock all the waiting threads */
			pthread_cond_broadcast(&(p->cv));
		
			/* unlock the writer */
			sem_post(&(p->can_write));
		}
		pthread_mutex_unlock(&(p->lock));
	}
	
	return (NULL);
}

/*******************************************************************************
	Function for 1 producer thread 
*******************************************************************************/
void *ProducerFunc(void *par)
{
	params_thread_t *p = (params_thread_t *)par;
	size_t i = 0;

	while (1)
	{
		sem_wait(&(p->can_write));
		
		p->str = (char *)(p->buffer[i]);	
		i = (i + 1) % p->buff_size;
		
		pthread_mutex_lock(&(p->lock));
		
		p->ctr = NUM_OF_CONS;
		pthread_cond_broadcast(&(p->cv));	
		
		pthread_mutex_unlock(&(p->lock));
	}

	return (NULL);  
}

/*******************************************************************************
		1 producer , n consumers - using 1 mutex, 1 semaphore, 1 cv 
*******************************************************************************/
int main(int argc, char *argv[])  
{ 
	int i = 0;
	pthread_t producer = {0};
	pthread_t consumers[NUM_OF_CONS] = {0};
	params_thread_t pa_thread = {0};
	const char *buffer[] =
	{
		"one", "two", "three", "four", "five",
		"six", "seven", "eight", "nine", "ten",
		"elven", "twelve", "thirteen", "fourteen",
		"fifteen", "sixteen", "seventeen", "eighteen",
		"nineteen", "twenty"
	};
	
	/* params struct initialization */
	if (pthread_mutex_init(&pa_thread.lock, NULL))
	{
		handle_error("pthread_mutex_init");
	}
	if (sem_init(&pa_thread.can_write, 0, 1))
	{
		pthread_mutex_destroy(&pa_thread.lock);
		handle_error("sem_init");
	}
	if (pthread_cond_init(&pa_thread.cv, NULL))
	{
		pthread_mutex_destroy(&pa_thread.lock);
		sem_destroy(&pa_thread.can_write);
		handle_error("pthread_cond_init");
	}
	pa_thread.ctr = 0;
	pa_thread.str = NULL;
	pa_thread.buffer = buffer;
	pa_thread.buff_size = sizeof(buffer) / sizeof(*buffer);
	
	pthread_create(&producer, NULL, &ProducerFunc, &pa_thread); 
	for (i = 0; i < NUM_OF_CONS; ++i)
	{
		pthread_create(&consumers[i], NULL, &ConsumersFunc, &pa_thread);
	}
	
	pthread_join(producer, NULL);  
	for (i = 0; i < NUM_OF_CONS; ++i)
	{
		pthread_join(consumers[i], NULL);  
	}
	
	/* destroy all initializations */
	if (pthread_mutex_destroy(&pa_thread.lock))
	{
		handle_error("pthread_mutex_destroy");
	}
	if (sem_destroy(&pa_thread.can_write))
	{
		handle_error("sem_destroy");
	}
	if (pthread_cond_destroy(&pa_thread.cv))
	{
		handle_error("pthread_cond_destroy");
	}
	
	return (0);  
}
