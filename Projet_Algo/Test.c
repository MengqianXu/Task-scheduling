#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct Task {
    int taskNumber;
    int executionTime;
};

struct Graph {
    int vertices;
    int** adjacencyMatrix;
};

// 假设你的其他函数和allocateMemory函数在这里
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

    graph->adjacencyMatrix = allocateMemory(vertices * sizeof(int*));
    if (graph->adjacencyMatrix == NULL) { // tu

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
        
    }
    free(graph->adjacencyMatrix);
    
    free(graph);
}


//main
int main() {
    // 创建一个图，假设有5个顶点
    int numVertices = 5;
    struct Graph* myGraph = initializeGraph(numVertices);

    if (myGraph == NULL) {
        printf("Graph initialization failed.\n");
        return 1; // 退出程序，表示出错
    }

    // 添加一些边
    addEdge(myGraph, 0, 1);
    addEdge(myGraph, 1, 2);
    addEdge(myGraph, 2, 3);
    addEdge(myGraph, 3, 4);
    addEdge(myGraph, 4, 0);

    // 打印邻接矩阵
    printf("Adjacency Matrix:\n");
    printGraph(myGraph);

    // 释放图的内存
    freeGraph(myGraph);

    return 0; // 表示程序成功执行
}

