#ifndef HEAPIFY_H_
#define HEAPIFY_H_

#include <stddef.h> /* size_t */

void HeapifyUp(void *arr, size_t element_size, size_t index,
			   int  (*is_before)(const void *data1, const void *data2, void *params),
			   void *params);

void HeapifyDown(void  *arr, size_t element_size, size_t len, size_t index,
				 int  (*is_before)(const void *data1, const void *data2, void *params),
				 void *params);

#endif /* HEAPIFY_H_ */
