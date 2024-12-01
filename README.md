\documentclass[a4paper,12pt]{article}
\usepackage[utf8]{inputenc}
\usepackage{geometry}
\geometry{a4paper, margin=1in}
\usepackage{hyperref}

\title{Task Scheduling Project}
\author{}
\date{}

\begin{document}

\maketitle

\section*{Overview}
This project implements a task scheduling system using a Directed Acyclic Graph (DAG) representation. Tasks have dependencies, durations, and are executed based on their dependencies and earliest start times. The system employs multithreading for task execution.

\section*{Files}
\begin{enumerate}
    \item \textbf{Graph.h}:
    Defines the structure for a directed graph.
    
    \item \textbf{Task.h}:
    Defines the structure for a task and functions to create and execute tasks.
    
    \item \textbf{TopoSort\_Dynamique.h}:
    Defines functions for topological sorting, calculating earliest start times, and dynamic thread allocation.
    
    \item \textbf{Graph.c}:
    Implementation of functions related to the graph structure. Includes functions for initializing and freeing graph memory.
    
    \item \textbf{Task.c}:
    Implementation of functions related to task creation and execution. Functions for creating tasks, adding tasks to the graph, and executing tasks in a multithreaded environment.
    
    \item \textbf{TopoSort\_Dynamique.c}:
    Implementation of functions for topological sorting, earliest start time calculation, and dynamic thread allocation.
    
    \item \textbf{Main.c}:
    The main program that initializes tasks, performs topological sorting, calculates earliest start times, dynamically allocates threads, executes tasks, and prints the results. It demonstrates the usage of the implemented system.
\end{enumerate}

\section*{How to Use}
\begin{enumerate}
    \item Compile the code using a C compiler, for example:
    \begin{verbatim}
    gcc Main.c -o task_scheduler
    \end{verbatim}
    
    \item Run the executable:
    \begin{verbatim}
    ./task_scheduler
    \end{verbatim}
    
    \item Adjust the parameters in the \texttt{main()} function of \texttt{Main.c} to customize the number of tasks, threads, and other properties.
\end{enumerate}

\section*{Dependencies}
This program uses the \texttt{pthread} library for multithreading.

\end{document}
