#ifndef CIRC_BUF_H_
#define CIRC_BUF_H_

#include <stddef.h> /* size_t */

typedef struct circ_buf circ_buf_t;

enum circ_buf_errno
{
	CIRC_BUF_SUCCESS = 0,
	CIRC_BUF_OVERFLOW = 1,
	CIRC_BUF_UNDERFLOW = 2
};

/************************************************/

circ_buf_t *CircBufCreate(size_t capacity);

void CircBufDestroy(circ_buf_t *buf);

size_t CircBufFreeSpace(const circ_buf_t *buf);

size_t CircBufCapacity(const circ_buf_t *buf);

/* return (1 = True, 0 = False) */
int CircBufIsEmpty(const circ_buf_t *buf);

/* return num of read bytes */
size_t CircBufRead(circ_buf_t *buf, void *extern_buf, size_t count);

/* return num of write bytes */
size_t CircBufWrite(circ_buf_t *buf, void *extern_buf, size_t count);



#endif /* CIRC_BUF_H_*/
