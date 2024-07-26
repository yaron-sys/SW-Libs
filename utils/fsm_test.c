#include <stdio.h>
#include <stdlib.h>

#include "fsm.h"


size_t AlignToWord2(size_t size)
{
	return(((sizeof(size_t) - (size % sizeof(size_t))) % sizeof(size_t)) + size);
}


int main(int argc, char *argv[])
{
	size_t num_blocks = 5;
	size_t block_size = 4;
	size_t suggest = 0;
	void *mem = NULL;
	fsm_t *fsm = NULL;
	void *alloc = 0;
	void *alloc2 = 0;
	void *alloc3 = 0;
	void *alloc4 = 0;
	void *alloc5 = 0;
	
	suggest = FsmSuggestSize(num_blocks, block_size);
	printf("%lu\n", suggest);
	
	mem = malloc(suggest); /* allocate 104 bytes */
	
	fsm = FsmInit(mem, num_blocks, block_size);
	printf("count free in init: %lu\n", FsmCountFree(fsm));
	
	alloc = FsmAlloc(fsm);
	printf("count free after alloc1: %lu\n", FsmCountFree(fsm));
	
	alloc2 = FsmAlloc(fsm);
	printf("count free after alloc2: %lu\n", FsmCountFree(fsm));
	
	alloc3 = FsmAlloc(fsm);
	printf("count free after alloc3: %lu\n", FsmCountFree(fsm));
	
	alloc4 = FsmAlloc(fsm);
	printf("count free after alloc4: %lu\n", FsmCountFree(fsm));
	
	alloc5 = FsmAlloc(fsm);
	printf("count free after alloc5: %lu\n", FsmCountFree(fsm));
	
	FsmFree(alloc);
	printf("count free after free: %lu\n", FsmCountFree(fsm));
	
	FsmFree(alloc2);
	printf("count free after free: %lu\n", FsmCountFree(fsm));
	
	FsmFree(alloc3);
	printf("count free after free: %lu\n", FsmCountFree(fsm));
	
	FsmFree(alloc4);
	printf("count free after free: %lu\n", FsmCountFree(fsm));
	
	FsmFree(alloc5);
	printf("count free after free: %lu\n", FsmCountFree(fsm));
	
	return (0);
}

