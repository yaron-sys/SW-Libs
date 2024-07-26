#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "sorts.h"
#include "heapify.h"


static void Swap(int *val1, int *val2)
{
	int tmp = *val1;
	*val1 = *val2;
	*val2 = tmp;
}

/*******************************************************************************
Quick Sort - complexity of O(nlog(n))
Pivot will be chosen as last element
*******************************************************************************/
void SortsQuickSort(int *base, size_t len)
{
	size_t wall = 0;
	size_t cur = 0;
	
	assert(base != NULL);
	
	if (len > 1)
	{
		for (cur = 0; cur < len - 1 ; ++cur)
		{
			if (base[cur] <= base[len - 1])
			{
				Swap(base + wall, base + cur);
				++wall;			
			}
		}
		Swap(base + wall, base + len - 1);

		SortsQuickSort(base, wall);
		SortsQuickSort(base + wall + 1 , len - wall - 1);
	}
}

/*******************************************************************************
Binary Search - complexity of O(log(n))
*******************************************************************************/
int *SortsBinarySearch(int key, const int *arr, size_t len)
{
	const int *mid = NULL;
	
	assert(arr != NULL);
	
	if (0 == len)
	{
		return (NULL);
	}
	
	mid = arr + (len / 2);
	if (key == *mid)
	{
		return ((int*)mid);
	}
	
	if (key < *mid)
	{
		return (SortsBinarySearch(key, arr, len / 2));
	}
	
	return (SortsBinarySearch(key, arr + (len / 2) + 1, len - (len / 2) - 1));
}


static void SwapHeap(void *val1, void *val2, size_t size)
{
	char *ch_val1 = (char*)val1;
	char *ch_val2 = (char*)val2;
	char tmp = 0;
	size_t i = 0;
	
	for (i = 0; i < size; ++i)
	{
		tmp = *(ch_val1 + i);
		*(ch_val1 + i) = *(ch_val2 + i);
		*(ch_val2 + i) = tmp;
	}
}

/*******************************************************************************
Heap Sort - complexity of O(nlog(n))
*******************************************************************************/
void SortsHeapSort(void  *arr, size_t len, size_t element_size, 
				   int  (*is_before)(const void *data1,const void *data2, void *params),
				   void  *params)
{
	size_t idx = len / 2;

	assert(arr != NULL);
	assert(is_before != NULL);
	assert(element_size != 0);
	
	/* Build max heap 
	   start index is set to first non-leaf node */
	while (idx > 0)
	{
		--idx;
		HeapifyDown(arr, element_size, len, idx, is_before, params); 
	}
	
	/* swap root node (max) with last node and short heap by 1 
	   until heap is empty */
	while (len > 0)
	{
		--len;
		SwapHeap(arr, (char*)arr + (len * element_size), element_size);
		
		/* Heapify Down */
		HeapifyDown(arr, element_size, len, 0, is_before, params);
	}
}

static void MergeImpl(int *arr, int *left, size_t len_left, int *right, size_t len_right)
{
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	
	while ((i < len_left) && (j < len_right))
	{
		if (left[i] <= right[j])
		{
			arr[k++] = left[i++];
		}
		else
		{
			arr[k++] = right[j++];
		}
	}
	
	while (i < len_left)
	{
		arr[k++] = left[i++];
	}
	
	while (j < len_right)
	{
		arr[k++] = right[j++];
	}
}

static void SortsMergeSortImpl(int *arr, int *dest, size_t len)
{
	size_t half = 0UL;
	
	if (len <= 1)
	{
		return;
	}
	
	half = len / 2;
		
	SortsMergeSortImpl(arr, dest, half);
	SortsMergeSortImpl(arr + half, dest, len-half);
	
	/* copy content from src to sub arrays */	
	memcpy(dest, arr, half * sizeof(int));
	memcpy(dest + half, arr + half, (len-half) * sizeof(int));
	
	MergeImpl(arr, dest, half, dest + half, len - half);
}


/*******************************************************************************
Merge Sort: Complexity of O(nlog(n)).
Recursive implementation
*******************************************************************************/
int SortsMergeSort(int *arr, size_t len)
{
	int *dest = NULL;
	
	assert(arr != NULL);
	
	/* creating temp array */
	dest = (int *)malloc(len*sizeof(int));
	if (NULL == dest)
	{
		return (1);
	}
	
	SortsMergeSortImpl(arr, dest, len);
	
	free(dest); dest = NULL;
	
	return (0);
}


static void SwapElements(void *val1, void *val2, size_t element_size, void *buffer)
{
	memcpy(buffer, val1, element_size);
	memcpy(val1, val2, element_size);
	memcpy(val2, buffer, element_size);
}


/*******************************************************************************
Bubble sort: Complexity of O(n^2).
worst case O(n^2) if array is reverse sorted.
Best case O(n) if is already sorted.

stability: stable.
*******************************************************************************/
int SortsBubbleSort(void *arr, size_t len, size_t element_size,							
					 int (*compar)(const void *data1, const void *data2))
{
	char *iter = NULL;
	char *last_element = (char*)arr + (element_size * (len-1));
	int   not_sorted = 1;
	void *buffer = NULL;

	assert(arr != NULL);
	assert(len != 0);
	assert(element_size != 0);	
	assert(compar != NULL);
	
	buffer = malloc(element_size);
	if (NULL == buffer)
	{
		return (1);
	}
	while ((arr != last_element) && (1 == not_sorted))
	{
		not_sorted = 0;
		
		/* compare elements and bubble forward the higher priority element */
		for (iter = (char*)arr; iter != last_element; iter += element_size)
		{
			if (0 < compar(iter, (iter+element_size)))
			{
				SwapElements(iter, iter + element_size, element_size, buffer);
				not_sorted = 1;
			}

		}		
		last_element -= element_size;	
	}
		
	free(buffer); buffer = NULL;
	return(0);
}
			
