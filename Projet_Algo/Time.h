#ifndef TIME_H
#define TIME_H


int calculateCompletionTime(struct Graph* graph, int v, int* visited, struct Task* tasks, int* completionTimes);
int calculateTotalCompletionTime(struct Graph* graph, struct Task* tasks);

#endif
