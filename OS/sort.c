#include <pthread.h>  /* threads */
#include <sys/mman.h> /* mmap */  
#include <stdio.h>    /* printf */    
#include <stdlib.h>   /* calloc, free, qsort */ 
#include <assert.h>   /* assert */
#include <unistd.h>   /* close */
#include <fcntl.h>    /* open */
#include <sys/stat.h> /* fstat */
#include <string.h>   /* memcpy */

#define THREAD_MAX (4)
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
               
typedef struct thread
{
	pthread_t thread_id;
	char **lines_seg;
	size_t segment;
}thread_t;

int CmpWrap(const void *p1, const void *p2);
int Cmp(const void *s1, const void *s2);
void ExtractToStream(char **array, size_t len, FILE *fp);
size_t CountLines(char *mem_ptr, size_t len);
void CopyToBuff(char **lines, size_t len, char *mem_ptr);
void *thread_qsort(void *arg);
thread_t *CreateThreads(char **lines, size_t len);
void MergeImpl(char **dest, char **left, size_t len_left, char **right, size_t len_right);
char **Merge(thread_t *thread_arr, char **lines, size_t num_of_lines);

int main(int argc, char *argv[])  
{
	int i = 0;
	int fd = 0;
	size_t num_of_lines = 0;
	void *mem_ptr = NULL;
	char **lines = NULL;
	char **dest = NULL;
	const char *out = argv[2];
	thread_t *thread_arr = NULL;
	FILE *fp = stdout;
	struct stat fileStat;
	
	assert(THREAD_MAX > 0);
	
	/* open dest file if requested */
	if (out != NULL)
	{
		fp = fopen (out, "w+");
		if (fp == NULL)
		{
			return (1);
		}
	}
	    
	/* open file for mapping */
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		fclose(fp);
		handle_error("open");
	}
	
	/* file status */
    if (fstat(fd, &fileStat) < 0)    
    {
    	fclose(fp);
    	close(fd);
    	handle_error("fstat");
    }
    
	/* map file to memory */
	mem_ptr = mmap(NULL, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mem_ptr == MAP_FAILED)
	{
		fclose(fp);
    	close(fd);
		handle_error("mmap");
	}
	
    /* count number of lines */
    num_of_lines = CountLines(mem_ptr, fileStat.st_size);
    
    /* create a buffer of lines */
	lines = (char **)calloc(num_of_lines, sizeof(char *));
    if (lines == NULL)
    {
    	fclose(fp);
    	close(fd);
    	munmap(mem_ptr, fileStat.st_size);
    	handle_error("calloc");
    }

    /* insert line pointers from memory into the buffer */
    CopyToBuff(lines, num_of_lines, mem_ptr);
    
    /* create threads (THREAD_MAX) */
    thread_arr = CreateThreads(lines, num_of_lines);

	/* joining all threads */
    for (i = 0; i < THREAD_MAX; ++i)
    {
    	pthread_join(thread_arr[i].thread_id, NULL);
    }
    
	dest = Merge(thread_arr, lines, num_of_lines);

	/* extract to file or stdout */
	ExtractToStream(dest, num_of_lines, fp);
    
    /* clean up */
    fclose(fp);
	close(fd);
	munmap(mem_ptr, fileStat.st_size);

	free(lines); free(thread_arr); free(dest);
  	
	return  (0);  
}

