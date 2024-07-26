/* #define NDEBUG */

#include <stddef.h> /* size_t */
#include <assert.h>

#include "vsm.h"

#define SIZE_WORD sizeof(long)
#define ABS(N) ((N < 0L) ? (-N) : (N))

#ifndef NDEBUG
#define DEBUG_ONLY(X) X
#else
#define DEBUG_ONLY(X)
#endif


DEBUG_ONLY(static const long magic_num = 12345;)

typedef struct header
{
	long size;          /* free or occupied space */
	DEBUG_ONLY(long magic_number);	/* special id for header */
}header_t;

struct vsm
{		
	long vsm_size;  /* adjusted size of memory */
};


/* Align to word size with padding */
static size_t AlignToWord(size_t size)
{
	return (((SIZE_WORD - (size % SIZE_WORD)) % SIZE_WORD) + size);
}



/******************************
 initialize the memory buffer
 returns pointer to vsm struct
*******************************/	
vsm_t *VsmInit(void *mem, long size)
{
	header_t *header = NULL;
	
	vsm_t *vsm = (vsm_t *)mem;
	vsm->vsm_size = size - (size % SIZE_WORD);
	
	assert(mem != NULL);
	assert(vsm->vsm_size > sizeof(vsm_t) + sizeof(header_t));
	
	header = (header_t *)((char *)mem + sizeof(vsm_t));
	header->size = vsm->vsm_size - sizeof(vsm_t);
	
	DEBUG_ONLY(header->magic_number = magic_num;)

	
	return (vsm);
}


/******************************	
 allocate a single block - O(n)
*******************************/
void *VsmAlloc(vsm_t *vsm, long size)
{
	long block_size = 0;
	long total_size = 0;
	long header_offset = 0;
	long size_to_alloc = 0;
	int alloc_flag = 0;
	header_t *cur_header = NULL;
	header_t *ret_alloc = NULL;

	assert(vsm != NULL);
	assert(size > 0);
	
	/* define block size */
	block_size = AlignToWord(size) + sizeof(header_t);
	
	/* first header in vsm */
	cur_header = (header_t *)((char *)vsm + sizeof(vsm_t));
	
	/* total size available in mem */
	total_size = vsm->vsm_size - sizeof(vsm_t);
	
	while ((block_size < total_size + size_to_alloc) && (0 == alloc_flag))
	{
		if (cur_header->size < 0)
		{
			/* occupied block */
			ret_alloc = NULL;
			size_to_alloc = 0;
		}
		else
		{
			/* sum accumulated space for allocating */
			size_to_alloc += cur_header->size;
			
			if (size_to_alloc == cur_header->size)
			{
				/* point to first free space */
				ret_alloc = cur_header;
			}	
			if (size_to_alloc >= block_size)
			{	
				/* found enough spcae for allocating */
				alloc_flag = 1;
			}
		}
		
		/* move to next header */
		header_offset = ABS(cur_header->size);
		total_size = total_size - header_offset;
		cur_header = (header_t *)((char *)cur_header + header_offset);
	}
	
	/* Found enough space for allocating */
	if (alloc_flag)
	{
		if ((size_to_alloc) >= (sizeof(header_t) + block_size))
		{
			/* Enough space for allocating and set additional header */
			cur_header = (header_t *)((char *)ret_alloc + block_size);
			cur_header->size = size_to_alloc - block_size;
			DEBUG_ONLY(cur_header->magic_number = magic_num;)
			ret_alloc->size = -block_size;
		}
		else
		{
			/* no space for second header. occupies all available space */
			ret_alloc->size = -size_to_alloc;
		}
		
		return ((void *)((char *)ret_alloc + sizeof(header_t)));
	}
	
	return (NULL);	
}

/******************************	
 free an allocated block - O(1)
*******************************/	
void VsmFree(void *block)
{
	assert (0 == (size_t)block % SIZE_WORD);
	
	if (block != NULL)
	{
		header_t *header = (header_t *)((char *)block - sizeof(header_t));
		
		assert(header->size < 0);
		assert(magic_num == header->magic_number);

		header->size = ABS(header->size);
	}
}




/*************************************************************
 clear the allocated memory -> init function already does it
**************************************************************/	
/*
void VsmClear(vsm_t *vsm)
{
	assert(vsm != NULL);
	
	VsmInit(vsm, vsm->vsm_size);
}
*/
