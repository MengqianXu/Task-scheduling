#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>  // 需要包含这个头文件来使用time函数
#include <string.h>

// 任务结构体
struct Task {
    int id;
    int duration;
    int* dependencies;
    int num_dependencies;
    int start_time;
    int end_time;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int num_dependencies_left;
};

// 图结构体
struct Graph {
    int vertices;
    struct Task** tasks;
};

// 全局图变量
struct Graph* graph;

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

    // 分配任务数组的内存
    graph->tasks = (struct Task**)allocateMemory(vertices * sizeof(struct Task*));

    // 初始化任务数组
    for (int i = 0; i < vertices; ++i) {
        graph->tasks[i] = NULL;  // 将任务数组初始化为NULL
    }

    return graph;
}

// 任务创建函数
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

// 添加任务
void addTask(struct Graph* graph, int id, int duration, int* dependencies, int num_dependencies) {
    struct Task* task = createTask(id, duration, dependencies, num_dependencies);
    graph->tasks[id] = task;
}

// 释放图的内存
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->tasks[i]->dependencies);
        free(graph->tasks[i]);
    }
    free(graph->tasks);
    free(graph);
}

// 线程函数
void* executeTask(void* arg) {
    struct Task* task = (struct Task*)arg;

    // 在执行任务之前获取互斥锁
    pthread_mutex_lock(&task->lock);

    // 等待所有依赖的任务完成
    while (task->num_dependencies_left > 0) {
        pthread_cond_wait(&task->cond, &task->lock);
    }

    // 计算任务开始时间
    for (int i = 0; i < task->num_dependencies; ++i) {
        int dep_id = task->dependencies[i];
        int dep_end_time = graph->tasks[dep_id]->end_time;
        task->start_time = (dep_end_time > task->start_time) ? dep_end_time : task->start_time;
    }

    // 模拟任务执行
    printf("执行任务 %d, 持续时间 %d, 开始时间 %d\n", task->id, task->duration, task->start_time);
    usleep(task->duration * 1000);

    // 记录结束时间
    task->end_time = task->start_time + task->duration;

    // 通知所有依赖于此任务的任务
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

    // 任务执行完成后释放互斥锁
    pthread_mutex_unlock(&task->lock);

    pthread_exit(NULL);
}

// 并行执行任务
void parallelExecution(struct Graph* graph) {
    pthread_t threads[graph->vertices];

    // 创建一个线程池
    for (int i = 0; i < graph->vertices; ++i) {
        pthread_create(&threads[i], NULL, executeTask, (void*)graph->tasks[i]);
    }

    // 等待所有的线程完成
    for (int i = 0; i < graph->vertices; ++i) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    const int num_tasks = 5;

    graph = initializeGraph(num_tasks);

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

    // 并行执行任务
    parallelExecution(graph);

    // 打印任务的执行顺序和总时间
    int total_time = 0;
    for (int i = 0; i < num_tasks; ++i) {
        printf("任务 %d 的开始时间：%d, 结束时间:%d\n", graph->tasks[i]->id, graph->tasks[i]->start_time, graph->tasks[i]->end_time);
        if (graph->tasks[i]->end_time > total_time) {
            total_time = graph->tasks[i]->end_time;
        }
    }

    printf("总时间：%d\n", total_time);

    // 释放图的内存
    freeGraph(graph);

    return 0;
}







//含拓扑排序和动态规划版
// 任务结构体
struct Task {
    int id;
    int duration;
    int* dependencies;
    int num_dependencies;
    int start_time;
    int end_time;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int num_dependencies_left;
};

// 图结构体
struct Graph {
    int vertices;
    struct Task** tasks;
    int* inDegree; // 入度数组
};

// 全局图变量
struct Graph* graph;

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

    // 分配任务数组和入度数组的内存
    graph->tasks = (struct Task**)allocateMemory(vertices * sizeof(struct Task*));
    graph->inDegree = (int*)allocateMemory(vertices * sizeof(int));

    // 初始化任务数组和入度数组
    for (int i = 0; i < vertices; ++i) {
        graph->tasks[i] = NULL;  // 将任务数组初始化为NULL
        graph->inDegree[i] = 0;  // 将入度数组初始化为0
    }

    return graph;
}

// 任务创建函数
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

// 添加任务
void addTask(struct Graph* graph, int id, int duration, int* dependencies, int num_dependencies) {
    struct Task* task = createTask(id, duration, dependencies, num_dependencies);
    graph->tasks[id] = task;

    // 更新入度数组
    for (int i = 0; i < num_dependencies; ++i) {
        int dep_id = dependencies[i];
        graph->inDegree[dep_id]++;
    }
}

// 释放图的内存
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->tasks[i]->dependencies);
        free(graph->tasks[i]);
    }
    free(graph->tasks);
    free(graph->inDegree);
    free(graph);
}

