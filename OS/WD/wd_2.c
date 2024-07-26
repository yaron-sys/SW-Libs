#define _GNU_SOURCE

#include <stdio.h>		/* for printf */ 
#include <string.h>		/* for memset */ 
#include <assert.h>		/* for assert */
#include <stdlib.h>

#include <pthread.h>	/* fot pthread_create */
#include <semaphore.h>	/* for semaphore */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait */
#include <unistd.h>    /* fork, exec */
#include <signal.h>    /* signal handling */

#include "scheduler.h"


extern int ImAlive;

typedef struct thread_params
{ 
	sem_t *semaphore;
} thread_par;  


static void SigHandler(int signal_num)
{
	if (signal_num == SIGUSR1)
	{
		ImAlive = 1;
	}
	
	else if (signal_num == SIGUSR2)
	{
		/* exit succesfully */
		return;
	}
}


static int SigTask(void *params)
{
	/* send signal SIGUSR1 */
	kill(getpid(), SIGUSR1);
	
	return (0);
}

static void Thread24(void *args)
{
	thread_par *p = (thread_par *)args;
	scheduler_t *sched = NULL;
	uuid_t task;
	
	/* create scheduler */
	sched = SchedulerCreate();
	
	/* Add tasks */
	task = SchedulerAdd(sched, &SigTask, NULL, 1);
	task = SchedulerAdd(sched, &SigTask, NULL, 5);
	
	sem_wait(&p->semaphore);
	
	/* run scheduler */
	SchedulerRun(sched);
	
	/* sending signals >>> */
}

static void ThreadWd(void *args)
{
	thread_par *p = (thread_par *)args;
	scheduler_t *sched = NULL;
	uuid_t task;
	
	/* create scheduler */
	sched = SchedulerCreate();
	
	/* Add tasks */
	task = SchedulerAdd(sched, &SigTask, NULL, 1);
	task = SchedulerAdd(sched, &SigTask, NULL, 5);
	
	sem_post(&p->semaphore);
	
	/* run scheduler */
	SchedulerRun(sched);
	
	/* sending signals >>> */
}

static void EnvokeProccess(char *program, char **arg_list)
{
	pid_t wd_pid;
	
	/* Duplicate this process. */
	wd_pid = fork();
	
	if (wd_pid > 0)
	{
		/* This is the parent process. */
		return;
	}
	else
	{
		/* execute program, searching for it in the path. */
		execvp(program, arg_list);
		
		/* execvp function returns only if an error occurs. */
		fprintf (stderr, "An error occurred in execvp\n");
		abort();
	}
}


void attach(char *program, char **arg_list)
{
	thread_par thread_args;
	pthread_t thread_wd;
	
	/* attach semaphore */
	
	/* Create Thread */
	pthread_create(&thread_wd,  NULL,  &ThreadWd,  &thread_args);
}

void create(char *program, char **arg_list)
{
	thread_par thread_args;
	pthread_t thread_24;
	
	/* create semaphore */
	if (sem_init(&thread_args.semaphore, 0, 1))
	{	
		return (1);
	}
	
	/* ForkExec watchdog */
	EnvokeProccess("wd", arg_list);
	
	/* Create Thread */
	pthread_create(&thread_24,  NULL,  &Thread24,  &thread_args);
}

int Mmi(int argc, char **argv, char **envp, void(*ptr)(char *prog, char **arg))
{	
	/* register signal handlers */
	signal(SIGUSR1, SigHandler);
	signal(SIGUSR2, SigHandler);
	
	/* get enviroment variable */
	if (getenv("Envoke") == NULL)
	{
		/* envoked from console */
		setenv("Envoke", "1", 0);
		create();
		return (0);
	}
	
	attach();
	
	return (0);
}


/* normal exit from proccess */
int Dnr(void)
{	
	kill(getpid(), SIGUSR2);
	
	return (0);
}
