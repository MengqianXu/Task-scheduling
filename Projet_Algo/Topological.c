#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "Topological.h"

// DFS递归函数
void topologicalSortDFS(struct Graph* graph, int v, int* visited, int* stack, int* stackIndex) {
    visited[v] = 1;

    for (int i = 0; i < graph->vertices; ++i) {
        if (!visited[i] && graph->adjacencyMatrix[v][i] == 1) {
            topologicalSortDFS(graph, i, visited, stack, stackIndex);
        }
    }

    stack[(*stackIndex)++] = v;
}

// 拓扑排序
void topologicalSort(struct Graph* graph) {
    int* visited = (int*)malloc(graph->vertices * sizeof(int));
    int* stack = (int*)malloc(graph->vertices * sizeof(int));
    int stackIndex = 0;

    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = 0;
    }

    for (int i = 0; i < graph->vertices; ++i) {
        if (!visited[i]) {
            topologicalSortDFS(graph, i, visited, stack, &stackIndex);
        }
    }

    printf("Topological Order: ");
    for (int i = graph->vertices - 1; i >= 0; --i) {
        printf("%d ", stack[i]);
    }

    free(visited);
    free(stack);
}