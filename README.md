# Task Scheduling System

## Overview

This project implements a task scheduling system using a Directed Acyclic Graph (DAG) representation. Tasks have dependencies and durations, and they are executed respecting these dependencies and their earliest start times. The system utilizes multithreading to optimize task execution.

---

## Features

- **Directed Acyclic Graph Representation**:
  - Models task dependencies efficiently.
- **Topological Sorting**:
  - Orders tasks while respecting their dependencies.
- **Dynamic Thread Allocation**:
  - Enables multithreaded execution for optimized scheduling.
- **Earliest Start Time Calculation**:
  - Ensures tasks are executed as early as dependencies allow.

---

## Files

1. **`Graph.h`**:
   - Defines the structure for a directed graph.

2. **`Task.h`**:
   - Defines the structure for tasks and functions to create and execute them.

3. **`TopoSort_Dynamique.h`**:
   - Declares functions for topological sorting, calculating earliest start times, and dynamic thread allocation.

4. **`Graph.c`**:
   - Implements functions related to the graph structure, including initialization and memory management.

5. **`Task.c`**:
   - Implements functions for task creation, graph integration, and multithreaded execution.

6. **`TopoSort_Dynamique.c`**:
   - Implements algorithms for topological sorting, earliest start time calculation, and dynamic thread management.

7. **`Main.c`**:
   - Demonstrates the complete scheduling system by initializing tasks, performing topological sorting, allocating threads, executing tasks, and displaying results.

---

## How to Use

1. **Compile the Code**:
   Use a C compiler such as `gcc`:
   ```bash
   gcc Main.c -o task_scheduler
   ```

2. **Run the Executable**:
   ```bash
   ./task_scheduler
   ```

3. **Customize Parameters**:
   Adjust the parameters in the `main` function of `Main.c` to configure the number of tasks, threads, and other properties.

---

## Dependencies

- **Pthread Library**:
  - This program utilizes the `pthread` library for multithreading.

