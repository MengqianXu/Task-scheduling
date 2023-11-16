#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h> // tu size_t在此定义

struct Task {
    int taskNumber;
    int executionTime;
};

struct Graph {
    int vertices;
    struct Task** tasks; // tu 在图的每个顶点上存储任务信息,便于访问和操作与该顶点关联
    int** adjacencyMatrix;
};

void* allocateMemory(size_t size); // tu 处理内存分配和错误检查
struct Graph* initializeGraph(int vertices) ;
void addEdge(struct Graph* graph, int src, int dest) ;
void printGraph(struct Graph* graph);
void freeGraph(struct Graph* graph) ;


#endif
