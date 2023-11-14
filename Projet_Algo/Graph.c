#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"


struct Graph* initializeGraph(int vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->vertices = vertices;

    
    graph->adjacencyMatrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; ++i) {
        graph->adjacencyMatrix[i] = (int*)malloc(vertices * sizeof(int));
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
    }
    free(graph->adjacencyMatrix);
    free(graph);
}



