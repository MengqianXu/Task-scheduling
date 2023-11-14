#include <stdio.h>
#include <stdlib.h>
#include "Time.h"
#include "Graph.h"


// DFS递归函数（用于动态规划）
int calculateCompletionTime(struct Graph* graph, int v, int* visited, struct Task* tasks, int* completionTimes) {
    if (visited[v]) {
        return completionTimes[v];
    }

    visited[v] = 1;

    int maxTime = 0;

    for (int i = 0; i < graph->vertices; ++i) {
        if (graph->adjacencyMatrix[v][i] == 1) {
            int time = tasks[v].executionTime + calculateCompletionTime(graph, i, visited, tasks, completionTimes);
            maxTime = (time > maxTime) ? time : maxTime;
        }
    }

    completionTimes[v] = maxTime;
    return maxTime;
}

// 计算总完成时间
int calculateTotalCompletionTime(struct Graph* graph, struct Task* tasks) {
    int* visited = (int*)malloc(graph->vertices * sizeof(int));
    int* completionTimes = (int*)malloc(graph->vertices * sizeof(int));

    for (int i = 0; i < graph->vertices; ++i) {
        visited[i] = 0;
        completionTimes[i] = -1;  // 初始化为-1，表示未计算
    }

    int totalCompletionTime = 0;

    // 对每个顶点应用动态规划
    for (int i = 0; i < graph->vertices; ++i) {
        if (!visited[i]) {
            totalCompletionTime += calculateCompletionTime(graph, i, visited, tasks, completionTimes);
        }
    }

    free(visited);
    free(completionTimes);

    return totalCompletionTime;
}
