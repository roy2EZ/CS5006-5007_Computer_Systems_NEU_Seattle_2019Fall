#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX         1001                 // Max line value

#define isLetter(a) ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')))
#define LENGTH(a)   (sizeof(a)/sizeof(a[0]))

// Adjacency Matrix
typedef struct _graph
{
    char vexs[MAX];       // set of Vertex
    int vexnum;           // Vertex number
    int edgnum;           // edge
    int matrix[MAX][MAX]; // Adjacency Matrix
}Graph, *PGraph;

// struct of edge
typedef struct _EdgeData
{
    char start; // start of edge
    char end;   // end of edge
    int weight; // weight of edge
}EData;

