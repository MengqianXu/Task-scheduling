#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "Graph.h"


// tu 解决分配内存和泄漏问题
void* allocateMemory(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        printf("Memory allocation failed.\n");
    }
    return ptr;
}

struct Graph* initializeGraph(int vertices) {
    struct Graph* graph = allocateMemory(sizeof(struct Graph));
    if (graph == NULL) { // tu
        return NULL;
    }

    graph->vertices = vertices; 

    graph->tasks = allocateMemory(vertices * sizeof(struct Task*)); // tu
    if (graph->tasks == NULL) {
        free(graph);
        return NULL;
    }

    graph->adjacencyMatrix = allocateMemory(vertices * sizeof(int*));
    if (graph->adjacencyMatrix == NULL) { // tu
        free(graph->tasks);
        free(graph);
        return NULL;
    }

    for (int i = 0; i < vertices; ++i) {
        graph->adjacencyMatrix[i] = allocateMemory(vertices * sizeof(int));
        if (graph->adjacencyMatrix[i] == NULL) { // tu
            for (int j = 0; j < i; ++j) {
                free(graph->adjacencyMatrix[j]);
            }
            free(graph->adjacencyMatrix);
            free(graph->tasks);
            free(graph);
            return NULL;
        }
        
        for (int j = 0; j < vertices; ++j) {
            graph->adjacencyMatrix[i][j] = 0;  // 初始化为0表示没有边
        }
    }

    return graph;
}


void addEdge(struct Graph* graph, int src, int dest) {
    graph->adjacencyMatrix[src][dest] = 1;
}

// 打印邻接矩阵
void printGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            printf("%d ", graph->adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
}

// 释放图的内存
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->adjacencyMatrix[i]);
        free(graph->tasks[i]); // tu
    }
    free(graph->adjacencyMatrix);
    free(graph->tasks); // tu 释放了任务的内存
    free(graph);
}


