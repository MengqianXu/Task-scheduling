#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <stdlib.h>

struct Graph {
    int vertices;
    struct Task** tasks;//stock des pointeurs vers toutes les tâches.
    int* inDegree; //stock le degré indegree de chaque nœud.
};

void* allocateMemory(size_t size);
struct Graph* initializeGraph(int vertices);
void freeGraph(struct Graph* graph);




#endif
