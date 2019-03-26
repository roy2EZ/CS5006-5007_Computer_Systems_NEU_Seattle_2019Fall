# define the commands we'll use for compilation and library building
AR = ar
ARFLAGS = rcs
CC = gcc

GOOGLE_TEST_INCLUDE=${HOME}/src/googletest-release-1.8.0/googletest/include/

# define useful flags to cc/ld/etc.
CFLAGS = -g -Wall -I. -I.. -Ihtll 
LDFLAGS = -L. 


#define common dependencies
OBJS = MovieSet.o DocIdMap.o FileParser.o FileCrawler.o MovieIndex.o Assert007.o Movie.o QueryProcessor.o MovieReport.o
HEADERS = FileParser.h FileCrawler.h DocIdMap.h MovieIndex.h MovieSet.h Movie.h Assert007.h MovieReport.h


# compile everything
all: main test indexer


main: main.c $(OBJS)
	gcc -Wall -g -o main main.c $(OBJS) -L. libHtll.a
	
indexer: example_indexer.c $(OBJS)
	gcc -Wall -g -o indexer example_indexer.c $(OBJS) -L. libHtll.a


%.o: %.c $(HEADERS) FORCE
	$(CC) $(CFLAGS) -c $<

test: test_suite.o $(OBJS)
	g++ -o test_suite test_suite_a4.o $(OBJS) -L. libHtll.a \
		 -L${HOME}/lib/gtest -lgtest -lpthread

test_suite.o: test_suite_a4.cc
	g++ -c -Wall -I $(GOOGLE_TEST_INCLUDE) test_suite_a4.cc \
		-o test_suite_a4.o


clean: FORCE
	/bin/rm -f *.o *~ main test_suite indexer

FORCE:
