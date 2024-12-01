#ifndef TASK_H
#define TASK_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


struct Task {
    int id;
    int duration; //le temps nécessaire à l’exécution.
    int* dependencies;//stocke les identifiants des autres tâches dont dépend la tâche.
    int num_dependencies;
    int start_time;
    int end_time;
    pthread_mutex_t lock;//verrous mutex 
    pthread_cond_t cond;//variables de condition utilisés pour la synchronisation des threads.
    int num_dependencies_left;//Utilisé pour suivre le nombre de dépendances qui ne sont pas encore terminées
};

struct Task* createTask(int id, int duration, int* dependencies, int num_dependencies);
void addTask(struct Graph* graph, int id, int duration, int* dependencies, int num_dependencies);
void* executeTask(void* arg);


#endif 
