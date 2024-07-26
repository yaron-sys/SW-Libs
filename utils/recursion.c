#include <stdio.h>   /* printf (tests) */
#include <string.h>  /* str funcs (tests) */
#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* asserts */

#include "slist.h"
#include "stack.h"
#include "recursion.h"


/*******************************************************************************
iterative fibonacci. Retruns fib of n. (n > 0)
*******************************************************************************/
unsigned long FibItr(unsigned int n)
{
	unsigned long prev = 0;
    unsigned long cur = 1;
    unsigned long next = prev + cur;
    
    while (n > 0)
    {
        prev = cur;
        cur = next;
        next = prev + cur;
        --n;
    }
    
    return (prev);
}

/*******************************************************************************
Recursive fibonacci. Returns fib of n. (n > 0)
*******************************************************************************/
unsigned long FibRec(unsigned int n)
{
    if (n <= 1)
    {
        return (n);
    }

	return (FibRec(n-1) + FibRec(n-2));
}

/*******************************************************************************
Recursive fibonacci + memoization. Returns fib of n. (n > 0)
*******************************************************************************/
unsigned long FibRecMem(unsigned int n)
{
	unsigned long fib_res = 0;
	unsigned long *array = NULL;
	
    if (n <= 1)
    {
        return (n);
    }
	
	array = (unsigned long *)calloc(n, sizeof(unsigned long));
	if (NULL == array)
	{
		return (0);
	}
	
	array[0] = array[1] = 1UL;	
	fib_res = FibRecMemImpel(n, array);	
	
	free(array);
	
	return (fib_res);
	
}

unsigned long FibRecMemImpel(unsigned int n, unsigned long *arr)
{
	assert(arr != NULL);
	
	if (arr[n-1] == 0)
	{
		arr[n-1] = FibRecMemImpel(n-1, arr) + FibRecMemImpel(n-2, arr);
	}	
	return (arr[n-1]);
}


/*******************************************************************************
StrLen: Determines length of a string
*******************************************************************************/
size_t StrLen(const char *str)
{
	assert(str != NULL);
	
	if ('\0' == *str)
	{
		return (0);
	}

	return (1 + StrLen(str + 1));
}

/*******************************************************************************
StrCmp: Compare two strings
*******************************************************************************/
int StrCmp(const char *s1, const char *s2)
{	
	assert(s1 != NULL);
	assert(s2 != NULL);
	
	if ((*s1 == '\0') || (*s1 != *s2))
	{
		return (*s1 - *s2);
	}
	
	return (StrCmp(s1 + 1, s2 + 1));
}

/*******************************************************************************
StrCpy: copy source string to destination
*******************************************************************************/
char *StrCpy(char *dest, const char *src)
{	
	assert(dest != NULL);
	assert(src != NULL);
	
	*dest = *src;
	
	if ('\0' == *src)
	{
		return (dest);
	}
	
	return ((StrCpy(dest + 1, src + 1)) - 1);
}

/*******************************************************************************
StrCat: concatenate two strings
*******************************************************************************/
char *StrCat(char *dest, const char *src)
{
	assert(dest != NULL);
	assert(src != NULL);
	
	/* copy src to end of destination */
	StrCpy(dest + StrLen(dest), src);
	
	return (dest);
}

/*******************************************************************************
StrnCmp: help function. Compare first n bytes of two strings
*******************************************************************************/
int StrnCmp(const char *s1, const char *s2, size_t n)
{	
	assert(s1 != NULL);
	assert(s2 != NULL);
	
	if ((*s1 == '\0') || (*s1 != *s2) || (n == 1))
	{
		return (*s1 - *s2);
	}
	
	return (StrnCmp(s1 + 1, s2 + 1, n-1));
}

/*******************************************************************************
StrStr: find a sub string
*******************************************************************************/
char *StrStr(const char *haystack, const char *needle)
{
	assert(haystack != NULL);
	assert(needle != NULL);

	if ('\0' == *haystack)
	{
	    return (NULL);
	}
	
	if ((*haystack == *needle) && (0 == StrnCmp(haystack, needle, StrLen(needle))))
	{
		return ((char*)haystack);
	}

	return (StrStr(++haystack, needle));
}

