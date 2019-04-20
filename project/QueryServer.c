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
Index runQuery(char *term);
int CreateMovieFromFileRow(char *file, long rowId, Movie** movie);

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

Index runQuery(char *term) {
  // Figure out how to get a set of Movies and create
  // a nice report from them.
  SearchResultIter results = FindMovies(docIndex, term);
  LinkedList movies = CreateLinkedList();

  if (results == NULL) {
    printf("No results for this term.\n");
    return NULL;
  } else {
    SearchResult sr = (SearchResult)malloc(sizeof(*sr));
    if (sr == NULL) {
      printf("Couldn't malloc SearchResult\n");
      return NULL;
    }
    int result;
    char *filename;

    // Get the last
    SearchResultGet(results, sr);
    filename = GetFileFromId(docs, sr->doc_id);

    Movie *movie;
    CreateMovieFromFileRow(filename, sr->row_id, &movie);
    InsertLinkedList(movies, movie);

    // Check if there are more
    while (SearchResultIterHasMore(results) != 0) {
      result =  SearchResultNext(results);
      if (result < 0) {
        printf("error retrieving result\n");
        break;
      }
      SearchResultGet(results, sr);
      char *filename = GetFileFromId(docs, sr->doc_id);

      Movie *movie;
      CreateMovieFromFileRow(filename, sr->row_id, &movie);
      InsertLinkedList(movies, movie);
    }

    free(sr);
    DestroySearchResultIter(results);
  }
  // Now that you have all the search results, print them out nicely.
  Index index = BuildMovieIndex(movies, Type);
  return index;
}

int CreateMovieFromFileRow(char *file, long rowId, Movie** movie) {
  FILE *fp;

  char buffer[1000];

  fp = fopen(file, "r");

  int i = 0;
  while (i <= rowId) {
    fgets(buffer, 1000, fp);
    i++;
  }
  // taking \n out of the row
  buffer[strlen(buffer)-1] = ' ';
  // Create movie from row
  *movie = CreateMovieFromRow(buffer);
  fclose(fp);
  return 0;
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
  printf("Listening on file descriptor %d, port %d\n",
    sock_fd, ntohs(result_addr->sin_port));
  // Step 5: Handle clients that connect
  printf("Waiting for connection...\n");
  while (1) {
    int client_fd = accept(sock_fd, NULL, NULL);
    printf("Connection made: client_fd=%d\n", client_fd);
    SendAck(client_fd);
    // here should get the query from client
    char* keyword = recieve_message(client_fd);
    printf("The keyword for searching is: %s\n", keyword);
    // here after use keyword to find movies, should get results number
    // and send the number to client
    SearchResultIter resultsIter = FindMovies(docIndex, keyword);
    if (resultsIter == NULL) {
      printf("No results for this term.\n");
      close(client_fd);
      Cleanup();
      return 0;
    }

    int num_result = NumResultsInIter(resultsIter);
    char result_num_string[50];
    sprintf(result_num_string, "%d", num_result);
    send_message(result_num_string, client_fd);
    // here should use the keyword for runQuery.
    if (num_result != 0) {
      Index res_idx = runQuery(keyword);
      HTIter htiter = CreateHashtableIterator(res_idx->ht);
      HTKeyValue movie_set;
      HTIteratorGet(htiter, &movie_set);
      LLIter lliter = CreateLLIter(((SetOfMovies)movie_set.value)->movies);
      Movie *movie;
      LLIterGetPayload(lliter, (void**)&movie);
      char* res = recieve_message(client_fd);
      if (CheckAck(res) == 0) {
        for (int i = 0; i < num_result; i++) {
          // here should send the real results
          send_message(movie->title, client_fd);
          printf("Sent result of %d\n", i + 1);
          LLIterNext(lliter);
          LLIterGetPayload(lliter, (void**)&movie);
          if (CheckAck(recieve_message(client_fd)) != 0) {
            break;
          }
        }
      }
      DestroyLLIter(lliter);
      DestroyHashtableIterator(htiter);
    }
    SendGoodbye(client_fd);
//  Step 6: Close the socket
//  Got Kill signal
//  if (CheckGoodbye(recieve_message(client_fd)) == 0) {
//    close(client_fd);
//    Cleanup();
//  }
  }
  return 0;
}
