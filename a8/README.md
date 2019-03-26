# A8: Indexing multiple movie files

Here's a quick overview 

I will likely make some tweaks and **will** add an HTLL.a library for you to use. 

[Docs!!](html/index.html) (After you download, open html/index.html in your browser)


```FileParser```:
Responsible for reading in a file, checking for errors, and parse out movies.


```Movie```:
A struct holding all the Movie info (id, title, type, genre, etc.)


```MovieIndex```:
A hashtable that indexes movies according to specified field. Basically wraps a hashtable with functionality specific to adding/removing/destroying with MovieSets. The key is the description for the document set, and the value is the document set.


```MovieReport```:
Responsible for printing out a report given a MovieIndex, to the command line or file.


```MovieSet```:
A set of Movies that are connected somehow: they have the same value for a given field. If the field is Year, all the movies in the set are made in the same year. It consists of a description and a LinkedList of Movies.

The basic tasks you need to complete:

* Modify main to accept flags on input and do the right thing.
* Modify Movie struct to hold an array of genres
* Modify MovieSet to ensure no duplicates of movies
* Bulk up tests

To do this, go through each file, and identify the "TODO" tags (An easy way to do this might be to use grep). 

While you're going through the TODO tags, you'll see some questions where we ask you what to put somewhere. Collect those, and put them (with answers) in your README.md file for this assignment. (In MovieIndex, FileParser). 

TODOs: 

* FileCrawler.c:  // TODO: use namelist to find all the files and put them in map.
* Movie.c:  // TODO: Populate/Initialize movie.
* Movie.c:  // TODO: Change such that genres is an array, not just a string.
* Movie.h:    char *genres; // TODO: Make this an array of char*s
* MovieIndex.c:  // TODO: How to choose?
* MovieIndex.c:        // TODO: What to do if there are multiple genres?
* MovieIndex.c:      // TODO: how to deal with multiple genres??
* MovieReport.c:  // TODO: Implement this.
* MovieReport.c:  // TODO: Implement this. You might utilize OutputReport.
* example_indexer.c:  // TODO: Parse command-line args to index based on the correct field
* example_indexer.c:  // TODO: Output report to file, rather than terminal (see MovieReport.h)
* main.c:  // TODO: Read a specified row from a specified file into Movie.
* main.c:// TODO: This function pretty much runs the queries.
* main.c:      // TODO: What to do with the filename?
* main.c:    // TODO: What to do with the filename?

Experiment: 

What happens if the payload for each key in MovieIndex is a MovieStruct, rather than a docId/rowId pair? 