/*******************************************************************************
Insertion Sort: Complexity of O(n^2).
worst case O(n^2) if array is reverse sorted.
Best case O(n) if is already sorted.

stability: stable.
*******************************************************************************/				
int SortsInsertionSort(void *arr, size_t len, size_t element_size,							
						int (*compar)(const void *data1, const void *data2))
{

	char *iter = (char*)arr + element_size; /* second element */
	char *end = (char*)arr + (element_size * len);
	char *insert = NULL;
	void *buffer = NULL;

	assert(arr != NULL);
	assert(len != 0);
	assert(element_size != 0);	
	assert(compar != NULL);
	
	buffer = malloc(element_size);
	if (NULL == buffer)
	{
		return (1);
	}
	while (iter != end)
	{
		insert = iter;
		
		/* iterate and swap each element to its correct position */
		while ((insert != (char*)arr) && (0 < compar(insert - element_size, insert)))
		{
			SwapElements(insert - element_size, insert, element_size, buffer);
			insert -= element_size;
		}	
		iter += element_size;
	}
	
	free(buffer); buffer = NULL;
	return(0);
}

/*******************************************************************************
Selection Sort: Complexity of O(n^2).
stability: not stable.
*******************************************************************************/					
int SortsSelectionSort(void *arr, size_t len, size_t element_size,							
						int (*compar)(const void *data1, const void *data2))
{
	char *last_element = (char*)arr + (element_size * (len-1));
	char *outer_iter = (char*)arr;
	char *inner_iter = (char*)arr;
	int is_min_found = 0;
	void *min_val = NULL;
	void *buffer = NULL;

	assert(arr != NULL);
	assert(len != 0);
	assert(element_size != 0);	
	assert(compar != NULL);	
	
	buffer = malloc(element_size);
	if (NULL == buffer)
	{
		return (1);
	}
	while (outer_iter != last_element)
	{
		/* set element to be current min value */
		min_val = outer_iter;
		
		/* scan array to find the absolute min value, if exist */
		for ((inner_iter = outer_iter+element_size); (inner_iter != last_element + element_size); 
			(inner_iter += element_size))
		{
			if (0 < compar(min_val, inner_iter))
			{
				/* replace previous min with new min */
				min_val = inner_iter;
				is_min_found = 1;
			}
		}
		if (is_min_found)
		{
			/* swap min value to its position */
			SwapElements(min_val, outer_iter, element_size, buffer);
			is_min_found = 0;
		}	
		outer_iter += element_size;
	}
	
	free(buffer); buffer = NULL;
	return(0);
}


/* create dest and call SortsCountingSort */
int SortsCountingSortInPlace(void *arr,										
							 size_t len,			
							 size_t element_size,			
							 size_t key_range,			
							 size_t (*get_key)(const void *element,  void *params),
							 void *params)		
{
	void *dest = NULL;
	int ret = 0;
	
	assert(arr != NULL);
	assert(get_key != NULL);
	assert(element_size != 0);
	
	dest = malloc(len * element_size);
	if (NULL == dest)
	{
		return (1);
	}
	
	ret = SortsCountingSort(dest, arr, len, element_size, key_range+1, get_key, params);
	if (ret != 1)
	{
		memcpy(arr, dest, len * element_size);
	}
	
	free(dest); dest = NULL;
	
	return (ret);
}
	
/* create histogram and call SortsCountingSortImpl */										
int SortsCountingSort(void *dest,										
					  const void *arr,					
					  size_t len,					
					  size_t element_size,					
				 	  size_t key_range,					
					  size_t (*get_key)(const void *element,  void *params),
					  void *params)					
{
	size_t *histogram = NULL;
	int ret = 0;
	
	assert(arr != NULL);
	assert(get_key != NULL);
	assert(dest != NULL);
	
	histogram = calloc(key_range, sizeof(size_t)); /* range is +1 */
	if (NULL == histogram)
	{
		return (1);
	}
	
	ret = SortsCountingSortImpl(dest, arr, len, element_size, histogram, key_range, get_key, params);
	
	free(histogram); histogram = NULL;
	
	return (ret);
}
			
										
int SortsCountingSortImpl(void *dest,										
						  const void *arr,					
						  size_t len,					
						  size_t element_size,					
						  size_t *histogram,					
						  size_t key_range,					
						  size_t (*get_key)(const void *element, void *params),
						  void *params)					
{
	const char *iter = (const char *)arr;
	const char *end = (const char*)arr + (len * element_size);
	size_t key = 0;
	size_t val = 0;
	size_t i = 1;
	
	assert(arr != NULL);
	assert(get_key != NULL);
	assert(dest != NULL);
	
	/* fill in histogram num of counts */
	while(iter != end)
	{
		key = get_key(iter, params);
		++histogram[key];
		iter += element_size;
	}
	
	/* sum counts in histogram - method 2 */
	while(i < key_range)
	{
		histogram[i] += histogram[i-1];
		++i;
	}
	
	for(iter = end-element_size; len > 0; iter-=element_size, --len)
	{
		key = get_key(iter, params);
		histogram[key] -= 1;
		val = histogram[key];
		memcpy(((char*)dest + (element_size*val)), &key, element_size);
	}
	
	return (0);
}
