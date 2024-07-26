#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MESSAGE_BUFFER 500
#define USERNAME_BUFFER 10

typedef struct {
    char* prompt;
    int socket;
} thread_data;

// Connect to server
void* connect_to_server(int socket_fd, struct sockaddr_in *address)
{
    int response = connect(socket_fd, (struct sockaddr *) address, sizeof *address);
    if (response < 0)
    {
        fprintf(stderr, "connect() failed: %s\n", strerror(errno));
        exit(1);
    } 
    else 
    {
      printf("Connected\n");
    }
}

// Get message from stdin and send to server
void* send_message(char prompt[USERNAME_BUFFER+5], int socket_fd, struct sockaddr_in *address)
{
  printf("%s", prompt+1);
  char message[MESSAGE_BUFFER];
  char final_message[MESSAGE_BUFFER+USERNAME_BUFFER+1];
  
  send(socket_fd, prompt, strlen(prompt)+1, 0);
  prompt[0] = 's'; //send data
  
  while (fgets(message, MESSAGE_BUFFER, stdin) != NULL) {
      memset(final_message,0,strlen(final_message)); // Clear final message buffer
      strcat(final_message, prompt);
      strcat(final_message, message);
      printf("%s", prompt+1);
      
      if (strncmp(message, "\n", 2) == 0){
            continue;
      }
      
      if (strncmp(message, "quit", 4) == 0){
        printf("Closing connection...\n");
        prompt[0] = 'q'; // quit chat
        send(socket_fd, prompt, strlen(prompt)+1, 0); 
        exit(0);
      }
     
      send(socket_fd, final_message, strlen(final_message)+1, 0);
      
  }
}

void* receive(void* threadData) {
    int socket_fd, response;
    char message[MESSAGE_BUFFER];
    thread_data* pData = (thread_data*)threadData;
    socket_fd = pData->socket;
    char* prompt = pData->prompt;
    memset(message, 0, MESSAGE_BUFFER); // Clear message buffer

    // Print received message
    while(true) 
    {
        /* response = recvfrom(socket_fd, message, MESSAGE_BUFFER, 0, NULL, NULL); */
        response = recv(socket_fd, message, MESSAGE_BUFFER, 0);
        if (response == -1) {
          fprintf(stderr, "recv() failed: %s\n", strerror(errno));
          break;
        } 
        else if (response == 0) 
        {
              printf("\nPeer disconnected\n");
              break;
        } 
        else
        {
              printf("\n%s", message);
              printf("%s", prompt);
              fflush(stdout); // Make sure "User>" gets printed
        }
    }
}

int main(int argc, char**argv)
{
    long port = strtol(argv[2], NULL, 10);
    struct sockaddr_in address, cl_addr;
    char * server_address;
    int socket_fd, response;
    char prompt[USERNAME_BUFFER+5];
    char username[USERNAME_BUFFER];
    pthread_t thread;

    // Check for required arguments
    if (argc < 3) {
        printf("Usage: client ip_address port_number\n");
        exit(1);
    }

    // Get user handle
    printf("Enter your user name: ");
    fgets(username, USERNAME_BUFFER, stdin);
    username[strlen(username) - 1] = 0; // Remove newline char from end of string
    prompt[0] = 'n'; // welcome new user
    strcpy(prompt+1, username);
    strcat(prompt, "> ");

    server_address = argv[1];
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server_address);
    address.sin_port = htons(port);
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    connect_to_server(socket_fd, &address);

    // Create data struct for new thread
    thread_data data;
    data.prompt = prompt+1;
    data.socket = socket_fd;

    // Create new thread to receive messages
    pthread_create(&thread, NULL, receive, (void *) &data);

    // Send message
    send_message(prompt, socket_fd, &address);

    // Close socket and kill thread
    close(socket_fd);
    pthread_exit(NULL);
    return 0;
}
