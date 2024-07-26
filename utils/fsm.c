#include <assert.h>  /* assert */

#include "fsm.h"


struct fsm			
{			
	size_t num_blocks;		/* Num of free blocks */
	size_t block_size;		/* Size of each block (after padding) */
	size_t offset;			/* Holds offset in bytes to the next free block */	
};


static size_t AlignToWord(size_t size)
{
	return(((sizeof(size_t) - (size % sizeof(size_t))) % sizeof(size_t)) + size);
}

/*******************************************************************************
Returns size in bytes of memory neccessary for pool - 
according to num_blocks and block_size
*******************************************************************************/			
size_t FsmSuggestSize(size_t num_blocks, size_t block_size)
{
	assert(num_blocks != 0);
	assert(block_size != 0);
	
	block_size = AlignToWord(block_size);
	
	return (sizeof(fsm_t) + (block_size * num_blocks) + (sizeof(size_t) * num_blocks));
}
	
	
/*******************************************************************************
initialize the memory buffer
*******************************************************************************/			
fsm_t *FsmInit(void *mem, size_t num_blocks, size_t block_size)
{
	fsm_t *fsm = (fsm_t*)mem;
	size_t total_block = 0;    /* contains header & padded block */
	size_t header_offset = 0;  /* header offset to next block */
	char *header = NULL;
	size_t i = 0;
	
	assert(mem != NULL);
	assert(num_blocks != 0);
	assert(block_size != 0);
		
	fsm->num_blocks = num_blocks;
	fsm->block_size = AlignToWord(block_size);
	fsm->offset = sizeof(fsm_t);
	
	total_block = fsm->block_size + sizeof(size_t);
	header = (char*)mem + fsm->offset;
	header_offset = fsm->offset + total_block;
	
	while(i < (num_blocks - 1))
	{
		*(size_t *)header = header_offset;

		/* move to the address of the next block */
		header = (char *)mem + header_offset;
		header_offset += total_block;
		++i;
	}
	
	/* Last block */
	*(size_t*)header = 0UL;
	
	return (fsm);
}


/*******************************************************************************
allocate a single block O(1)
*******************************************************************************/					
void *FsmAlloc(fsm_t *fsm)
{
	char *header = NULL;
	void *alloc_block = NULL;
	size_t offset = 0;
	
	assert(fsm != NULL);
	
	if(0 == fsm->offset)
	{
		/* all memory is occupied */
		return (NULL);
	}
	
	/* header of free block */
	header = (char*)fsm + fsm->offset;
	offset = fsm->offset;
	
	/* swap between fsm-offeset and head values */
	fsm->offset = *(size_t*)header;
	*(size_t*)header = offset;	
	
	alloc_block = header + sizeof(size_t);
	fsm->num_blocks -= 1;
	
	return (alloc_block);
}			


/*******************************************************************************
free an allocated block  O(1)
*******************************************************************************/						
void FsmFree(void *block)			
{
	fsm_t *fsm = NULL;
	char *head = NULL;
	size_t head_offset = 0;
	
	assert(0 == (size_t)block % sizeof(size_t));
	
	if (block != NULL)
	{
		head = (char*)block - sizeof(size_t);
		fsm = (fsm_t*)((char*)head - *(size_t*)head);
		
		head_offset = *(size_t*)head;
		*(size_t*)head = fsm->offset;
		fsm->offset = head_offset;
		fsm->num_blocks += 1;
	}
}

/*******************************************************************************
returns the number of free blocks - O(1)
*******************************************************************************/					
size_t FsmCountFree(const fsm_t *fsm)		
{
	assert(fsm != NULL);

	return (fsm->num_blocks);
}


