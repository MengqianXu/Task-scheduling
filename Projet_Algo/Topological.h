#ifndef TOPOLOGICAL_H
#define TOPOLOGICAL_H
#include <stdio.h>
#include <stdlib.h>

enum NodeState {
    NOT_VISITED,
    IN_PROGRESS,
    VISITED
};

int  DFS(struct Graph* graph, int v, int* visited, int* stack, int* stackIndex);
int* topologicalSort(struct Graph* graph);

#endif
