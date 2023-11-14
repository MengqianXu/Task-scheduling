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
