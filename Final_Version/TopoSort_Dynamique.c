#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Graph.h"
#include "Task.h"
#include "TopoSort_Dynamique.h"


struct Graph* graph;


// Tri topologique
void topologicalSort(struct Graph* graph, int* stack) {
    int stackIndex = 0;

    // Créer une file d'attente pour le tri topologique
    int* queue = (int*)allocateMemory(graph->vertices * sizeof(int));
    int front = 0, rear = 0;
    //front : indique la position du prochain élément à traiter.
    //Rear : indique la prochaine position où l'élément doit être inséré.
    // Ajouter les nœuds de degré entrant 0 à la file d'attente
    for (int i = 0; i < graph->vertices; ++i) {
        if (graph->inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }

    while (front != rear) {
        int u = queue[front++];

        // Ajouter le nœud actuel aux résultats du tri topologique
        stack[stackIndex++] = u;

        // Mettre à jour le degré entrant des nœuds adjacents, si le degré entrant est 0, alors ajouter à la file d'attente
        for (int i = 0; i < graph->vertices; ++i) {
            if (graph->tasks[u]->dependencies[i]) {
                if (--graph->inDegree[i] == 0) {
                    queue[rear++] = i;
                }
            }
        }
    }

    // Libérer la mémoire
    free(queue);
}


// Calculer les heures de début les plus tôt
// void calculateEarliestStartTimes(struct Graph* graph, int* stack) {
//     for (int i = 0; i < graph->vertices; ++i) {
//         int u = stack[i];
//         graph->tasks[u]->start_time = 0;

//         // Calculer l'heure de début la plus tôt pour la tâche
//         for (int j = 0; j < graph->tasks[u]->num_dependencies; ++j) {
//             int dep_id = graph->tasks[u]->dependencies[j];
//             int dep_end_time = graph->tasks[dep_id]->end_time;
//             graph->tasks[u]->start_time = (dep_end_time > graph->tasks[u]->start_time) ? dep_end_time : graph->tasks[u]->start_time;
//         }
//     }
// }

void calculateEarliestStartTimes(struct Graph* graph, int* earliest_start_times) {
    for (int i = 0; i < graph->vertices; ++i) {
        earliest_start_times[i] = 0;
        // Rechercher d'autres tâches qui dépendent de la tâche en cours
        for (int j = 0; j < graph->vertices; ++j) {
            //Si la tâche j a des dépendances
            if (graph->tasks[j]->dependencies != NULL) {
                for (int k = 0; k < graph->tasks[j]->num_dependencies; ++k) {
                    if (graph->tasks[j]->dependencies[k] == i) {
                        int dep_end_time = graph->tasks[j]->end_time;// Récupère l'heure de fin de la tâche j
                        earliest_start_times[i] = (dep_end_time > earliest_start_times[i]) ? dep_end_time : earliest_start_times[i];
                    }
                }
            }
        }
    }
}


// Allocation dynamique des threads
void dynamicThreadAllocation(struct Graph* graph, int num_threads) {
    int* earliest_start_times = (int*)allocateMemory(graph->vertices * sizeof(int));

    calculateEarliestStartTimes(graph, earliest_start_times);

    // Allocation dynamique des threads
    pthread_t threads[num_threads];
    int current_thread = 0;

    // Créer un pool de threads
    for (int i = 0; i < graph->vertices; ++i) {
        int task_id = i;
        pthread_create(&threads[current_thread], NULL, executeTask, (void*)graph->tasks[task_id]);
        current_thread = (current_thread + 1) % num_threads;
    }

    // Attendre que tous les threads soient terminés
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Libérer la mémoire
    free(earliest_start_times);
}
