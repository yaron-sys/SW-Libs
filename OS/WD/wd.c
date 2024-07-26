#define _GNU_SOURCE

#include <stdio.h>		/* for printf */ 
#include <string.h>		/* for memset */ 
#include <assert.h>		/* for assert */
#include <stdlib.h>

#include <pthread.h>	/* fot pthread_create */
#include <semaphore.h>	/* for semaphore */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait */
#include <sys/stat.h>  /* For mode constants */
#include <unistd.h>    /* fork, exec */
#include <signal.h>    /* signal handling */
#include <fcntl.h>     /* For O_* constants */

#include "scheduler.h"
#include "wd.h"

int ImAlive = 1;

typedef struct proc
{
	pid_t proc_pid[2];
}proc_t;

static void SigHandler(int signal_num)
{
	if (signal_num == SIGUSR1)
	{
		printf("---------sending is_alive signal (1)\n");
		ImAlive = 1;
	}
	
	else if (signal_num == SIGUSR2)
	{
		/* exit succesfully */
		printf("exit\n");
		return;
	}
}

static int SigTaskCheck(void *params)
{
	if (ImAlive)
	{
		printf("---------is_alive check (3), set to (0)\n");
		ImAlive = 0;
	}
	
	else
	{
		/* call ressurection */
		printf("Call resurection()\n");
	}
	
	return (0);
}


static int SigTaskAlive(void *params)
{
	/* send signal SIGUSR1 */
/*	printf("\n\tpid: %d ->", getpid());*/
	kill(getpid(), SIGUSR1);
	
	return (0);
}

static void *Thread24(void *args)
{
	sem_t *p = (sem_t *)args;
	scheduler_t *sched = NULL;
	uuid_t task;
	
	/* create scheduler */
	sched = SchedulerCreate();
	
	/* Add tasks */
	task = SchedulerAdd(sched, &SigTaskAlive, NULL, 1);
	task = SchedulerAdd(sched, &SigTaskCheck, NULL, 3);
	
	sem_wait(p);
	
	/* run scheduler and send signals */
	SchedulerRun(sched);
	
	return (NULL);
}

static void *ThreadWd(void *args)
{
	sem_t *p = (sem_t *)args;
	scheduler_t *sched = NULL;
	uuid_t task;
		
	/* create scheduler */
	sched = SchedulerCreate();
	
	/* Add tasks */
	task = SchedulerAdd(sched, &SigTaskAlive, NULL, 1);
	task = SchedulerAdd(sched, &SigTaskCheck, NULL, 3);
	
	sem_post(p);
		
	/* run scheduler and send signals */
	SchedulerRun(sched);
	
	return (NULL);
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

static int Attach(void)
{	
	pthread_t thread_wd;
	
	/* attach semaphore */
	sem_t *semaphore = sem_open("semaphore", O_CREAT, 0644, 0);
	
	/* register signal handlers */
	signal(SIGUSR1, SigHandler);
	signal(SIGUSR2, SigHandler);
		
	/* Create Thread */
	pthread_create(&thread_wd,  NULL,  &ThreadWd,  semaphore);
	
	sem_unlink("semaphore");
	
	return (0);
}

static int Create(char *program, char **arg_list)
{
	pthread_t thread_24;
	char *arg[] = {"./wd_exec", NULL};
	
	/* create semaphore */
	sem_t *semaphore = sem_open("semaphore", O_CREAT, 0644, 0);
	
	/* ForkExec watchdog */
	EnvokeProccess(program, arg);
	
	/* Create Thread (parent) */
	pthread_create(&thread_24,  NULL,  &Thread24, semaphore);
	
	return (0);
}

int Mmi(int argc, char **argv)
{	
	/* register signal handlers */
	signal(SIGUSR1, SigHandler);
	signal(SIGUSR2, SigHandler);
	
	/* get enviroment variable */
	if (getenv("Envoke") == NULL)
	{
		/* envoked from console */
		setenv("Envoke", "1", 0);
		
		Create("./wd_exec", argv);
		
		return (0);
	}
	
	Attach();
	
	return (0);
}


/* exit from proccess */
int Dnr(void)
{	
	kill(getpid(), SIGUSR2);
	
	return (0);
}