thread_t *CreateThreads(char **lines, size_t len)
{ 
	thread_t *thread_arr = NULL;
	size_t seg_size = len / THREAD_MAX;
	size_t occupied = 0;
	size_t i = 0;

	/* create array of thread structs */
    thread_arr = (thread_t *)calloc(THREAD_MAX, sizeof(thread_t)); 
    if (thread_arr == NULL)
    {
		handle_error("calloc");
    }
	
	/* initialize each thread struct */
    for (i = 0; i < THREAD_MAX - 1; ++i)
    {
    	thread_arr[i].lines_seg = lines + occupied;
    	thread_arr[i].segment = seg_size;
    	occupied += seg_size;
    	
    	/* Create threads */
        if (pthread_create(&(thread_arr[i].thread_id), NULL, &thread_qsort, &thread_arr[i]))
        { 
			handle_error("pthread_create");
        }
    }
    /* Create last thread */
    thread_arr[i].lines_seg = lines + occupied;
    thread_arr[i].segment = len - occupied;
    if (pthread_create (&(thread_arr[i].thread_id), NULL, &thread_qsort, &thread_arr[i]))
    {
		handle_error("pthread_create");
    }
    
    return (thread_arr);
}

void *thread_qsort(void *arg)
{
    thread_t *p = (thread_t *)arg;

	qsort(p->lines_seg, p->segment, sizeof(char *), CmpWrap);

    return (NULL);
}

char **Merge(thread_t *thread_arr, char **lines, size_t num_of_lines)
{	
	size_t segment = thread_arr[0].segment;
	size_t i = 0;
	size_t accumulate = 0;
	
	/* Create destination buffer that stores merged segments of data */
    char **dest = (char **)calloc(num_of_lines, sizeof(char *));
    if (dest == NULL)
    {
    	handle_error("calloc");
    }
    
    /* merge array */
	for (i = 1; i < THREAD_MAX - 1; ++i)
	{
		accumulate = i * segment;
		MergeImpl(dest, lines, accumulate, lines + accumulate, segment);			  
		memcpy(lines, dest, (accumulate + segment) * sizeof(char *));
	}
    
	/* merge last element */
	accumulate = i * segment;
	MergeImpl(dest, lines, accumulate, lines + accumulate, num_of_lines - accumulate);
	
	return (dest);
}

void MergeImpl(char **dest, char **left, size_t len_left, char **right, size_t len_right)
{
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	
	while ((i < len_left) && (j < len_right))
	{
		if (Cmp(left[i], right[j]) <= 0)
		{
			dest[k++] = left[i++];
		}
		else
		{
			dest[k++] = right[j++];
		}
	}
	
	if (i < len_left)
	{
		memcpy(dest + k, left + i, sizeof(char *) * (len_left - i));
	}
	
	else
	{
		memcpy(dest + k, right + j, sizeof(char *) * (len_right - j));
	}
}

/*******************************************************************************
returns 0 if equall
returns positive val if s1 is greater than s2 
returns negative val if s1 is lesser than s2 
*******************************************************************************/
int Cmp(const void *s1, const void *s2)
{	
	const char *itr1 = (const char *)s1;
	const char *itr2 = (const char *)s2;
	
	while ((*itr1 == *itr2) && (*itr1 != '\n'))  
	{
		++itr1; ++itr2;
	}
	
	return (*itr1 - *itr2);
}

int CmpWrap(const void *p1, const void *p2)
{
   return (Cmp(*(char * const *)p1, *(char * const *)p2));
}

void ExtractToStream(char **array, size_t len, FILE *fp)
{
	size_t i = 0;
	size_t j = 0;
	
    for (i = 0; i < len; ++i)
    {
    	for (j = 0; array[i][j] != '\n'; ++j)
    	{
    		fputc(array[i][j], fp);
    	}    	
    	
    	fputc(array[i][j], fp);
    }
}

size_t CountLines(char *mem_ptr, size_t len)
{
	size_t i = 0;
	size_t ctr = 0;
	
	for (i = 0; i < len; ++i)
	{
		if (mem_ptr[i] == '\n')
		{
			++ctr;
		}
	}
	
	return (ctr);
}

void CopyToBuff(char **lines, size_t len, char *mem_ptr)
{
	size_t i = 0;

	for (i = 0; i < len; ++i)
	{
		lines[i] = mem_ptr;
		
		while (*mem_ptr != '\n')
		{
			++mem_ptr;
		}		
		++mem_ptr;
	}
}
