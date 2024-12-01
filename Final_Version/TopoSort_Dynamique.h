#ifndef TOPOSORT_DYNAMIQUE_H
#define TOPOSORT_DYNAMIQUE_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void topologicalSort(struct Graph* graph, int* stack);
void calculateEarliestStartTimes(struct Graph* graph, int* stack);
void dynamicThreadAllocation(struct Graph* graph, int num_threads);

#endif
