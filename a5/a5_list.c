/**
 * Dijkstra get shortest path with Adjacency List
 * @author Rongyi Chen
 * @date 2019/02/22
 * @reference https://www.cnblogs.com/skywang12345/p/3711512.html#anchor3
 */

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

typedef struct _LGraph
{
    int vexnum;             // amount of vertex
    int edgnum;             // amount of dege
    VNode vexs[MAX];
}LGraph;

static int get_position(LGraph G, char ch)
{
    int i;
    for(i=0; i<G.vexnum; i++)
        if(G.vexs[i].data==ch)
            return i;
    return -1;
}

// struct of edge
typedef struct _edata
{
    char start; // start vertex of the edge
    char end;   // end vertex of the edge
    int weight; // weight of the edge
}EData;


static char read_char()
{
    char ch;
    do {
        ch = getchar();
    } while(!isLetter(ch));
    return ch;
}

// add node to end of list
static void link_last(ENode *list, ENode *node)
{
    ENode *p = list;


    while(p->next_edge)
        p = p->next_edge;
    p->next_edge = node;
}

/*
 * create list from the csv file
 */
LGraph* create_lgraph()
{
    // get vertex from nodes.csv
    static char gVexs[1000];     
    char file_name[] = "nodes.csv";
    FILE *fp;
    fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "failed to open file for reading\n");
    }
    char line[1001];
    char *result = NULL;
    fgets(line, MAX, fp); // skip first line
    while(fgets(line, MAX, fp) != NULL) {
        result = strtok(line, ",");
        int i = 0;
        while( result != NULL ) {
            if (i == 1) {
                gVexs[i] = *result;
            } else {
                i++;
            }
            result = strtok(NULL, ",");
        }
    }
    fclose (fp);
    
    // get edge from edges_FINAL.csv
    static EData gEdges[MAX][MAX];
    char file_name[] = "edges_FINAL.csv";
    FILE *fp;
    fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "failed to open file for reading\n");
    }
    char line[MAX];
    char *result = NULL;
    fgets(line, MAX, fp);// skip first line

    while(fgets(line, MAX, fp) != NULL) {
        result = strtok(line, " ");
        int i, j = 0;
        while( result != NULL ) {
          if (j == 0 or j == 1) {
            gEdges[i][j] = *result;
          } else {
            gEdges[i][j] = atoi(result);
          }
          j++;
        }
        result = strtok(NULL, ",");
      }
      i++;
    }    
    fclose (fp);

    char c1, c2;
    int vlen = 1000;
    int elen = MAX;
    int i, p1, p2;
    int weight;
    ENode *node1, *node2;
    LGraph* pG;


    if ((pG=(LGraph*)malloc(sizeof(LGraph))) == NULL )
        return NULL;
    memset(pG, 0, sizeof(LGraph));


    // initialize vexnum and edgnum
    pG->vexnum = vlen;
    pG->edgnum = elen;
    // initialize vertex
    for(i=0; i<pG->vexnum; i++)
    {
        pG->vexs[i].data = gVexs[i];
        pG->vexs[i].first_edge = NULL;
    }


    // initialize edges
    for(i=0; i<pG->edgnum; i++)
    {
        c1 = gEdges[i].start;
        c2 = gEdges[i].end;
        weight = gEdges[i].weight;


        p1 = get_position(*pG, c1);
        p2 = get_position(*pG, c2);


        // init node1
        node1 = (ENode*)malloc(sizeof(ENode));
        node1->ivex = p2;
        node1->weight = weight;
        // add node1 to end of list
        if(pG->vexs[p1].first_edge == NULL)
            pG->vexs[p1].first_edge = node1;
        else
            link_last(pG->vexs[p1].first_edge, node1);
        // init node2
        node2 = (ENode*)malloc(sizeof(ENode));
        node2->ivex = p1;
        node2->weight = weight;
        // add node1 to end of list
        if(pG->vexs[p2].first_edge == NULL)
            pG->vexs[p2].first_edge = node2;
        else
            link_last(pG->vexs[p2].first_edge, node2);
    }


    return pG;
}
