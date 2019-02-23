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
    char file_name_node[] = "nodes.csv";
    FILE *fp1;
    fp1 = fopen(file_name_node, "r");
    if (!fp1) {
        fprintf(stderr, "failed to open file for reading\n");
    }
    char line1[1001];
    char *result1 = NULL;
    fgets(line1, MAX, fp1); // skip first line
    while(fgets(line1, MAX, fp1) != NULL) {
        result1 = strtok(line1, ",");
        int i = 0;
        while( result1 != NULL ) {
            if (i == 1) {
                gVexs[i] = *result1;
            } else {
                i++;
            }
            result1 = strtok(NULL, ",");
        }
    }
    fclose(fp1);
    
    // get edge from edges_FINAL.csv
    static EData gEdges[MAX];
    char file_name_edge[] = "edges_FINAL.csv";
    FILE *fp2;
    fp2 = fopen(file_name_edge, "r");
    if (!fp2) {
        fprintf(stderr, "failed to open file for reading\n");
    }
    char line2[MAX];
    char *result2 = NULL;
    fgets(line2, MAX, fp2);// skip first line

    while(fgets(line2, MAX, fp2) != NULL) {
        result2 = strtok(line2, " ");
        int i= 0;
        while( result2 != NULL ) {
          if (i == 0) {
            gEdges[i].start = *result2;
          } else if (i == 1) {
            gEdges[i].end = *result2;
          } else {
            gEdges[i].weight = atoi(result2);
          }
          i++;
        }
        result2 = strtok(NULL, ",");
    }    
    fclose(fp2);

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

/*
 * get weight of time from start to end of the edge, if start to end is not connected, return INF
 */
int get_weight(LGraph G, int start, int end)
{
    ENode *node;


    if (start==end)
        return 0;


    node = G.vexs[start].first_edge;
    while (node!=NULL)
    {
        if (end==node->ivex)
            return node->weight;
        node = node->next_edge;
    }


    return INF;
}

/*
 * Dijkstra
 * G -- the graph
 * vs -- start vertex
 * prev -- prev[i] is the vertex before i, in all vertexs in the shortest path from vs to i
 * time -- time[i] is the time of the shortest path from vs to i
 */
void dijkstra(LGraph G, int vs, int end, int prev[], int time[])
{
    int i,j,k;
    int min;
    int tmp;
    int flag[MAX];      // flag[i] = 1 means got the shortest path from vs to i
    
    // init
    for (i = 0; i < G.vexnum; i++)
    {
        // the shortest path to i did not get yet
        flag[i] = 0;
        // prev of i is 0 
        prev[i] = 0;
        // the shortest path to i is the cost(time) from vs to i
        time[i] = get_weight(G, vs, i);
    }

    // init vs
    flag[vs] = 1;
    time[vs] = 0;

    // traversing G.vexnum-1 times, each time find a shortest path to a vertex
    for (i = 1; i < G.vexnum; i++)
    {
        // find shortest path to current vertex
        // which means in the unupdated vertexs, find the closest vertex k
        min = INF;
        for (j = 0; j < G.vexnum; j++)
        {
            if (flag[j]==0 && time[j]<min)
            {
                min = time[j];
                k = j;
            }
        }
        // mark k as the gotten shortest path
        flag[k] = 1;
        // update current shortest path and prev vertex
        for (j = 0; j < G.vexnum; j++)
        {
            tmp = get_weight(G, k, j);
            tmp = (tmp==INF ? INF : (min + tmp));
            if (flag[j] == 0 && (tmp  < time[j]) )
            {
                time[j] = tmp;
                prev[j] = k;
            }
        }
    }

    // print result
    printf("%d \n", time[end]);
}

void main()
{
    int prev[MAX] = {0};
    int dist[MAX] = {0};
    LGraph* pG;
    //create the graph from the csv file
    pG = create_lgraph();
    // dijkstra got the shortest path from Seattle_WA to Boston_MA
    printf("Shortest time Seattle to Boston: ");
    dijkstra(*pG, 824, 97, prev, dist);
    // dijkstra got the shortest path from Minneapolis_MN to Ann Arbor_MI
    printf("Shortest time Minneapolis to Ann Arbor: ");
    dijkstra(*pG, 573, 28, prev, dist);
}

