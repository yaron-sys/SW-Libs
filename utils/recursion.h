#ifndef RECURSION_H_
#define RECURSION_H_

#include <stddef.h> /* size_t */

unsigned long FibItr(unsigned int n);
unsigned long FibRec(unsigned int n);
unsigned long FibRecMem(unsigned int n);
unsigned long FibRecMemImpel(unsigned int n, unsigned long *arr);

size_t StrLen(const char *str);
int StrCmp(const char *s1, const char *s2);
char *StrCpy(char *dest, const char *src);
char *StrCat(char *dest, const char *src);
int StrnCmp(const char *s1, const char *s2, size_t n);
char *StrStr(const char *haystack, const char *needle);

slist_node_t *SListFlipRec(slist_node_t *head);

int SortStack(stack_t *stack,
			   size_t element_size,
			   int (*cmp)(const void *data1, const void *data2, void *params),
			   void *params);
			   
			   
int InsertStack(stack_t *stack,
				 const void *element,
				 size_t element_size,
				 int (*cmp)(const void *data1, const void *data2, void *params),
				 void *params);


#endif /* RECURSION_H_ */
