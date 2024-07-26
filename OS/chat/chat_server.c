#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MESSAGE_BUFFER 500
#define BACKLOG 10   // how many pending connections queue will hold
#define PORT "12345"

#define TRUE             1
#define FALSE            0


int main (int argc, char *argv[])
{
  int    len, rc, on = 1;
  int    listen_sd = -1, new_sd = -1;
  int    end_server = FALSE, compress_array = FALSE;
  int    close_conn;
  char   buffer[MESSAGE_BUFFER];
  struct addrinfo hints, *servinfo, *p;
  int    timeout;
  struct pollfd fds[200];
  int    nfds = 1, current_size = 0, i, j;
   
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     //use IPv4 or IPv6, whichev
    hints.ai_socktype = SOCK_STREAM; //tcp
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rc = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }
    // loop through all the results and bind to the first we can
    //the list is for if the server have more than one ip.
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((listen_sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
        {
            close(listen_sd);
            perror("setsockopt");
            exit(-1);
        }
        
        rc = ioctl(listen_sd, FIONBIO, (char *)&on);
        if (rc < 0)
        {
            perror("ioctl() failed");
            close(listen_sd);
            exit(-1);
        }
        
        if (bind(listen_sd, p->ai_addr, p->ai_addrlen) == -1) {
            close(listen_sd);
            perror("server: bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo); // all done with this structure

  /*************************************************************/
  /* Set the listen back log                                   */
  /*************************************************************/
  rc = listen(listen_sd, BACKLOG);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }

  /*************************************************************/
  /* Initialize the pollfd structure                           */
  /*************************************************************/
  memset(fds, 0 , sizeof(fds));

  /*************************************************************/
  /* Set up the initial listening socket                        */
  /*************************************************************/
  fds[0].fd = listen_sd; //assign the server fd(socket) to poll set
  fds[0].events = POLLIN;
  /*************************************************************/
  /* Initialize the timeout to 3 minutes. If no                */
  /* activity after 3 minutes this program will end.           */
  /* timeout value is based on milliseconds.                   */
  /*************************************************************/
  timeout = (3 * 60 * 1000);

  /*************************************************************/
  /* Loop waiting for incoming connects or for incoming data   */
  /* on any of the connected sockets.                          */
  /*************************************************************/
  do
  {
    /***********************************************************/
    /* Call poll() and wait 3 minutes for it to complete.      */
    /***********************************************************/
    printf("Waiting on poll()...\n");
    rc = poll(fds, nfds, timeout);

    /***********************************************************/
    /* Check to see if the poll call failed.                   */
    /***********************************************************/
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }

    /***********************************************************/
    /* Check to see if the 3 minute time out expired.          */
    /***********************************************************/
    if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
    }
    
    /***********************************************************/
    /* One or more descriptors are readable.  Need to          */
    /* determine which ones they are.                          */
    /***********************************************************/
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      /*********************************************************/
      /* Loop through to find the descriptors that returned    */
      /* POLLIN and determine whether it's the listening       */
      /* or the active connection.                             */
      /*********************************************************/
      /* if it the listening return the head of the loop  = no event on this fd*/
      if(fds[i].revents == 0) 
        continue;

      /*********************************************************/
      /* If revents is not POLLIN, it's an unexpected result,  */
      /* log and end the server.                               */
      /*********************************************************/
      if(fds[i].revents != POLLIN)
      {
        printf("  Error! revents = %d\n", fds[i].revents);
        end_server = TRUE;
        break;

      }
      if (fds[i].fd == listen_sd)
      {
        /*******************************************************/
        /* Listening descriptor is readable.                   */
        /*******************************************************/
        printf("  Listening socket is readable\n");

        /*******************************************************/
        /* Accept all incoming connections that are            */
        /* queued up on the listening socket before we         */
        /* loop back and call poll again.                      */
        /*******************************************************/
        do
        {
          
          /*****************************************************/
          /* Accept each incoming connection. If               */
          /* accept fails with EWOULDBLOCK, then we            */
          /* have accepted all of them. Any other              */
          /* failure on accept will cause us to end the        */
          /* server.                                           */
          /*****************************************************/
          new_sd = accept(listen_sd, NULL, NULL);
          if (new_sd < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  accept() failed");
              end_server = TRUE;
            }
            break;
          }

          /*****************************************************/
          /* Add the new incoming connection to the            */
          /* pollfd structure                                  */
          /*****************************************************/
          printf("  New incoming connection - %d\n", new_sd);
          fds[nfds].fd = new_sd;
          fds[nfds].events = POLLIN;
          nfds++;

          /*****************************************************/
          /* Loop back up and accept another incoming          */
          /* connection                                        */
          /*****************************************************/
        } while (new_sd != -1);
      }

      /*********************************************************/
      /* This is not the listening socket, therefore an        */
      /* existing connection must be readable                  */
      /*********************************************************/

      else
      {
        printf("  Descriptor %d is readable\n", fds[i].fd);
        close_conn = FALSE;
        /*******************************************************/
        /* Receive all incoming data on this socket            */
        /* before we loop back and call poll again.            */
        /*******************************************************/
        do
        {
          /*****************************************************/
          /* Receive data on this connection until the         */
          /* recv fails with EWOULDBLOCK. If any other         */
          /* failure occurs, we will close the                 */
          /* connection.                                       */
          /*****************************************************/
          rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = TRUE;
            }
            break;
          }

          /*****************************************************/
          /* Check to see if the connection has been           */
          /* closed by the client                              */
          /*****************************************************/
          if (rc == 0)
          {
            printf("  Connection closed\n");
            close_conn = TRUE;
            break;
          }

          /*****************************************************/
          /* Data was received                                 */
          /*****************************************************/
          len = rc;
          printf("  %d bytes received\n", len);

          /*****************************************************/
          /* Echo the data back to the client                  */
          /*****************************************************/   
            char msg[20];
            
            switch(buffer[0])
            {
                case 'n':
                    memset(msg, 0, strlen(msg));
                    strncpy(msg, buffer + 1, strlen(buffer) - 3);
                    strcpy(msg + strlen(msg), " has joined in!\n");
                    for (j = 1; j < current_size; ++j) {
                        if (j != i) {
                            rc = send(fds[j].fd, msg, strlen(msg) + 1, 0);
                        } 
                    }    
                    break;

                case 's':
                {
                    for (j = 1; j < current_size; ++j) {
                        if (j != i) {
                            rc = send(fds[j].fd, buffer+1, strlen(buffer+1)+1, 0);
                        }
                        if (rc < 0){
                            perror("  send() failed");
                            close_conn = TRUE;
                            break;
                        }
                    }
                    break;
                }
                
                case 'q':
                {
                    memset(msg, 0, strlen(msg));
                    strncpy(msg, buffer + 1, strlen(buffer) - 3);
                    strcpy(msg + strlen(msg), " has left!\n");
                    for (j = 1; j < current_size; ++j) {
                        if (j != i) {
                            rc = send(fds[j].fd, msg, strlen(msg) + 1, 0);
                        } 
                    }
                    break;
                }
                
                default:
                    printf("client send is not irc protocol\n");
                    break;
            }
          
            break;
        } while(TRUE);

        /*******************************************************/
        /* If the close_conn flag was turned on, we need       */
        /* to clean up this active connection. This            */
        /* clean up process includes removing the              */
        /* descriptor.                                         */
        /*******************************************************/
        if (close_conn)
        {
          close(fds[i].fd);
          fds[i].fd = -1;
          compress_array = TRUE;
        }


      }  /* End of existing connection is readable             */
    } /* End of loop through pollable descriptors              */

    /***********************************************************/
    /* If the compress_array flag was turned on, we need       */
    /* to squeeze together the array and decrement the number  */
    /* of file descriptors. We do not need to move back the    */
    /* events and revents fields because the events will always*/
    /* be POLLIN in this case, and revents is output.          */
    /***********************************************************/
    if (compress_array)
    {
      compress_array = FALSE;
      for (i = 0; i < nfds; i++)
      {
        if (fds[i].fd == -1)
        {
          for(j = i; j < nfds; j++)
          {
            fds[j].fd = fds[j+1].fd;
          }
          i--;
          nfds--;
        }
      }
    }

  } while (end_server == FALSE); /* End of serving running.    */

  /*************************************************************/
  /* Clean up all of the sockets that are open                 */
  /*************************************************************/
  for (i = 0; i < nfds; i++)
  {
    if(fds[i].fd >= 0)
      close(fds[i].fd);
  }
  
  return (0);
}


