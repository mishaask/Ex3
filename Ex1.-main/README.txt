//mishaaskarov@gmail.com

Graph Algorithms Library
By: Misha Askarov
Email: mishaaskarov@gmail.com

***Overview***
This project implements a simple undirected, weighted graph along with several graph algorithms, including:

-Breadth-First Search (BFS)
-Depth-First Search (DFS)
-Dijkstra's Shortest Path Algorithm
-Prim's Minimum Spanning Tree Algorithm
-Kruskal's Minimum Spanning Tree Algorithm

Since the Standard Template Library (STL) is not allowed, custom implementations of Queue, PriorityQueue, and Union-Find structures are provided.

***Structure***

Graph.h / Graph.cpp:  Implements the Graph class and the Edge struct with add, remove, print, and utility functions.
Algorithms.h / Algorithms.cpp:  Implements the Algorithms class with BFS, DFS, Dijkstra, Prim, and Kruskal functions.
main.cpp:  Demonstrates usage of the Graph and Algorithms classes with example outputs.
test.cpp:  Contains unit tests for all functionality using the doctest framework.
Makefile:  Provides easy commands for compilation, running, testing, and memory leak checking via Valgrind.
doctest.h:  used for testing

***How to Build and Run***

Compiler: clang++

Compile and Run Main Demo: make Main

Run Unit Tests: make test

Check for Memory Leaks (Valgrind): make valgrind

Clean Build Files: make clean

***Testing***

The project uses doctest to create unit tests to verify:

-Adding and removing edges
-BFS and DFS correctness
-Shortest paths with Dijkstra
-Minimum Spanning Trees with Prim and Kruskal
-Various edge cases