// 线程函数
void* executeTask(void* arg) {
    struct Task* task = (struct Task*)arg;

    // 在执行任务之前获取互斥锁
    pthread_mutex_lock(&task->lock);

    // 等待所有依赖的任务完成
    while (task->num_dependencies_left > 0) {
        pthread_cond_wait(&task->cond, &task->lock);
    }

    // 计算任务开始时间
    for (int i = 0; i < task->num_dependencies; ++i) {
        int dep_id = task->dependencies[i];
        int dep_end_time = graph->tasks[dep_id]->end_time;
        task->start_time = (dep_end_time > task->start_time) ? dep_end_time : task->start_time;
    }

    // 模拟任务执行
    printf("执行任务 %d, 持续时间 %d, 开始时间 %d\n", task->id, task->duration, task->start_time);
    usleep(task->duration * 1000);

    // 记录结束时间
    task->end_time = task->start_time + task->duration;

    // 通知所有依赖于此任务的任务
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

    // 任务执行完成后释放互斥锁
    pthread_mutex_unlock(&task->lock);

    pthread_exit(NULL);
}

// 并行执行任务
void parallelExecution(struct Graph* graph) {
    pthread_t threads[graph->vertices];

    // 创建一个线程池
    for (int i = 0; i < graph->vertices; ++i) {
        pthread_create(&threads[i], NULL, executeTask, (void*)graph->tasks[i]);
    }

    // 等待所有的线程完成
    for (int i = 0; i < graph->vertices; ++i) {
        pthread_join(threads[i], NULL);
    }
}






//运行100个任务的main函数
int main() {
    const int num_tasks = 100;

    // 初始化随机数生成器
    srand(time(NULL));

    graph = initializeGraph(num_tasks);

    // 添加任务
    for (int i = 0; i < num_tasks; ++i) {
        int duration = 300;
        int dependencies[num_tasks];
        int num_dependencies = 0;

        for (int j = 0; j < i; ++j) {
            // 对于每个先前的任务，有50%的概率将其添加为依赖关系
            if (rand() % 2) {
                dependencies[num_dependencies] = j;
                num_dependencies++;
            }
        }

        // 使用动态分配的数组来存储依赖关系
        int* dependencies_copy = (int*)allocateMemory(num_dependencies * sizeof(int));
        memcpy(dependencies_copy, dependencies, num_dependencies * sizeof(int));

        addTask(graph, i, duration, dependencies_copy, num_dependencies);
    }

    // 并行执行任务
    parallelExecution(graph);

    // 打印任务的执行顺序和总时间
    int total_time = 0;
    for (int i = 0; i < num_tasks; ++i) {
        printf("任务 %d 的开始时间：%d, 结束时间:%d\n", graph->tasks[i]->id, graph->tasks[i]->start_time, graph->tasks[i]->end_time);
        if (graph->tasks[i]->end_time > total_time) {
            total_time = graph->tasks[i]->end_time;
        }
    }

    printf("总时间：%d\n", total_time);

    // 释放图的内存
    freeGraph(graph);

    return 0;
}




///////////////
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 任务结构体
struct Task {
    int id;
    int duration;
    int* dependencies;
    int num_dependencies;
    int start_time;
    int end_time;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int num_dependencies_left;
};

// 图结构体
struct Graph {
    int vertices;
    struct Task** tasks;
    int* inDegree; // 入度数组
};

// 全局图变量
struct Graph* graph;

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

    // 分配任务数组和入度数组的内存
    graph->tasks = (struct Task**)allocateMemory(vertices * sizeof(struct Task*));
    graph->inDegree = (int*)allocateMemory(vertices * sizeof(int));

    // 初始化任务数组和入度数组
    for (int i = 0; i < vertices; ++i) {
        graph->tasks[i] = NULL;  // 将任务数组初始化为NULL
        graph->inDegree[i] = 0;  // 将入度数组初始化为0
    }

    return graph;
}

// 任务创建函数
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

// 添加任务
void addTask(struct Graph* graph, int id, int duration, int* dependencies, int num_dependencies) {
    struct Task* task = createTask(id, duration, dependencies, num_dependencies);
    graph->tasks[id] = task;

    // 更新入度数组
    for (int i = 0; i < num_dependencies; ++i) {
        int dep_id = dependencies[i];
        graph->inDegree[dep_id]++;
    }
}

// 释放图的内存
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->tasks[i]->dependencies);
        free(graph->tasks[i]);
    }
    free(graph->tasks);
    free(graph->inDegree);
    free(graph);
}

