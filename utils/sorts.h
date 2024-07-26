#ifndef SORTS_H_
#define SORTS_H_


#include <stddef.h> /* size_t */

int *SortsBinarySearch(int key, const int *arr, size_t len);

void SortsQuickSort(int *base, size_t len);

void SortsHeapSort(void  *arr, size_t len, size_t element_size, 
				   int  (*is_before)(const void *data1,const void *data2, void *params),
				   void  *params);
				   
int SortsMergeSort(int *arr, size_t len);

int SortsBubbleSort(void *arr, size_t len, size_t element_size,							
				int (*compar)(const void *data1, const void *data2));			
							
int SortsInsertionSort(void *arr, size_t len, size_t element_size,							
					int (*compar)(const void *data1, const void *data2));		
							
int SortsSelectionSort(void *arr, size_t len, size_t element_size,							
					int (*compar)(const void *data1, const void *data2));		


int SortsCountingSortInPlace(void *arr,										
							 size_t len,			
							 size_t element_size,			
							 size_t key_range,			
							 size_t (*get_key)(const void *element,  void *params),
							 void *params);	
								
int SortsCountingSort(void *dest,										
					  const void *arr,					
					  size_t len,					
					  size_t element_size,					
				 	  size_t key_range,					
					  size_t (*get_key)(const void *element,  void *params),
					  void *params);			
								
int SortsCountingSortImpl(void *dest,										
						  const void *arr,					
						  size_t len,					
						  size_t element_size,					
						  size_t *histogram,					
						  size_t key_range,					
						  size_t (*get_key)(const void *element, void *params),
						  void *params);		
						
#endif /* SORTS_H_ */
