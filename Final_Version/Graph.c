#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "Task.h"

// Variable globale pour le graphe
struct Graph* graph;

// Fonction pour allouer de la mémoire
void* allocateMemory(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        printf("echec。\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Initialiser le graphe
struct Graph* initializeGraph(int vertices) {
    struct Graph* graph = (struct Graph*)allocateMemory(sizeof(struct Graph));
    graph->vertices = vertices;

    // Allouer de la mémoire pour le tableau des tâches et le tableau des degrés entrants
    graph->tasks = (struct Task**)allocateMemory(vertices * sizeof(struct Task*));
    graph->inDegree = (int*)allocateMemory(vertices * sizeof(int));

    // Initialiser le tableau des tâches et le tableau des degrés entrants
    for (int i = 0; i < vertices; ++i) {
        graph->tasks[i] = NULL;  // Initialiser le tableau des tâches à NULL
        graph->inDegree[i] = 0;  // Initialiser le tableau des degrés entrants à 0
    }

    return graph;
}

// Libérer la mémoire du graphe
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->tasks[i]->dependencies);
        free(graph->tasks[i]);
    }
    free(graph->tasks);
    free(graph->inDegree);
    free(graph);
}
