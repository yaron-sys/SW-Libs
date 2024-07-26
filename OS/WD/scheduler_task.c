#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */
#include <time.h>    /* time */
#include <unistd.h>  /* sleep */

#include "scheduler_task.h"

struct task					
{					
	uuid_t			uid;	
	int				(*func)(void *params);	
	void			*params;	
	unsigned long	interval_sec;			
	time_t			next_invocation_time;	
};

/************************************************
Create new scheduler task
*************************************************/
task_t *SchedulerTaskCreate(int(*func)(void *params), void *params, unsigned long interval_sec)
{
	task_t *new_task = NULL;
	
	assert(func != NULL);
	
	new_task = (task_t *)malloc(sizeof(*new_task));
	if(NULL == new_task)
	{
		return(NULL);
	}
	
	new_task->uid = UuidCreate();
	new_task->func = func;
	new_task->params = params;
	new_task->interval_sec = interval_sec;
	new_task->next_invocation_time = time(NULL);
	
	return (new_task);
}

/************************************************
Destroy a task
*************************************************/
void SchedulerTaskDestroy(task_t *task)
{
	free(task); task = NULL;
}

/************************************************
Check if task1 is before task2
*************************************************/
int SchedulerTaskIsBefore(const void *task1, const void *task2, void *params)
{	
	assert(task1 != NULL);
	assert(task2 != NULL);
	
	return(((const task_t*)task1)->next_invocation_time < ((const task_t*)task2)->next_invocation_time);
}

/************************************************
check if a task matches to uid
*************************************************/
int SchedulerTaskIsMatch(const void *task, const void *uid, void *params)				
{
	assert(task != NULL);

	return(UuidIsequal(((const task_t*)task)->uid, *(const uuid_t*)uid));
}

/************************************************
Update next invocation time of task
*************************************************/
void SchedulerTaskUpdate(task_t *task)				
{
	assert(task != NULL);
	
	task->next_invocation_time = time(NULL) + task->interval_sec;
}

/************************************************
Get uid of task
*************************************************/
uuid_t SchedulerTaskGetId(const task_t *task)				
{
	assert(task != NULL);
	
	return(task->uid);
}

/************************************************
Run task
Return 0 if task needs to re-occure in queueu.
Return 1 if task should be occured only once.
*************************************************/
int SchedulerTaskRun(task_t *task)
{
	assert(task != NULL);
	
	while((time(NULL)) < (task->next_invocation_time))
	{
		sleep(task->next_invocation_time - time(NULL));
	}
	
	return (task->func(task->params));
}