/*******************************************************************************
Recursive slist flip. Returns pointer to new head.
*******************************************************************************/
slist_node_t *SListFlipRec(slist_node_t *head)
{
	slist_node_t *rev = NULL;
	
	assert(head != NULL);
	assert(0 == SListHasLoop(head));
		
	if (NULL == (head->next))
	{
		return (head);
	}

	rev = SListFlipRec(head->next);	
	head->next->next = head;	
	head->next = NULL;

	return (rev);
}


/*******************************************************************************
Free allocated memory
*******************************************************************************/
static void FreeMemoryAlloc(void *data)
{
	free(data); data = NULL;
}

/*******************************************************************************
Sort Stack Recursively. Returns 0 in success or 1 in failure.
*******************************************************************************/
int SortStack(stack_t *stack,
			  size_t element_size,
			  int (*cmp)(const void *data1, const void *data2, void *params),
			  void *params)
{
    void *data = NULL;
    int ret = 0;
    
    assert(stack != NULL);
    assert(element_size != 0);
    assert(cmp != NULL);
    
    if (StackSize(stack) < 2)
    {
        return (0);
    }
    
    /* get data from top element */
    data = malloc(element_size);
    if (NULL == data)
    {
    	return (1);
    }
    memcpy(data, StackPeek(stack), element_size);
    
    /* remove top element */
    StackPop(stack);
    
    /* pop out elements in stack */
    ret = SortStack(stack, element_size, cmp, params);
    if (ret)
    {
    	FreeMemoryAlloc(data);
    	return (ret);
    }
    
    /* Insert elements back into sorted list */
    ret = InsertStack(stack, data, element_size, cmp, params);
    
    FreeMemoryAlloc(data);
    
    return (ret);
}

/*******************************************************************************
Insert Stack Recursively. Returns 0 in success or 1 in failure.
*******************************************************************************/
int InsertStack(stack_t *stack,
				 const void *element,
				 size_t element_size,
				 int (*cmp)(const void *data1, const void *data2, void *params),
				 void *params)
{
    void *buffer = NULL;
    int ret = 0;
    
    assert(stack != NULL);
    assert(element != NULL);
    assert(element_size != 0);
    assert(cmp != NULL);
    
    /* Stack is empty or inserted element is bigger than top */
    if ((0 == StackSize(stack)) || (cmp(element, StackPeek(stack), params) > 0))
    {
        ret = StackPush(stack, element);
        return (ret);
    }
 
    /* If top is greater, pop the top item and insert recursively */
    buffer = malloc(element_size);
    if (NULL == buffer)
    {
    	return (1);
    }
    
    /* copy top element into buffer */
    memcpy(buffer, StackPeek(stack), element_size);
  	
  	/* pop top element */
    StackPop(stack);
    
    /* insert recursively */
    ret = InsertStack(stack, element, element_size, cmp, params);
	if (ret)
	{
		FreeMemoryAlloc(buffer);
		return (ret);
	}
	
    /* Put back the top item removed earlier */
    ret = StackPush(stack, buffer);
	
    /* free memory */
    FreeMemoryAlloc(buffer);
    
    return (ret);
}



/*******************************************************************************
                                    TESTS
*******************************************************************************/
static int Cmp(const void *data1, const void *data2, void *params)
{
	return (*(int*)data1 - *(int*)data2);
}


