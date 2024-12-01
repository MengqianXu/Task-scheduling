#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "Graph.c"
#include "Task.c"
#include "TopoSort_Dynamique.c"



struct Graph* graph;

// Fonction principale
int main() {
    const int num_tasks = 6;
    const int num_threads = 6;
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    graph = initializeGraph(num_tasks);

    // Ajouter des tâches
    for (int i = 0; i < num_tasks; ++i) {
        //int duration = 300;
        int duration = rand() % 900 + 100;
        int dependencies[num_tasks];
        int num_dependencies = 0;

        for (int j = 0; j < i; ++j) {
            // Pour chaque tâche précédente, il y a 50% de chance de l'ajouter comme dépendance
            if (rand() % 2) {
                dependencies[num_dependencies] = j;
                num_dependencies++;
            }
        }

        // Utiliser un tableau alloué dynamiquement pour stocker les dépendances
        int* dependencies_copy = (int*)allocateMemory(num_dependencies * sizeof(int));
        memcpy(dependencies_copy, dependencies, num_dependencies * sizeof(int));

        addTask(graph, i, duration, dependencies_copy, num_dependencies);
    }


    // Trier topologiquement
    int* stack = (int*)allocateMemory(num_tasks * sizeof(int));
    topologicalSort(graph, stack);

    // Calculer les heures de début les plus tôt
    int* earliest_start_times = (int*)allocateMemory(num_tasks * sizeof(int));
    calculateEarliestStartTimes(graph, earliest_start_times);

    // Allouer des threads de manière dynamique
    dynamicThreadAllocation(graph, num_threads);

    // Imprimer l'ordre d'exécution des tâches et le temps total
    int total_time = 0;
    for (int i = 0; i < num_tasks; ++i) {
        printf("Heure de début de la tâche %d : %d, heure de fin : %d\n", graph->tasks[i]->id, graph->tasks[i]->start_time, graph->tasks[i]->end_time);
        if (graph->tasks[i]->end_time > total_time) {
            total_time = graph->tasks[i]->end_time;
        }
    }

    printf("Temps total : %d\n", total_time);

    // Libérer la mémoire du graphique
    free(stack);
    freeGraph(graph);

    return 0;
}
