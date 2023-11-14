#ifndef TOPOLOGICAL_H
#define TOPOLOGICAL_H

void topologicalSortDFS(struct Graph* graph, int v, int* visited, int* stack, int* stackIndex);
void topologicalSort(struct Graph* graph) ;

#endif