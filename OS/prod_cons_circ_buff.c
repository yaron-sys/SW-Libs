#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 10
#define NUM_OF_THREADS 2
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
              
typedef struct params_thread 
{ 
	int circ_buff[BUF_SIZE];
	size_t read_idx;
	size_t write_idx;
	sem_t sem_not_over;  /* prevent overflow */
	sem_t sem_not_under; /* prevent underflow */
	pthread_mutex_t lock;
		
}params_thread_t;


/*******************************************************************************
	Function for consumer thread
*******************************************************************************/
void *Reader(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	int i = 0;
	
	for (i = 1; i <= 15; ++i)
	{
		/* sem wait */
		sem_wait(&(p->sem_not_under));
			
		/************** start critical section **************/
		pthread_mutex_lock(&(p->lock));
			
		/* read data from array */
		printf("buf[%lu] = %d\n", p->read_idx, p->circ_buff[p->read_idx]);
		
		/* promote read idx */
		p->read_idx = (p->read_idx + 1) % BUF_SIZE;
		
		pthread_mutex_unlock(&(p->lock));
		/************** end critical section **************/
				
		/* sem post */
		sem_post(&(p->sem_not_over));
	}
		
	return (NULL);  
}

/*******************************************************************************
	Function for producer thread 
*******************************************************************************/
void *Writer(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	int i = 0;
	
	for (i = 1; i <= 15; ++i)
	{
		/* sem wait */
		sem_wait(&(p->sem_not_over));
		
		/************** start critical section **************/
		pthread_mutex_lock(&(p->lock));
		
		/* write data to array */
		p->circ_buff[p->write_idx] = i;
		
		/* promote write idx */
		p->write_idx = (p->write_idx + 1) % BUF_SIZE;

		pthread_mutex_unlock(&(p->lock));
		/************** end critical section **************/
		
		/* sem post */
		sem_post(&(p->sem_not_under));
	}

	return (NULL);  
}

/* n producers , n consumers - using 1 mutex & 2 semaphores */
int main(int argc, char *argv[])  
{ 
	int i = 0;
	pthread_t producers[NUM_OF_THREADS] = {0};
	pthread_t consumers[NUM_OF_THREADS] = {0};
	params_thread_t pa_thread = {{0}};
	
	/* params struct initialization */
	pa_thread.read_idx = 0;
	pa_thread.write_idx = 0;
	if (pthread_mutex_init(&pa_thread.lock, NULL))
	{
		handle_error("pthread_mutex_init");
	}
	if (sem_init(&pa_thread.sem_not_over, 0, BUF_SIZE))
	{
		pthread_mutex_destroy(&pa_thread.lock);
		handle_error("sem_init");
	}
	if (sem_init(&pa_thread.sem_not_under, 0, 0))
	{
		pthread_mutex_destroy(&pa_thread.lock);
		sem_destroy(&pa_thread.sem_not_over);
		handle_error("sem_init");
	}

	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_create(&producers[i], NULL, &Writer, &pa_thread); 
		pthread_create(&consumers[i], NULL, &Reader, &pa_thread);
	}
	
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(producers[i], NULL);  
		pthread_join(consumers[i], NULL);  
	}
	
	pthread_mutex_destroy(&pa_thread.lock);
	sem_destroy(&pa_thread.sem_not_over);
	sem_destroy(&pa_thread.sem_not_under);
	
	return (0);  
}
