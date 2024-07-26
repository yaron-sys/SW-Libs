#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */

#include "scheduler.h"
#include "scheduler_task.h"
#include "pq.h"


struct scheduler					
{					
	pq_t	*tasks;	        /* queue of tasks */
	task_t	*current_task;	/* current task in queue */	
	int keep_run;             /* 1 if scheduler is running, otherwise 0. */
	int self_remove;        /* 1 if task removes itself, otherwise 0.  */
};


/************************************************
Create new scheduler
*************************************************/
scheduler_t *SchedulerCreate(void)
{
	scheduler_t *new_scheduler = (scheduler_t*)malloc(sizeof(*new_scheduler));
	if(NULL == new_scheduler)
	{
		return (NULL);
	}

	new_scheduler->tasks = PQCreate(NULL, SchedulerTaskIsBefore);
	if(NULL == new_scheduler->tasks)
	{
		free(new_scheduler); new_scheduler = NULL;
		return (NULL);
	}
	
	new_scheduler->current_task = NULL;
	new_scheduler->keep_run = 0;
	new_scheduler->self_remove = 0;
	
	return (new_scheduler);
}
	
/************************************************
Destroy scheduler - O(n)
It is forbidden to destroy while running
*************************************************/			
void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler != NULL);	
	/* assert(1 == scheduler->keep_run); */
	if(scheduler->keep_run)
	{
		/* scheduler is already running */
		return;
	}
	
	
	/* Clear scheduler */
	SchedulerClear(scheduler);
	
	/* Destroy pq */
	PQDestroy(scheduler->tasks);
	
	/* Free scheduler */
	free(scheduler); scheduler = NULL;
}

/************************************************
Returns 1 if empty. otherwise 0. - O(1)
*************************************************/
int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler != NULL);
	
	return((PQIsempty(scheduler->tasks)) && (NULL == scheduler->current_task));
}

/************************************************
Add a task to scheduler - O(n)
Return uid of task in success.
Return invalid uid in failure.
*************************************************/		
uuid_t SchedulerAdd(scheduler_t *scheduler, int(*func)(void *params), void *params, unsigned long interval_sec)
{
	task_t *add_task = NULL;
	
	assert(scheduler != NULL);
	assert(func != NULL);
		
	/* creating a new task */
	add_task = SchedulerTaskCreate(func, params, interval_sec);
	if (NULL == add_task)
	{
		return(UuidGetInvalidID());
	}
	
	if (PQEnqueue(scheduler->tasks, add_task))
	{
		/* if failed - destroy task and return invalid uid */
		SchedulerTaskDestroy(add_task);
		return(UuidGetInvalidID());
	}
	
	return(SchedulerTaskGetId(add_task));
}

/************************************************
Clear all tasks from scheduler.
Except for the running task, if exists. - O(n)
*************************************************/
void SchedulerClear(scheduler_t *scheduler)
{
	void *data = NULL;
	
	assert(scheduler != NULL);
	
	/* dequeue each node (loop until empty) */
	while (! PQIsempty(scheduler->tasks))
	{
		/* remove node and get data */
		data = PQDequeue(scheduler->tasks);
		
		/* free task of each node (data) */
		SchedulerTaskDestroy((task_t*) data);
	}
	
	if (scheduler->current_task != NULL)
	{
		/* task erase itself */
		scheduler->self_remove = 1;
	}			
	
}

/***************************************************************
Remove a task - O(n)
Return 1 if not found. otherwise return 0
If task removes itself, turn on 'self_remove' flag and return 0.
****************************************************************/				
int SchedulerRemove(scheduler_t *scheduler, uuid_t uid)
{
	task_t *remove_task = NULL;
	
	assert(scheduler != NULL);

	remove_task = (task_t *)PQRemove(scheduler->tasks, &uid, NULL, SchedulerTaskIsMatch);
	
	if(NULL == remove_task)
	{
		if ((scheduler->current_task != NULL) && 
			(1 == SchedulerTaskIsMatch(scheduler->current_task, &uid, NULL)))
		{
			/* task erase itself */
			scheduler->self_remove = 1;
			return (0);
		}
		
		/* in case of not found */
		return (1);
	}
	
	/* remove if found */
	SchedulerTaskDestroy(remove_task);
	
	return(0);	   
}

/************************************************
Stop scheduler - turn 'keep_run' flag to 0
*************************************************/			
void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler != NULL);
	
	scheduler->keep_run = 0;
}

/************************************************
Run scheduler - O(n)
Return 1 if is already running or in failure
Return 0 if stopped running.
*************************************************/									
int SchedulerRun(scheduler_t *scheduler)
{
	assert(scheduler != NULL);
	
	if(scheduler->keep_run)
	{
		/* scheduler is already running */
		return (1);
	}
	/* change flag to 1 */
	scheduler->keep_run = 1;
	
	while((! SchedulerIsEmpty(scheduler)) && (scheduler->keep_run))
	{	
		/* poping out head node and getting task */
		scheduler->current_task = (task_t *)PQDequeue(scheduler->tasks);
		
		/* run task */
		if (0 == SchedulerTaskRun(scheduler->current_task) && (0 == scheduler->self_remove))
		{
			/* task should be occured again - update next invocation */
			SchedulerTaskUpdate(scheduler->current_task);
		
			/* insert back to pq */
			if (1 == PQEnqueue(scheduler->tasks, scheduler->current_task))
			{
				/* if failed - destroy task and return 1 */
				SchedulerTaskDestroy(scheduler->current_task);
				scheduler->current_task = NULL;
				scheduler->keep_run = 0;
				return(1);
			}
		}
		else
		{
			/* if task should not occure again destroy it */
			SchedulerTaskDestroy(scheduler->current_task);
		}
		
		scheduler->current_task = NULL;
		scheduler->self_remove = 0;
	}
	
	scheduler->keep_run = 0; /* change flag to 0 */

	return (0);
}