int main(void)
{
	unsigned int n = 0;
	char dest[50] = "this is ";
	char src[] = "what i wanna add.";
	char *s;

	size_t elements = 3;
	size_t size = sizeof(int);
	int *peek;
	int num = 10;
	int num2 = 20;
	int num3 = 30;
	stack_t *my_stack = NULL;
	
	
	/* Recursive fib */
	printf("Recursive fib:\n");
	for(n = 1; n <= 20; ++n)
	{
		printf("fib %u: %lu\n", n, FibRec(n));
	}
	
	printf("\n\n");
	
	/* Iterative fib */
	printf("Iterative fib:\n");
	for(n = 1; n <= 20; ++n)
	{
		printf("fib %u: %lu\n", n, FibItr(n));
	}
	
		printf("\n\n");
	
	/* fib_rec_mem */
	printf("Recursive memoization fib:\n");
	for(n = 1; n <= 20; ++n)
	{
		printf("fib %u: %lu\n", n, FibRecMem(n));
	}
	printf("\n-----------------------------------------------------------------\n");
	
	printf("hello VS hello: %d", StrCmp("hello", "hello"));
	printf("\nhello VS hello: %d", strcmp("hello", "hello"));
	
	printf("\nheLlo VS hello: %d", StrCmp("heLlo", "hello"));
	printf("\nheLlo VS hello: %d", strcmp("heLlo", "hello"));
	
	printf("\nhelllllll VS hello: %d", StrCmp("helllllll", "hello"));
	printf("\nhelllllll VS hello: %d", strcmp("helllllll", "hello"));
	
	printf("\nhezllllll VS hello: %d", StrCmp("hezllllll", "hello"));
	printf("\nhezllllll VS hello: %d", strcmp("hezllllll", "hello"));
	
	printf("\nhe VS hello: %d", StrCmp("he", "hello"));
	printf("\nhe VS hello: %d", strcmp("he", "hello"));
	
	printf("\nhelao VS helko: %d", StrCmp("heLlo", "hello"));
	printf("\nhelao VS helko: %d", strcmp("heLlo", "hello"));
	printf("\n");
	
	printf("\n-----------------------------------------------------------------\n");
	
	printf("\nhello VS hello: %d", StrnCmp("hello", "hello", 5));
	printf("\nhello VS hello: %d", strncmp("hello", "hello", 5));
	
	printf("\nheLlo VS hello: %d", StrnCmp("heLlo", "hello", 5));
	printf("\nheLlo VS hello: %d", strncmp("heLlo", "hello", 5));
	
	printf("\nhelllllll VS hello: %d", StrnCmp("helllllll", "hello", 5));
	printf("\nhelllllll VS hello: %d", strncmp("helllllll", "hello", 5));
	
	printf("\nhzlllllll VS hello: %d", StrnCmp("hzlllllll", "hello", 5));
	printf("\nhzlllllll VS hello: %d", strncmp("hzlllllll", "hello", 5));
	
	printf("\nhe VS hello: %d", StrnCmp("he", "hello", 5));
	printf("\nhe VS hello: %d", strncmp("he", "hello", 5));
	
	printf("\nhelao VS helko: %d", StrnCmp("heLlo", "hello", 5));
	printf("\nhelao VS helko: %d", strncmp("heLlo", "hello", 5));
	printf("\n");
	
	printf("\n-----------------------------------------------------------------\n");
	
	printf("\n");
	s = StrCat(dest, src);
	printf("%s\n", s);
	
	printf("\n-----------------------------------------------------------------\n");

	s = StrStr("ababc", "b");
	if(s != NULL)
	{
		printf("%s\n", s);
	}
	else
	{
		printf("NULL\n");
	}
	
	/****************************************************************************/
	
	my_stack = StackCreate(size, elements);
	printf("\nstack size: %lu\n", StackSize(my_stack));

	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);

	printf("-----------\n");
	
    printf("Unsorted stack: 20, 30, 10:\n");
	/* push 20 */
	printf("push 20:\n");
	StackPush(my_stack, &num2);
	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);

	/* push 30 */
	printf("\npush 30:\n");
	StackPush(my_stack, &num3);
	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);
	
	/* push 10 */
	printf("\npush 10:\n");
	StackPush(my_stack, &num);
	peek = (int*)StackPeek(my_stack);
	printf("stack peek: %d\n", *peek);
	
	printf("\n\nSorted stack: 10, 20, 30:\n");
    SortStack(my_stack, sizeof(int), &Cmp, NULL);

	/* peek 30 */
	peek = (int*)StackPeek(my_stack);
	printf("stack peek 30: %d\n", *peek);
	StackPop(my_stack);
	
	/* peek 20 */
	peek = (int*)StackPeek(my_stack);
	printf("stack peek 20: %d\n", *peek);
	StackPop(my_stack);
	
	/* peek 10 */
	peek = (int*)StackPeek(my_stack);
	printf("stack peek 10: %d\n", *peek);
	StackPop(my_stack);

	
	/* free memory */
	StackDestroy(my_stack);
	
	
	return (0);
}
