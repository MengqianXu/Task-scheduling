#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "Topological.h"



int DFS(struct Graph* graph, int v, int* visited, int* stack, int* top) {
    visited[v] = 1;

    for (int i = 0; i < graph->vertices; ++i) {
        if (graph->adjacencyMatrix[v][i] == 1) {
            if (visited[i] == IN_PROGRESS) {
                // 存在环路，图不是DAG
                return 0;
            } else if (visited[i] == NOT_VISITED) {
                visited[i] = IN_PROGRESS;
                int flag = DFS(graph, i, visited, stack, top);
                if (!flag) {
                    // 递归的下一层发现环路
                    return 0;
                }
                visited[i] = VISITED;
            }
        }
    }

    stack[*top] = v;
    (*top)--;
    

    return 1;
}

// 拓扑排序
int* topologicalSort(struct Graph* graph) {
    int* visited = (int*)malloc(graph->vertices * sizeof(int));
    int* stack = (int*)malloc(graph->vertices * sizeof(int));
    int top = graph->vertices - 1;

    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = NOT_VISITED;
    }

    for (int i = 0; i < graph->vertices; ++i) {
        if (visited[i] == NOT_VISITED) {
            int flag = DFS(graph, i, visited, stack, &top);
            if (!flag) {
                // 存在环路，图不是DAG
                free(visited);
                free(stack);
                return NULL;
            }
        }
    }

    free(visited);
    return stack;
}



