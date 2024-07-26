#ifndef HEAP_H_		
#define HEAP_H_		
		
#include <stddef.h> /* size_t */		
		
typedef struct heap heap_t;		
		
		
/* the element before is higher in the heap */		
heap_t *HeapCreate(size_t element_size,
				   int (*is_before)(const void *data1, const void *data2, void *params),
				   void *params);	

void HeapDestroy(heap_t *heap);		

size_t HeapSize(const heap_t *heap);		

int HeapIsEmpty(const heap_t *heap);		

int HeapPush(heap_t *heap, const void *data);		

void HeapPop(heap_t *heap);		

void *HeapPeek(const heap_t *heap);		
		
/* removd is an optional parameter */		
int HeapRemove(heap_t *heap, const void *data, int (*is_match)(const void *data,		
	 		   const void *to_find, void *params), void *params, void *removed);		
		
#endif /* HEAP_H_ */
