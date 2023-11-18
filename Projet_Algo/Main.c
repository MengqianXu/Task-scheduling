#include <stdio.h>
#include <stdlib.h>
#include "Topological.h"
#include "Graph.h"
#include "Time.h"

int main() {
  
    struct Task tasks[] = {
        {0, 2},
        {1, 4},
        {2, 3},
        {3, 5}
    };

    int numTasks = sizeof(tasks) / sizeof(tasks[0]);

    struct Graph* graph = createGraph(numTasks);

    
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);

 
    printf("Adjacency Matrix:\n");
    printGraph(graph);

    
    printf("\n");
    topologicalSort(graph);


    int totalCompletionTime = calculateTotalCompletionTime(graph, tasks);
    printf("\n\nTotal Completion Time: %d\n", totalCompletionTime);


    freeGraph(graph);

    return 0;
}





//TU main
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
