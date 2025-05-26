//mishaaskarov@gmail.com

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"

namespace graph {

class Algorithms {
public:

    // Returns a tree resulting from running BFS starting at source.
    static Graph bfs(const Graph& graph, int src);

    // Returns a tree resulting from running DFS starting at source.
    static Graph dfs(const Graph& graph, int src);

    // Returns a weighted tree representing the shortest paths starting at source.
    static Graph dijkstra(const Graph& graph, int src);

    // Returns the minimum spanning tree using Prim's algorithm.
    static Graph prim(const Graph& graph);

    // Returns the minimum spanning tree using Kruskal's algorithm.
    static Graph kruskal(const Graph& graph);
};

} 

#endif
