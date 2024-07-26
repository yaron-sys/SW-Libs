#include <sys/mman.h> /* mmap */
#include <stdio.h>    /* printf */    
#include <stdlib.h>   /* calloc, free */ 
#include <assert.h>   /* assert */
#include <unistd.h>   /* close */
#include <fcntl.h>    /* open */
#include <sys/stat.h> /* fstat */
#include <time.h>     /* srand */

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

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

void Randomize(char **lines, size_t num_of_lines, char *mem_ptr)
{
	size_t i = 0;
	size_t rand_idx = 0;

	for (i = 0; i < num_of_lines; ++i)
	{
		do
		{
			rand_idx = rand() % num_of_lines;
		} while (lines[rand_idx] != 0);
	
		lines[rand_idx] = mem_ptr;
		
		while (*mem_ptr != '\n')
		{
			++mem_ptr;
		}
		
		++mem_ptr;
	}
}

void ExtractToStream(char **lines, size_t num_of_lines, FILE *fp)
{
	size_t i = 0;
	size_t j = 0;
	
    for (i = 0; i < num_of_lines; ++i)
    {
    	for (j = 0; lines[i][j] != '\n'; ++j)
    	{
    		fputc(lines[i][j], fp);
    	}    	
    	
    	fputc(lines[i][j], fp);
    }
}

int main(int argc, char *argv[])  
{
	int fd = 0;
	void *mem_ptr = NULL;
	size_t num_of_lines = 0;
	char **lines = NULL;
	const char *out = argv[2];
	FILE *fp = stdout;
	struct stat fileStat;
	
	srand (time(NULL));
	
	/* open dest file if is needed */	
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
    
    /* create an array with size of lines */
    lines = (char **) calloc(num_of_lines, sizeof(char *));
    if (lines == NULL)
    {
    	fclose(fp);
    	close(fd);
    	munmap(mem_ptr, fileStat.st_size);
    	handle_error("calloc");
    }
    
    /* random file-lines into buffer */
    Randomize(lines, num_of_lines, mem_ptr);
    
    /* extract to file or stdout */
	ExtractToStream(lines, num_of_lines, fp);
    
    /* clean up */
    fclose(fp);
	close(fd);
	munmap(mem_ptr, fileStat.st_size);
	
  	free(lines); lines = NULL;
  	
	return  (0);  
}
