/* shm_server.c*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define SHMSZ     27

int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
	pid_t child;
    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;

	
	child = fork(); /*create a new process*/
 
   if (child < 0)
   {
       printf ("Error Create New Process\n");
       exit(1);
   }
   else if (child == 0)
   {
       printf ("Process Created With ID %d\n", getpid());
       
       if (execl("c.o", "c.o", NULL) < 0)
             printf ("Error replace image process\n");
   }
    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
     char ping[] = "ping";
    for (int i = 0; i < 1000; ++i)
	{
		while (shm[1] != 'o')
		    sleep(1);
		s = shm;
		puts("\x1B[34m");    
		puts(shm);
		int i = 0;
		for (i = 0; i < 4; i++)
        	*s++ = ping[i];
    	*s = '\0';
    	
    }
			    
    shmctl(shmid, IPC_RMID, NULL);

    exit(0);
}

