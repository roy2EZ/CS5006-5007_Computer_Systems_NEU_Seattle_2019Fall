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


/*
 * Dijkstra
 * G -- the graph
 * vs -- start vertex
 * prev -- prev[i] is the vertex before i, in all vertexs in the shortest path from vs to i
 * dist -- dist[i] is the distance of the shortest path from vs to i
 */
void dijkstra(Graph G, int vs, int prev[], int dist[])
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
        // the shortest path to i is the cost(distance) from vs to i          
        dist[i] = G.matrix[vs][i];
    }

    // init vs
    flag[vs] = 1;
    dist[vs] = 0;

    // traversing G.vexnum-1 times, each time find a shortest path to a vertex
    for (i = 1; i < G.vexnum; i++)
    {
        // find shortest path to current vertex
        // which means in the unupdated vertexs, find the closest vertex k
        min = -1;
        for (j = 0; j < G.vexnum; j++)
        {
            if (flag[j]==0 && dist[j]<min)
            {
                min = dist[j];
                k = j;
            }
        }
        // mark k as the gotten shortest path
        flag[k] = 1;

        // update current shortest path and prev vertex
        for (j = 0; j < G.vexnum; j++)
        {
            tmp = (G.matrix[k][j]== -1 ? -1 : (min + G.matrix[k][j])); 
            if (flag[j] == 0 && (tmp  < dist[j]) )
            {
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }

    // print result
    printf("dijkstra(%c): \n", G.vexs[vs]);
    for (i = 0; i < G.vexnum; i++)
        printf("  shortest(%c, %c)=%d\n", G.vexs[vs], G.vexs[i], dist[i]);
}


void main()
{
    int prev[MAX] = {0};
    int dist[MAX] = {0};
    Graph* pG;
    //create the graph from the csv file
    pG = create_example_graph();
    // dijkstra算法获取"第4个顶点"到其它各个顶点的最短距离
    dijkstra(*pG, 4, prev, dist);
}

