#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "spin_lock.h"

typedef struct params_thread 
{ 
	int ctr;  
	spin_lock_t *sl;  
}params_thread_t;

/* thread function */
static void *Calculate(void *par)  
{
	params_thread_t *p = (params_thread_t *)par;
	int i = 0;
	
	for (i = 0 ; i < 15000000; ++i)
	{
		SpinLockLock(p->sl); 
		 
		++(p->ctr);

		SpinLockUnlock(p->sl); 
	}
		
	return (NULL);  
}

int main(int argc, char *argv[])  
{ 
	pthread_t thread1 = {0};
	pthread_t thread2 = {0};
	
	params_thread_t pa_thread;  
	pa_thread.ctr = 0;
	pa_thread.sl = SpinLockCreate();
	
	pthread_create (&thread1, NULL, &Calculate, &pa_thread); 
	pthread_create (&thread2, NULL, &Calculate, &pa_thread); 
	
	pthread_join (thread1, NULL);  
	pthread_join (thread2, NULL);  

	printf("%d\n", pa_thread.ctr);
	
	SpinLockDestroy(pa_thread.sl);
	
	return (0);  
}
