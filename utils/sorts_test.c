#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "sorts.h"


int compar(const void *data1, const void *data2)
{
	if (*(int *)data1 > *(int *)data2)
	{
		return(1);
	}
	else if(*(int *)data1 < *(int *)data2)
	{
		return(-1);
	}
	return(0);
}


int IsSorted(int *arr, size_t size)
{
	size_t i = 0;
	for(i = 0; i < size-1; ++i)
	{
		if(1 == compar(&arr[i], &arr[i+1]))
		{
			return(0);
		}
	}
	return(1);
}


void Print(int *arr, size_t len)
{
	int i;
	for(i = 0; i < len; ++i)
	{
		printf("%4d", arr[i]);
	}
	printf("\n");
}


/*******************************************************************************
							Bubble Sort
*******************************************************************************/
void BubbleSort_Test(int *arr, size_t size)
{
	clock_t start, end;
    double cpu_time_used;
	int i;
	
	printf("\n");
	printf("***************************************************************\n");
	printf("	Bubble Sort TEST - ARRAY OF %lu\n", size);
	printf("***************************************************************\n");
	/***********************************************************
	 						unsorted
	************************************************************/
    for (i = 0; i < size; ++i)
	{
		arr[i] = size-i;
	}
    
    start = clock();
	SortsBubbleSort(arr, size, sizeof(int), compar);
	end = clock();
	
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Unsorted - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
	
	/***********************************************************
	 						sorted
	************************************************************/
	for (i = 0; i < size; ++i)
	{
		arr[i] = i+1;
	}
	   
    start = clock();
	SortsBubbleSort(arr, size, sizeof(int), compar);
	end = clock();
		
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Sorted - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
	
	/***********************************************************
	 						random sort
	************************************************************/
	for (i = 0; i < size; ++i)
	{
		arr[i] = rand()%10000;
	}
 
    start = clock();
	SortsBubbleSort(arr, size, sizeof(int), compar);
	end = clock();
		
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Random sort - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
}



/*******************************************************************************
							Insertion Sort
*******************************************************************************/
void InsertionSort_Test(int *arr, size_t size)
{
	clock_t start, end;
    double cpu_time_used;
	int i;
	
	printf("\n");
	printf("***************************************************************\n");
	printf("	Insertion Sort TEST - ARRAY OF %lu\n", size);
	printf("***************************************************************\n");
	/***********************************************************
	 						unsorted
	************************************************************/
    for (i = 0; i < size; ++i)
	{
		arr[i] = size-i;
	}
    
    start = clock();
	SortsInsertionSort(arr, size, sizeof(int), compar);
	end = clock();
	
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Unsorted - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
	
	/***********************************************************
	 						sorted
	************************************************************/
	for (i = 0; i < size; ++i)
	{
		arr[i] = i+1;
	}
	   
    start = clock();
	SortsInsertionSort(arr, size, sizeof(int), compar);
	end = clock();
		
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Sorted - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
	
	/***********************************************************
	 						random sort
	************************************************************/
	for (i = 0; i < size; ++i)
	{
		arr[i] = rand()%10000;
	}
 
    start = clock();
	SortsInsertionSort(arr, size, sizeof(int), compar);
	end = clock();
		
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Random sort - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
}



/*******************************************************************************
						  	Selection Sort
*******************************************************************************/
void SelectionSort_Test(int *arr, size_t size)
{
	clock_t start, end;
    double cpu_time_used;
	int i;
	
	printf("\n");
	printf("***************************************************************\n");
	printf("	Selection Sort TEST - ARRAY OF %lu\n", size);
	printf("***************************************************************\n");
	/***********************************************************
	 						unsorted
	************************************************************/
    for (i = 0; i < size; ++i)
	{
		arr[i] = size-i;
	}
    
    start = clock();
	SortsSelectionSort(arr, size, sizeof(int), compar);
	end = clock();
	
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Unsorted - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
	
	/***********************************************************
	 						sorted
	************************************************************/
	for (i = 0; i < size; ++i)
	{
		arr[i] = i+1;
	}
	   
    start = clock();
	SortsSelectionSort(arr, size, sizeof(int), compar);
	end = clock();
		
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Sorted - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
	
	/***********************************************************
	 						random sort
	************************************************************/
	for (i = 0; i < size; ++i)
	{
		arr[i] = rand()%10000;
	}
 
    start = clock();
	SortsSelectionSort(arr, size, sizeof(int), compar);
	end = clock();
		
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cpu_time_used = 1000 * cpu_time_used;
	
	1 == IsSorted(arr, size) ? printf("Random sort - time of exec: %g ms\n", cpu_time_used) : printf("Error\n");
}

/*******************************************************************************/

int main(int argc, char *argv[])
{

	int arr1[10];
	int size1=sizeof(arr1)/sizeof(int);
	
	int arr2[1000];
	int size2=sizeof(arr2)/sizeof(int);
	
	int arr3[10000];
	int size3=sizeof(arr3)/sizeof(int);
	
	int arr4[40000];
	int size4=sizeof(arr4)/sizeof(int);
	
	srand(time(NULL));
	
	/***********************************************************
	 						array of 10
	************************************************************/
	
	BubbleSort_Test(arr1, size1);
	InsertionSort_Test(arr1, size1);
	SelectionSort_Test(arr1, size1);
	
	/***********************************************************
	 						array of 1000
	************************************************************/
	BubbleSort_Test(arr2, size2);
	InsertionSort_Test(arr2, size2);
	SelectionSort_Test(arr2, size2);
	
	/***********************************************************
	 						array of 10000
	************************************************************/
	BubbleSort_Test(arr3, size3);
	InsertionSort_Test(arr3, size3);
	SelectionSort_Test(arr3, size3);
	
	/***********************************************************
	 						array of 40000
	************************************************************/
	BubbleSort_Test(arr4, size4);
	InsertionSort_Test(arr4, size4);
	SelectionSort_Test(arr4, size4);
	
	
	return (0);
}


