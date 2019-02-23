#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX         100012
#define INF         (~(0x1<<31))  // define the infinity which is 0X7FFFFFFF
#define isLetter(a) ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')))
#define LENGTH(a)   (sizeof(a)/sizeof(a[0]))

// struct of edge node in the Adjacency List
typedef struct _ENode
{
    int ivex;                   // the vex of this edge
    int weight;                 // the weight(cost) of this edge
    struct _ENode *next_edge;   // pointer to next edge
}ENode, *PENode;

// struct of vertex node in the Adjacency List
typedef struct _VNode
{
    char data;              // data of vertex
    ENode *first_edge;      // pointer to the first edge to this vertex
}VNode;