// 线程函数
void* executeTask(void* arg) {
    struct Task* task = (struct Task*)arg;

    // 在执行任务之前获取互斥锁
    pthread_mutex_lock(&task->lock);

    // 等待所有依赖的任务完成
    while (task->num_dependencies_left > 0) {
        pthread_cond_wait(&task->cond, &task->lock);
    }

    // 计算任务开始时间
    for (int i = 0; i < task->num_dependencies; ++i) {
        int dep_id = task->dependencies[i];
        int dep_end_time = graph->tasks[dep_id]->end_time;
        task->start_time = (dep_end_time > task->start_time) ? dep_end_time : task->start_time;
    }

    // 模拟任务执行
    printf("执行任务 %d, 持续时间 %d, 开始时间 %d\n", task->id, task->duration, task->start_time);
    usleep(task->duration * 1000);

    // 记录结束时间
    task->end_time = task->start_time + task->duration;

    // 通知所有依赖于此任务的任务
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

    // 任务执行完成后释放互斥锁
    pthread_mutex_unlock(&task->lock);

    pthread_exit(NULL);
}

// 拓扑排序
void topologicalSort(struct Graph* graph, int* stack) {
    int stackIndex = 0;

    // 创建一个队列用于拓扑排序
    int* queue = (int*)allocateMemory(graph->vertices * sizeof(int));
    int front = 0, rear = 0;

    // 将入度为0的节点入队
    for (int i = 0; i < graph->vertices; ++i) {
        if (graph->inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }

    while (front != rear) {
        int u = queue[front++];

        // 将当前节点添加到拓扑排序的结果中
        stack[stackIndex++] = u;

        // 更新邻接节点的入度，如果入度为0，则入队
        for (int i = 0; i < graph->vertices; ++i) {
            if (graph->tasks[u]->dependencies[i]) {
                if (--graph->inDegree[i] == 0) {
                    queue[rear++] = i;
                }
            }
        }
    }

    // 释放内存
    free(queue);
}

// 计算任务的最早开始时间
void calculateEarliestStartTimes(struct Graph* graph, int* stack) {
    for (int i = 0; i < graph->vertices; ++i) {
        int u = stack[i];
        graph->tasks[u]->start_time = 0;

        // 计算任务的最早开始时间
        for (int j = 0; j < graph->tasks[u]->num_dependencies; ++j) {
            int dep_id = graph->tasks[u]->dependencies[j];
            int dep_end_time = graph->tasks[dep_id]->end_time;
            graph->tasks[u]->start_time = (dep_end_time > graph->tasks[u]->start_time) ? dep_end_time : graph->tasks[u]->start_time;
        }
    }
}

// 动态规划分配线程
void dynamicThreadAllocation(struct Graph* graph, int num_threads) {
    int* earliest_start_times = (int*)allocateMemory(graph->vertices * sizeof(int));

    // 计算每个任务的最早开始时间
    for (int i = 0; i < graph->vertices; ++i) {
        int u = i;
        earliest_start_times[u] = 0;

        // 计算任务的最早开始时间
        for (int j = 0; j < graph->tasks[u]->num_dependencies; ++j) {
            int dep_id = graph->tasks[u]->dependencies[j];
            int dep_end_time = graph->tasks[dep_id]->end_time;
            earliest_start_times[u] = (dep_end_time > earliest_start_times[u]) ? dep_end_time : earliest_start_times[u];
        }
    }

    // 动态规划分配线程
    pthread_t threads[num_threads];
    int current_thread = 0;

    // 创建一个线程池
    for (int i = 0; i < graph->vertices; ++i) {
        int task_id = i;
        pthread_create(&threads[current_thread], NULL, executeTask, (void*)graph->tasks[task_id]);
        current_thread = (current_thread + 1) % num_threads;
    }

    // 等待所有的线程完成
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // 释放内存
    free(earliest_start_times);
}

// 主函数
int main() {
    const int num_tasks = 5;
    const int num_threads = 3;  // 设置线程数量

    graph = initializeGraph(num_tasks);

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

    // 拓扑排序
    int* stack = (int*)allocateMemory(num_tasks * sizeof(int));
    topologicalSort(graph, stack);

    // 计算任务的最早开始时间
    calculateEarliestStartTimes(graph, stack);

    // 动态规划分配线程
    dynamicThreadAllocation(graph, num_threads);

    // 打印任务的执行顺序和总时间
    int total_time = 0;
    for (int i = 0; i < num_tasks; ++i) {
        printf("任务 %d 的开始时间：%d, 结束时间:%d\n", graph->tasks[i]->id, graph->tasks[i]->start_time, graph->tasks[i]->end_time);
        if (graph->tasks[i]->end_time > total_time) {
            total_time = graph->tasks[i]->end_time;
        }
    }

    printf("总时间：%d\n", total_time);

    // 释放图的内存
    free(stack);
    freeGraph(graph);

    return 0;
}


