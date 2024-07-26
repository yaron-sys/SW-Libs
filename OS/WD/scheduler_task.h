#ifndef SCHEDULER_TASK_H_					
#define SCHEDULER_TASK_H_					
					
#include <stddef.h> /* size_t */					
					
#include "uuid.h"					
					
typedef struct task task_t;					
					
				
task_t *SchedulerTaskCreate(int(*func)(void *params), void *params, unsigned long interval_sec);	
				
void SchedulerTaskDestroy(task_t *task);					

int SchedulerTaskIsBefore(const void *task1, const void *task2, void *params);	
				
int SchedulerTaskIsMatch(const void *task, const void *uid, void *params);					

void SchedulerTaskUpdate(task_t *task);					

uuid_t SchedulerTaskGetId(const task_t *task);					

/* if task needs to re-occure in the queueu return 0, otherwise return 1 */					
int SchedulerTaskRun(task_t *task);					
					
					
#endif /* SCHEDULER_TASK_H_ */					
