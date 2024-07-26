#include <stdio.h>
#include <time.h>

#include "uuid.h"



int main(int argc, char *argv[])
{


	uuid_t ps1 = UuidCreate();
	uuid_t ps2 = UuidCreate();
	uuid_t invalid = UuidCreate();
	
	
	printf("process1 ID: %d\n" , ps1.pid);
	printf("process1 TIME: %lu.%06lu\n" , ps1.time.tv_sec, ps1.time.tv_usec);
	printf("process1 CTR: %lu\n\n" , ps1.ctr);
	
	printf("process2 ID: %d\n" , ps2.pid);
	printf("process2 TIME: %lu.%06lu\n" , ps2.time.tv_sec, ps2.time.tv_usec);
	printf("process2 CTR: %lu\n\n" , ps2.ctr);
	
	1 == UuidIsequal(ps1,ps1) ? printf("1. good\n") : printf("1. bad\n");

	0 == UuidIsequal(ps1,ps2) ? printf("2. good\n") : printf("2. bad\n");
	
	printf("\ninvalid before:\n");
	printf("process ID: %d\n" , invalid.pid);
	printf("process TIME: %lu.%06lu\n" , invalid.time.tv_sec, invalid.time.tv_usec);
	printf("process CTR: %lu\n\n" , invalid.ctr);
	
	invalid = UuidGetInvalidID();
	
	printf("\ninvalid after:\n");
	printf("process ID: %d\n" , invalid.pid);
	printf("process TIME: %lu.%06lu\n" , invalid.time.tv_sec, invalid.time.tv_usec);
	printf("process CTR: %lu\n\n" , invalid.ctr);
	
	return (0);
}

