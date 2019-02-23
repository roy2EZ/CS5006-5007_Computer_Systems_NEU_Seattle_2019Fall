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

/*
 * return the position of char ch in the matrix 
 */
static int get_position(Graph G, char ch)
{
    int i;
    for(i=0; i<G.vexnum; i++)
        if(G.vexs[i]==ch)
            return i;
    return -1;
}


/*
 * read a char
 */
static char read_char()
{
    char ch;

    do {
        ch = getchar();
    } while(!isLetter(ch));

    return ch;
}

/*
 * create the graph from csv
 */
Graph* create_example_graph()
{
    char file_name[] = "miles_graph_FINAL.csv";
    FILE *fp;
    fp = fopen(file_name, "r");
    
    if (!fp) {
        fprintf(stderr, "failed to open file for reading\n");
    }

    char line[MAX];
    char *result = NULL;
    fgets(line, MAX, fp);
    char vexs[MAX];
    int matrix[MAX][MAX];

    while(fgets(line, MAX, fp) != NULL) {
        
        result = strtok(line, ",");
        int i = 0;
	int j = 0;
        while( result != NULL ) {
            if (i=0) {
                vexs[i] = *result;
                
            }
            else{

                matrix[j][i] = *result;
                i++;
            }
            result = strtok(NULL, ",");
            j++;
        }
    }
    
    int vlen = LENGTH(vexs);
    int i, j;
    Graph* pG;
    
    // input the number of vex and edge
    if ((pG=(Graph*)malloc(sizeof(Graph))) == NULL )
        return NULL;
    memset(pG, 0, sizeof(Graph));

    // init the vexnum
    pG->vexnum = vlen;
    // init the vexs
    for (i = 0; i < pG->vexnum; i++)
        pG->vexs[i] = vexs[i];

    // init the edge
    for (i = 0; i < pG->vexnum; i++)
        for (j = 0; j < pG->vexnum; j++)
            pG->matrix[i][j] = matrix[i][j];

    // count amount of
    for (i = 0; i < pG->vexnum; i++)
        for (j = 0; j < pG->vexnum; j++)
            if (i!=j && pG->matrix[i][j]!=-1)
                pG->edgnum++;
    pG->edgnum /= 2;

    return pG;
}

/*
 * return the first adjacency vertex of v,  return -1 otherwise 
 */
static int first_vertex(Graph G, int v)
{
    int i;

    if (v<0 || v>(G.vexnum-1))
        return -1;

    for (i = 0; i < G.vexnum; i++)
        if (G.matrix[v][i]!=0 && G.matrix[v][i]!=-1)
            return i;

    return -1;
}

/*
 * return next index of w to v, return -1 otherwise
 */
static int next_vertix(Graph G, int v, int w)
{
    int i;

    if (v<0 || v>(G.vexnum-1) || w<0 || w>(G.vexnum-1))
        return -1;

    for (i = w + 1; i < G.vexnum; i++)
        if (G.matrix[v][i]!=0 && G.matrix[v][i]!= (-1))
            return i;

    return -1;
}

