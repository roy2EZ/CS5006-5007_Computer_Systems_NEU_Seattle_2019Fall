#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>


#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "DocIdMap.h"
#include "htll/Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "FileCrawler.h"

DocIdMap docs;
Index docIndex;

#define BUFFER_SIZE 1000
#define SEARCH_RESULT_LENGTH 1500
char movieSearchResult[SEARCH_RESULT_LENGTH];

int Cleanup();
char* recieve_message(int client_fd);
void send_message(char* msg, int sock_fd);

void sigint_handler(int sig) {
  write(0, "Exit signal sent. Cleaning up...\n", 34);
  Cleanup();
  exit(0);
}


void Setup(char *dir) {
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

char* recieve_message(int client_fd) {
  char buffer[1000];
  int len = read(client_fd, buffer, sizeof(buffer) - 1);
  buffer[len] = '\0';
  printf("SERVER RECEIVED: %s \n", buffer); 
  char* res = buffer;
  return res;
}

void send_message(char* msg, int sock_fd) {
  
  write(sock_fd, msg, strlen(msg));
}


int main(int argc, char **argv) {
  // Get args
  char* dir_to_crawl = argv[1];
  
  // Setup graceful exit
  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  Setup(dir_to_crawl);

  // Step 1: get address/port info to open
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  int s;
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
  // Step 5: Handle clients that connect
  printf("Waiting for connection...\n");
  int client_fd = accept(sock_fd, NULL, NULL);
  printf("Connection made: client_fd=%d\n", client_fd);

  SendAck(client_fd);
  
  // here should get the query from client 
  // char* keyword = recieve_message(client_fd);

  // here should use the keyword for runQuery. 
  // runQuery(keyword);

  // here after runQuery of keyword, should get results, and send the result number
  // and send the number and results back to client
  int result_num = 5;
  // char results[result_num];
  for (int i = 0; i < result_num; i++) {
    // here should put real movie results name
    //results[i] = "movie name";
  }
  send_message("5", client_fd);
  char* res = recieve_message(client_fd);
  if (CheckAck(res) == 0) {
    for (int i = 0; i < result_num; i++) {
      send_message("movie name", client_fd);
      if (CheckAck(recieve_message(client_fd)) != 0) {
        break;
      }
    }
  }
  SendGoodbye(client_fd);
  
  // Step 6: Close the socket
  // Got Kill signal
  close(sock_fd);

  Cleanup();

  return 0;
}
