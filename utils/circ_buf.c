#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memcpy */
#include <assert.h>   /* asserts */

#include "circ_buf.h"


struct circ_buf
{
	size_t capacity; /* total bytes */
	size_t read_idx; /* read index */
	size_t size;     /* amount of occupied bytes */
	char data[1];    /* cyclic buffer */
};

/* Global enum veriable (saves last error state) */
enum circ_buf_errno circ_buf_e;



/* Creates a circ buffer */
circ_buf_t *CircBufCreate(size_t capacity)
{
	circ_buf_t *buf = NULL;
	assert(capacity != 0);

	/* Creating a circ-buff instance */
	buf = (circ_buf_t *) malloc (sizeof(*buf) + capacity);
	if(!buf)
	{
		return (NULL);
	}
	
	/* initialize buffer fields */
	buf -> capacity = capacity;
	buf -> read_idx = 0;
	buf -> size = 0;

	return (buf);
}




/* Destroys a circ buffer */
void CircBufDestroy(circ_buf_t *buf)
{
	free(buf);
	buf = NULL;
}




/* Returns free space */
size_t CircBufFreeSpace(const circ_buf_t *buf)
{
	assert(buf != NULL);
	return (buf->capacity - buf->size);
}




/* Returns capacity */
size_t CircBufCapacity(const circ_buf_t *buf)
{
	assert(buf != NULL);
	return (buf->capacity);
}




/* nothing is written or everything was read */
int CircBufIsEmpty(const circ_buf_t *buf)
{
	assert(buf != NULL);
	return (0 == buf->size);
}




/***************************************************
	Read from circ buf to extern buffer
	return num of read bytes
*****************************************************/
size_t CircBufRead(circ_buf_t *buf, void *extern_buf, size_t count)
{
	size_t to_end = 0;
	
	assert(buf != NULL);
	assert(extern_buf != NULL);
	
	/* num of bytes from read_start to the end */
	to_end = buf->capacity - buf->read_idx;

	/* initiate errno state */
	circ_buf_e = CIRC_BUF_SUCCESS;
	
	if(buf->size < count)
	{
		/* limit copy to maximum occupied bytes */
		circ_buf_e = CIRC_BUF_UNDERFLOW;
		count = buf->size;
	}
	
	if(count <= to_end)
	{
		/* count is less than or eqal toEnd - copy one segment */
		memcpy(extern_buf, buf->data + buf->read_idx, count);
	}
	
	else
	{
		/* count is bigger than toEnd - copy in two segments */
		memcpy(extern_buf, buf->data + buf->read_idx, to_end);
		memcpy(((char *)extern_buf + to_end) , buf->data, count - to_end);
	}
	
	/* decrease size */
	buf->size -= count;
	
	/* update read index */
	buf->read_idx = (buf->read_idx + count) % (buf->capacity);

	return (count);
}




/***************************************************
	Write from extern buffer to circ buf
	return num of writen bytes
*****************************************************/
size_t CircBufWrite(circ_buf_t *buf, void *extern_buf, size_t count)
{
	size_t free_space = 0;
	size_t write_start_idx = 0;
	size_t to_end = 0;
	
	assert(buf != NULL);
	assert(extern_buf != NULL);
	
	/* amount of free space */
	free_space = CircBufFreeSpace(buf);

	/* index of write start */
	write_start_idx = (buf->read_idx + buf->size) % (buf->capacity);
	
	/* num of bytes from write_start to the end */
	to_end = buf->capacity - write_start_idx;

	/* initiate errno state */
	circ_buf_e = CIRC_BUF_SUCCESS;
	
	
	if(free_space < count)
	{
		/* limit copy to maximum free space bytes */
		circ_buf_e = CIRC_BUF_OVERFLOW;
		count = free_space;
	}
	
	if(count <= to_end)
	{
		/* count is less than or eqal toEnd - copy one segment */
		memcpy(buf->data + write_start_idx ,extern_buf, count);
	}
	
	else
	{
		/* count is bigger than toEnd - copy in two segments */
		memcpy(buf->data + write_start_idx ,extern_buf, to_end);
		memcpy(buf->data, ((char *)extern_buf + to_end), count - to_end);
	}
	
	/* increase size */
	buf->size += count;
	
	return (count);
}


