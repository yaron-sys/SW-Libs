#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "spin_lock.h"

#define SIZE 27

typedef struct params_thread 
{ 
	char str[SIZE];
	spin_lock_t *sl;  
}params_thread_t;

/* Function for consumer thread */
static void *Read(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	int i = 0;
	
	for (i = 0; p->str[i] != '\0' ; ++i)
	{
		/* critical section */
		SpinLockLock(p->sl); 	 
			
		printf("%c\n", p->str[i]);
		
		SpinLockUnlock(p->sl); 
	}
		
	return (NULL);  
}

/* Function for producer thread */
static void *Write(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	int i = 0;
	
	for (i = 0; i < SIZE-1; ++i)
	{
		/* critical section */
		SpinLockLock(p->sl);
	
		p->str[i] = (char)('a'+ i);
		
		SpinLockUnlock(p->sl); 
	}
	p->str[i] = '\0';
	
	return (NULL);  
}

/* 1 producer , 1 consumer - use busy waiting with spin lock */
int main(int argc, char *argv[])  
{ 
	pthread_t producer = {0};
	pthread_t consumer = {0};
	
	params_thread_t pa_thread;
	pa_thread.sl = SpinLockCreate();
	
	pthread_create (&producer, NULL, &Write, &pa_thread); 
	pthread_create (&consumer, NULL, &Read, &pa_thread);
	
	pthread_join (producer, NULL);  
	pthread_join (consumer, NULL);  

	printf("Finished\nstring: %s\n", pa_thread.str);
	
	SpinLockDestroy(pa_thread.sl);
	
	return (0);  
}
