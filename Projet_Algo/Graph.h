#ifndef GRAPH_H
#define GRAPH_H


struct Task {
    int taskNumber;
    int executionTime;
};

struct Graph {
    int vertices;
    int** adjacencyMatrix;
};

struct Graph* initializeGraph(int vertices) ;
void addEdge(struct Graph* graph, int src, int dest) ;
void printGraph(struct Graph* graph);
void freeGraph(struct Graph* graph) ;


#endif