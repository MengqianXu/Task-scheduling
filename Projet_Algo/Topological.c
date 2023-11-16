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

// 深度优先搜索
void topologicalSortDFS(struct Graph* graph, int v, int* visited, int* stack, int* top) {
    visited[v] = 1;

    for (int i = 0; i < graph->vertices; ++i) {
        if (graph->adjacencyMatrix[v][i] == 1 && !visited[i]) { // 检查是否存在一条从当前顶点到邻居顶点的边，且邻居顶点尚未被访问
            dfs(graph, i, visited, stack, top);
        }
    }

    stack[*top] = v; //将当前顶点添加到栈中
    (*top)--; // 需要将栈顶指针向下移动，以便下次可以在正确的位置添加新的顶点
}

// 拓扑排序
int* topologicalSort(struct Graph* graph) {
    int* visited = (int*)malloc(graph->vertices * sizeof(int));
    int* stack = (int*)malloc(graph->vertices * sizeof(int));
    int top = graph->vertices - 1; // 将栈顶指针设置为最后一个元素的位置

    for (int i = 0; i < graph->vertices; ++i) { // 初始化
        visited[i] = 0;
    }

    for (int i = 0; i < graph->vertices; ++i) { //对每个尚未访问的顶点调用 dfs 函数
        if (!visited[i]) {
            dfs(graph, i, visited, stack, &top);
        }
    }

    free(visited);
    return stack;
}
