/**
 * Final project of CS5007
 * @author Rongyi Chen
 * @date 2019/04/22
 * @reference I used the following resources to implement this code: 
 * http://suo.im/4tMWRu
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "DocIdMap.h"
#include "htll/Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "FileCrawler.h"

#define BUFFER_SIZE 1000

int Cleanup();

DocIdMap docs;
Index docIndex;

#define SEARCH_RESULT_LENGTH 1500

char movieSearchResult[SEARCH_RESULT_LENGTH];

void sigchld_handler(int s) {
  write(0, "Handling zombies...\n", 20);
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


void sigint_handler(int sig) {
  write(0, "Ahhh! SIGINT!\n", 14);
  Cleanup();
  exit(0);
}


int HandleConnections(int sock_fd) {
  // Step 5: Accept connection
  int opt = 1;   
  int addrlen, new_socket, activity, i, valread, sd;   
  int max_sd;
  struct sockaddr_in address;
  char buffer[1025];
  fd_set readfds; 
  char *message = "test message \r\n"; 
  // max number of clients
  int max_clients = 30;
  int client_socket[30];
  //initialise all client_socket[] to 0; 
  for (i = 0; i < max_clients; i++) {   
    client_socket[i] = 0;   
  }
  //set master socket to allow multiple connections
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {   
    perror("setsockopt");   
    exit(EXIT_FAILURE);   
  }
  //try to specify maximum of 3 pending connections for the master socket  
  if (listen(sock_fd, 3) < 0) {   
    perror("listen");   
    exit(EXIT_FAILURE);   
  } 
  // Fork on every connection. 
  //accept the incoming connection  
  addrlen = sizeof(address);   
  puts("Waiting for connections ...");     
  while (1) {   
    //clear the socket set  
    FD_ZERO(&readfds);    
    //add master socket to set  
    FD_SET(sock_fd, &readfds);
    max_sd = sock_fd;        
    //add child sockets to set  
    for ( i = 0 ; i < max_clients ; i++) {
      //socket descriptor
      sd = client_socket[i];         
      //if valid socket descriptor then add to read list  
      if(sd > 0) {
        FD_SET( sd , &readfds); 
      }   
      //highest file descriptor number, need it for the select function  
      if (sd > max_sd) {
        max_sd = sd;
      }
    }   
    //wait for an activity on one of the sockets , timeout is NULL ,  
    //so wait indefinitely  
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno!=EINTR)) {   
      printf("select error");
    }   
             
    //If something happened on the master socket ,  
    //then its an incoming connection  
    if (FD_ISSET(sock_fd, &readfds)) {            
      if ((new_socket = accept(sock_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {   
        perror("accept");   
        exit(EXIT_FAILURE);   
      }       
      //inform user of socket number - used in send and receive commands  
      printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket,           		inet_ntoa(address.sin_addr) , ntohs (address.sin_port));   
           
      //send new connection greeting message  
      if (send(new_socket, message, strlen(message), 0) != strlen(message)) {   
        perror("send");   
      }   
      puts("Welcome");   
               
      //add new socket to array of sockets  
      for (i = 0; i < max_clients; i++) {   
        //if position is empty  
        if( client_socket[i] == 0 ) {   
          client_socket[i] = new_socket;   
          printf("Adding to list of sockets as %d\n" , i);   
                         
          break;   
        }   
      }   
    }   
             
    //else its some IO operation on some other socket 
    for (i = 0; i < max_clients; i++) {   
      sd = client_socket[i];
                
      if (FD_ISSET( sd , &readfds)) {   
        //Check if it was for closing , and also read the  
        //incoming message  
        if ((valread = read(sd, buffer, 1024)) == 0) {   
          //Somebody disconnected , get his details and print  
          getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);   
          printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr),
                  ntohs(address.sin_port));
          //Close the socket and mark as 0 in list for reuse  
          close( sd );
          client_socket[i] = 0;   
        } else {
          //Echo back the message that came in  
          //set the string terminating NULL byte on the end  
          //of the data read  
          buffer[valread] = '\0';   
          send(sd , buffer , strlen(buffer) , 0 );   
        }   
      }   
    }   
  }          
  return 0;
}

void Setup(char *dir) {
  struct sigaction sa;

  sa.sa_handler = sigchld_handler;  // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("Crawling directory tree starting at: %s\n", dir);
  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(dir, docs);
  printf("Crawled %d files.\n", NumElemsInHashtable(docs));

  // Create the index
  docIndex = CreateIndex();

  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex);
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex->ht));
}

int Cleanup() {
  DestroyOffsetIndex(docIndex);
  DestroyDocIdMap(docs);
  return 0;
}

int main(int argc, char **argv) {
  // Get args
  char* dir_to_crawl = argv[1];
  Setup(dir_to_crawl);

  // Step 1: Get address stuff
  int s;
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  s = getaddrinfo(NULL, argv[2], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }
  // Step 2: Open socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  // Step 3: Bind socket 
  if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind()");
    exit(1);
  }
  // Step 4: Listen on the socket
  if (listen(sock_fd, 10) != 0) {
      perror("listen()");
      exit(1);
  }
  struct sockaddr_in *result_addr = (struct sockaddr_in *) result->ai_addr;
  printf("Listening on file descriptor %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));
  // Step 5: Handle the connections
  int client_fd = accept(sock_fd, NULL, NULL);
  HandleConnections(client_fd);
  close(client_fd);
  // Got Kill signal
  close(sock_fd);
  Cleanup();
  return 0;
}
