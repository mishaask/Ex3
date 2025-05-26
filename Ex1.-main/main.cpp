//mishaaskarov@gmail.com

#include "Graph.h"
#include "Algorithms.h"
#include <iostream>

int main() {
    using namespace graph;
    try {
        // Create a graph with 6 vertices (vertex 5 has no edges)
        Graph g(6);
        g.addEdge(0, 1, 2);
        g.addEdge(0, 2, 4);
        g.addEdge(1, 2, 1);
        g.addEdge(1, 3, 7);
        g.addEdge(2, 4, 3);
        g.addEdge(3, 4, 1);

        std::cout << "Original Graph:" << '\n' << "vertex: (source,dest,weight)" << std::endl;
        g.printGraph();

        // Demonstrate removeEdge
        std::cout << "\nRemoving edge between 1 and 3" << std::endl;
        g.removeEdge(1, 3);
        g.printGraph();

        // Demonstrate BFS traversal
        Graph bfsTree = Algorithms::bfs(g, 0);
        std::cout << "\nBFS Tree (starting vertex is 0):" << std::endl;
        bfsTree.printGraph();

        // Demonstrate DFS traversal
        Graph dfsTree = Algorithms::dfs(g, 0);
        std::cout << "\nDFS Tree (starting vertex is 0):" << std::endl;
        dfsTree.printGraph();

        // Demonstrate Dijkstra (shortest path tree from vertex 0)
        Graph dijkstraTree = Algorithms::dijkstra(g, 0);
        std::cout << "\nDijkstra Shortest Path Tree from vertex 0:" << std::endl;
        dijkstraTree.printGraph();

        // Demonstrate Prim's MST
        Graph primTree = Algorithms::prim(g);
        std::cout << "\nPrim's Minimum Spanning Tree:" << std::endl;
        primTree.printGraph();

        // Demonstrate Kruskal's MST
        Graph kruskalTree = Algorithms::kruskal(g);
        std::cout << "\nKruskal's Minimum Spanning Tree:" << std::endl;
        kruskalTree.printGraph();

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
    return 0;
}