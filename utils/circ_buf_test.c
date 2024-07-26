#include <stdio.h>
#include "circ_buf.h"

extern enum circ_buf_errno circ_buf_e;


int main(int argc, char *argv[])
{
	
	circ_buf_t *mybuf = NULL;
	char extern_buf[] = "one two";
	size_t bytes;
	
	/* Create a cyrc buff */
	printf("Creating cyrc buff.\n");
	mybuf = CircBufCreate(10);
	
	/* Capacity */
	printf("Capacity: %lu\n", CircBufCapacity(mybuf));
	
	/* buff free space */
	printf("Free Space: %lu\n", CircBufFreeSpace(mybuf));
	
	/* Is empty? */
	printf("Is empty? %d\n", CircBufIsEmpty(mybuf));
	
	/* write from extern buff to circ buffer */
	printf("\nWriting:");
	
	bytes = 4;
	CircBufWrite(mybuf, extern_buf, bytes);
	printf("\nwrite %lu bytes.", bytes);
	printf("\nErrno status: %d.", circ_buf_e);
	printf("\nFree Space after write: %lu\n", CircBufFreeSpace(mybuf));
	
	bytes = 6;
	CircBufWrite(mybuf, extern_buf, bytes);
	printf("\nwrite %lu bytes.", bytes);
	printf("\nErrno status: %d.", circ_buf_e);
	printf("\nFree Space after write: %lu\n", CircBufFreeSpace(mybuf));
	
	bytes = 2;
	CircBufWrite(mybuf, extern_buf, bytes);
	printf("\nwrite %lu bytes.", bytes);
	printf("\nErrno status: %d.", circ_buf_e);
	printf("\nFree Space after write: %lu\n", CircBufFreeSpace(mybuf));
	
	
	
	/* Destroy */
	CircBufDestroy(mybuf);
	

	return (0);
}

