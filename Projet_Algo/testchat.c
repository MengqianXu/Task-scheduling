#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NOT_VISITED 0
#define IN_PROGRESS 1
#define VISITED 2

// 任务结构体
struct Task {
    int id;
    int duration;
    int* dependencies;
    int num_dependencies;
    int start_time;
    pthread_mutex_t lock;
};

// 图结构体
struct Graph {
    int vertices;
    struct Task** tasks;
    int** adjacencyMatrix;
};

// 内存分配函数
void* allocateMemory(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        printf("内存分配失败。\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// 初始化图
struct Graph* initializeGraph(int vertices) {
    struct Graph* graph = (struct Graph*)allocateMemory(sizeof(struct Graph));
    graph->vertices = vertices;

    // 分配任务数组和邻接矩阵的内存
    graph->tasks = (struct Task**)allocateMemory(vertices * sizeof(struct Task*));
    graph->adjacencyMatrix = (int**)allocateMemory(vertices * sizeof(int*));

    // 初始化邻接矩阵
    for (int i = 0; i < vertices; ++i) {
        graph->adjacencyMatrix[i] = (int*)allocateMemory(vertices * sizeof(int));
        for (int j = 0; j < vertices; ++j) {
            graph->adjacencyMatrix[i][j] = 0;  // 初始化为0表示没有边
        }
    }

    // 初始化任务数组
    for (int i = 0; i < vertices; ++i) {
        graph->tasks[i] = NULL;  // 将任务数组初始化为NULL
    }

    return graph;
}

// 任务创建函数
struct Task* createTask(int id, int duration, int* dependencies, int num_dependencies, int** adjacencyMatrix) {
    struct Task* task = (struct Task*)allocateMemory(sizeof(struct Task));
    task->id = id;
    task->duration = duration;
    task->dependencies = dependencies;
    task->num_dependencies = num_dependencies;
    task->start_time = 0;              // 将开始时间初始化为0
    pthread_mutex_init(&task->lock, NULL);  // 初始化互斥锁
    task->dependencies = adjacencyMatrix[id];  // 指向邻接矩阵的一行

    return task;
}

// 添加任务
void addTask(struct Graph* graph, int id, int duration, int* dependencies, int num_dependencies) {
    struct Task* task = createTask(id, duration, dependencies, num_dependencies, graph->adjacencyMatrix);
    graph->tasks[id] = task;

    // 更新邻接矩阵，表示任务之间的关系
    for (int i = 0; i < num_dependencies; ++i) {
        int dep_id = dependencies[i];
        graph->adjacencyMatrix[dep_id][id] = 1;  // 表示有边
    }
}

// 释放图的内存
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->tasks[i]->dependencies);
        free(graph->tasks[i]);
        free(graph->adjacencyMatrix[i]);
    }
    free(graph->tasks);
    free(graph->adjacencyMatrix);
    free(graph);
}

// 打印邻接矩阵
void printGraph(struct Graph* graph) {
    printf("邻接矩阵：\n");
    for (int i = 0; i < graph->vertices; ++i) {
        for (int j = 0; j < graph->vertices; ++j) {
            printf("%d ", graph->adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
}

// 线程函数
void* executeTask(void* arg) {
    struct Task* task = (struct Task*)arg;

    // 在执行任务之前获取互斥锁
    pthread_mutex_lock(&task->lock);

    // 根据依赖关系计算任务开始时间
    for (int i = 0; i < task->num_dependencies; ++i) {
        int dep_id = task->dependencies[i];
        int dep_end_time = graph->tasks[dep_id]->start_time + graph->tasks[dep_id]->duration;
        task->start_time = (dep_end_time > task->start_time) ? dep_end_time : task->start_time;
    }

    // 模拟任务执行
    printf("执行任务 %d，持续时间 %d，开始时间 %d\n", task->id, task->duration, task->start_time);
    // 模拟任务执行时间
    usleep(task->duration * 1000);

    // 任务执行完成后释放互斥锁
    pthread_mutex_unlock(&task->lock);

    pthread_exit(NULL);
}

int main() {
    const int num_tasks = 5;

    struct Graph* graph = initializeGraph(num_tasks);

    // 添加任务
    int dependencies_task1[] = {};       // Task 1 has no dependencies
    int dependencies_task2[] = {0};      // Task 2 depends on Task 1
    int dependencies_task3[] = {0};      // Task 3 depends on Task 1
    int dependencies_task4[] = {1, 2};   // Task 4 depends on Task 2 and Task 3
    int dependencies_task5[] = {3};      // Task 5 depends on Task 4

    addTask(graph, 0, 300, dependencies_task1, sizeof(dependencies_task1) / sizeof(int));
    addTask(graph, 1, 500, dependencies_task2, sizeof(dependencies_task2) / sizeof(int));
    addTask(graph, 2, 400, dependencies_task3, sizeof(dependencies_task3) / sizeof(int));
    addTask(graph, 3, 600, dependencies_task4, sizeof(dependencies_task4) / sizeof(int));
    addTask(graph, 4, 200, dependencies_task5, sizeof(dependencies_task5) / sizeof(int));

    printGraph(graph);

    // 创建线程
    pthread_t threads[num_tasks];

    // 并行执行任务
    for (int i = 0; i < num_tasks; ++i) {
        pthread_create(&threads[i], NULL, executeTask, (void*)graph->tasks[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < num_tasks; ++i) {
        pthread_join(threads[i], NULL);
    }

    // 释放图的内存
    freeGraph(graph);

    return 0;
}
