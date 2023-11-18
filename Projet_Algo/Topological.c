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


//TU

enum NodeState {
    NOT_VISITED,
    IN_PROGRESS,
    COMPLETED
};

// 深度优先搜索
int topologicalSortDFS(struct Graph* graph, int v, int* visited, int* stack, int* top) {
    visited[v] = 1;

    for (int i = 0; i < graph->vertices; ++i) {
        if (graph->adjacencyMatrix[v][i] == 1) {
            if (visited[i] == IN_PROGRESS) {
                // 存在环路，图不是DAG
                return 0;
            } else if (visited[i] == NOT_VISITED) {
                if (!topologicalSortDFS(graph, i, visited, stack, top)) {
                    // 递归的下一层发现环路
                    return 0;
                }
            }
        }
    }

    stack[*top] = v;
    (*top)--;
    visited[v] = COMPLETED;

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
            if (!topologicalSortDFS(graph, i, visited, stack, &top)) {
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



