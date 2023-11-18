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
    // 创建一个有向图
    int numVertices = 6;
    struct Graph* myGraph = initializeGraph(numVertices);

    // test 1:添加有向边，创建环路
    addEdge(cyclicGraph, 0, 1);
    addEdge(cyclicGraph, 1, 2);
    addEdge(cyclicGraph, 2, 0);
  
    // test 2:添加有向边,无环路
    addEdge(myGraph, 5, 2);
    addEdge(myGraph, 5, 0);
    addEdge(myGraph, 4, 0);
    addEdge(myGraph, 4, 1);
    addEdge(myGraph, 2, 3);
    addEdge(myGraph, 3, 1);

    // 打印图的邻接矩阵
    printf("Adjacency Matrix:\n");
    printGraph(myGraph);

    // 进行拓扑排序
    int* result = topologicalSort(myGraph);

    if (result != NULL) {
        // 打印拓扑排序结果
        printf("Topological Sort Order: ");
        for (int i = 0; i < myGraph->vertices; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");

        // 释放图的内存
        freeGraph(myGraph);
        free(result);
    } else {
        // 图中存在环路，无法进行拓扑排序
        printf("Graph has a cycle. Topological sorting is not possible.\n");
        freeGraph(myGraph);
    }

    return 0;
}


//TU main 测time
int main() {
    // 创建一个有向图
    int numVertices = 6;
    struct Graph* myGraph = initializeGraph(numVertices);

    // 添加有向边
    addEdge(myGraph, 0, 1);
    addEdge(myGraph, 0, 2);
    addEdge(myGraph, 1, 3);
    addEdge(myGraph, 2, 3);
    addEdge(myGraph, 3, 4);
    addEdge(myGraph, 4, 5);

    // 打印图的邻接矩阵
    printf("Adjacency Matrix:\n");
    printGraph(myGraph);

    // 进行拓扑排序
    int* result = topologicalSort(myGraph);

    // 创建任务数组
    struct Task* tasks = (struct Task*)malloc(numVertices * sizeof(struct Task));
    for (int i = 0; i < numVertices; ++i) {
        tasks[i].taskNumber = i;
        tasks[i].executionTime = i + 1; // 假设执行时间为任务编号加1
    }

    // 计算总完成时间
    int totalCompletionTime = calculateTotalCompletionTime(myGraph, tasks);

    // 打印结果
    printf("Total Completion Time: %d\n", totalCompletionTime);

    return 0;
}

