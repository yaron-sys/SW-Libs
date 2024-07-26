#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "wd.h"

int main(int argc, char **argv)
{
	if (Mmi(argc, argv))
	{
		return (1);
	}
	
	/* call function non-stop */
	while (1)
	{
		printf(">>> wd_exec\n");
		sleep(1);
	}
	
	return (0);
}
