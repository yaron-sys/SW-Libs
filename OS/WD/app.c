#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "wd.h"

/******************************** test file ************************************/
int main(int argc, char **argv)
{
	if (Mmi(argc, argv))
	{
		return (1);
	}
	
	/* call function non-stop */
	while (1)
	{
		printf(">>> 27/7\n");
		sleep(1);
	}
	
	/* safe exit - SIGUSR2 handle signal */
/*	Dnr();*/
	
	return (0);
}

