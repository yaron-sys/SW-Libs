#include <string.h>    /* memset, strcmp */
#include <stdlib.h>    /* abort */
#include <stdio.h>     /* printf */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait */
#include <unistd.h>    /* fork, exec */

#define MAX_LEN 200

void GetUserCommand(char *buffer, char **arg_list)
{
	char delim[] = {' ', '\n'};
	char *word = NULL;
	size_t i = 0;

	do
	{
		printf(">>> ");
		memset(buffer, 0, MAX_LEN+1);
		fgets(buffer, MAX_LEN, stdin);
	} while(!strcmp(buffer, "\n"));
	
	word = strtok(buffer, delim);
	while (word != NULL)
	{
		arg_list[i++] = word;
		word = strtok(NULL, delim);
	}
}

void Spawn(char *program, char **arg_list)
{
	pid_t child_pid;
	
	/* Duplicate this process. */
	child_pid = fork();
	
	if (child_pid > 0)
	{
		/* This is the parent process. wait until child is finished */
		wait(NULL);
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

int main (int argc, char *argv[])
{
	char buffer[MAX_LEN+1] = {0};
	char *arg_list[MAX_LEN] = {0};
	
	GetUserCommand(buffer, arg_list);
	
	while (strcmp(arg_list[0], "exit"))
	{
		Spawn(arg_list[0], arg_list);
		GetUserCommand(buffer, arg_list);
	}
	
	return (0);
}
