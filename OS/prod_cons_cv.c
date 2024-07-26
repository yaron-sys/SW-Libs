#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE (20)
#define NUM_OF_CONS (100)
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
              
typedef struct params_thread 
{
	char *str_arr[SIZE];
	size_t ctr;
	pthread_cond_t condition;
	sem_t semaphore;
	pthread_mutex_t lock;
		
}params_thread_t;

/*******************************************************************************
	Function for n consumer threads
*******************************************************************************/
void *ConsumersFunc(void *par)
{
	params_thread_t *p = (params_thread_t *)par;
	size_t i = 0;
	
	while (1)
	{
		sem_wait(&(p->semaphore));
		pthread_mutex_lock(&(p->lock));
		
		printf("%s - %lu\n", p->str_arr[i], pthread_self());
		i = (i + 1) % SIZE;
		
		--(p->ctr);
		sem_post(&(p->semaphore));
		pthread_cond_wait(&(p->condition), &(p->lock));
	
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
	static const char *buffer[] = 
	{
		"one", "two", "three", "four", "five",
		"six", "seven", "eight", "nine", "ten",
		"elven", "twelve", "thirteen", "fourteen",
		"fifteen", "sixteen", "seventeen", "eighteen",
		"nineteen", "twenty"
	};
	
	while (1)
	{
		if (p->ctr == 0)
		{
			pthread_mutex_lock(&(p->lock));
			
			p->str_arr[i] = (char *)buffer[i];	
			i = (i + 1) % SIZE;
			p->ctr = NUM_OF_CONS;
			sem_post(&(p->semaphore));
		
			pthread_cond_broadcast(&(p->condition));	
			
			pthread_mutex_unlock(&(p->lock));
		}
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
	params_thread_t pa_thread = {{0}};

	/* params struct initialization */
	if (pthread_mutex_init(&pa_thread.lock, NULL))
	{
		handle_error("pthread_mutex_init");
	}
	if (sem_init(&pa_thread.semaphore, 0, 0))
	{
		pthread_mutex_destroy(&pa_thread.lock);
		handle_error("sem_init");
	}
	if (pthread_cond_init(&pa_thread.condition, NULL))
	{
		pthread_mutex_destroy(&pa_thread.lock);
		sem_destroy(&pa_thread.semaphore);
		handle_error("pthread_cond_init");
	}
	pa_thread.ctr = 0;

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
	if (sem_destroy(&pa_thread.semaphore))
	{
		handle_error("sem_destroy");
	}	
	if (pthread_cond_destroy(&pa_thread.condition))
	{
		handle_error("pthread_cond_destroy");
	}
	
	return (0);  
}
