#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "includes/QueryProtocol.h"


char *port_string = "1500";
unsigned short int port;
char *ip = "127.0.0.1";

#define BUFFER_SIZE 1000
int do_connect(char *host, char *port);
void read_response(int sock_fd);
char* get_response(int sock_fd);
int get_num_of_result(int sock_fd);
void send_message(char *msg, int sock_fd);

void RunQuery(char *query) {
  // Find the address
  // Create the socket
  // Connect to the server
  int sock_fd = do_connect(ip, port_string);
  // Do the query-protocol
  int check1 = CheckAck(get_response(sock_fd));
  if (check1 == 0) {
    printf("Receivd ACK from server.");
  }
  send_message(query, sock_fd);

  int num_of_result = get_num_of_result(sock_fd);
  printf("The number of results: %d\n", num_of_result);
  if (num_of_result != 0) {
    char result[50];
    for (int i = 0; i < num_of_result; i++) {
      SendAck(sock_fd);
      char* res = result;
      res = get_response(sock_fd);
      printf("movie result: %s\n", res);
    }
  } 
  SendAck(sock_fd);
  if (CheckGoodbye(get_response(sock_fd)) == 0) {
    // Close the connection
    close(sock_fd);
  }
  
  return;
}

int do_connect(char *host, char *port) {
  int s;
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo hints, *result;

  // Allows "global"
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* IPv4 only */
  hints.ai_socktype = SOCK_STREAM; /* TCP */
  s = getaddrinfo(host, port, &hints, &result);

  // If I can't get the address, write an error. 
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }

  // Try to connect; if I can't, write an error. 
  if (connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1) {
    perror("connect");
    exit(2);
  } else {
    printf("Connection is good!\n"); 
  }
  return sock_fd; 
}

void read_response(int sock_fd) {
  // Response
  char resp[1000];
  int len = read(sock_fd, resp, 999);
  resp[len] = '\0';
  printf("RECEIVED: %s\n", resp);
}

char* get_response(int sock_fd) {
  // Response
  char resp[1000];
  int len = read(sock_fd, resp, 999);
  resp[len] = '\0';
  char* res = resp;
  return res;
}

int get_num_of_result(int sock_fd) {
  // Response
  char resp[1000];
  int len = read(sock_fd, resp, 999);
  resp[len] = '\0';
  int n = atoi(resp);
  return n;
}

void send_message(char *msg, int sock_fd) {
  printf("SENDING: %s", msg);
  printf("===\n");
  write(sock_fd, msg, strlen(msg));
}


void RunPrompt() {
  char input[BUFFER_SIZE];

  while (1) {
    printf("Enter a term to search for, or q to quit: ");
    scanf("%s", input);

    printf("input was: %s\n", input);

    if (strlen(input) == 1) {
      if (input[0] == 'q') {
        printf("Thanks for playing! \n");
        return;
      }
    }
    printf("\n\n");
    RunQuery(input);
  }
}

// arguments will be like:
// ./queryclient [IP address] [port number]
int main(int argc, char **argv) {
  // Check/get arguments
  ip = argv[1];
  port_string = argv[2];
  // Get info from user
  // Run Query
  RunPrompt();
  return 0;
}
