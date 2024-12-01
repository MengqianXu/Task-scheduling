#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Graph.h"
#include "Task.h"


struct Graph* graph;

// Fonction pour créer une tâche
struct Task* createTask(int id, int duration, int* dependencies, int num_dependencies) {
    struct Task* task = (struct Task*)allocateMemory(sizeof(struct Task));
    task->id = id;
    task->duration = duration;
    task->dependencies = dependencies;
    task->num_dependencies = num_dependencies;
    task->start_time = 0;
    task->end_time = 0;
    task->num_dependencies_left = num_dependencies;
    pthread_mutex_init(&task->lock, NULL);
    pthread_cond_init(&task->cond, NULL);

    return task;
}

// Fonction pour ajouter une tâche
void addTask(struct Graph* graph, int id, int duration, int* dependencies, int num_dependencies) {
    struct Task* task = createTask(id, duration, dependencies, num_dependencies);
    graph->tasks[id] = task;

    // Mettre à jour le tableau des degrés entrants
    for (int i = 0; i < num_dependencies; ++i) {
        int dep_id = dependencies[i];
        graph->inDegree[dep_id]++;
    }
}

// Fonction pour exécuter une tâche
void* executeTask(void* arg) {
    struct Task* task = (struct Task*)arg;

    // Obtenir le verrou avant d'exécuter la tâche
    pthread_mutex_lock(&task->lock);

    // Attendre que toutes les tâches dépendantes soient terminées
    while (task->num_dependencies_left > 0) {
        pthread_cond_wait(&task->cond, &task->lock);
    }

    // Calculer l'heure de début de la tâche
    for (int i = 0; i < task->num_dependencies; ++i) {
        int dep_id = task->dependencies[i];
        int dep_end_time = graph->tasks[dep_id]->end_time;
        task->start_time = (dep_end_time > task->start_time) ? dep_end_time : task->start_time;
    }

    // Simuler l'exécution de la tâche
    printf("Exécution de la tâche %d, durée %d, heure de début %d\n", task->id, task->duration, task->start_time);
    usleep(task->duration * 1000);

    // Enregistrer l'heure de fin
    task->end_time = task->start_time + task->duration;

    // Avertir toutes les tâches qui dépendent de cette tâche
    for (int i = 0; i < graph->vertices; ++i) {
        struct Task* t = graph->tasks[i];
        for (int j = 0; j < t->num_dependencies; ++j) {
            if (t->dependencies[j] == task->id) {
                pthread_mutex_lock(&t->lock);
                --t->num_dependencies_left;
                pthread_cond_signal(&t->cond);
                pthread_mutex_unlock(&t->lock);
            }
        }
    }
    

    // Libérer le verrou après l'exécution de la tâche
    pthread_mutex_unlock(&task->lock);

    pthread_exit(NULL);
}
