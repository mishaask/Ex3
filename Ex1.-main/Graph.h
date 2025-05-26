//mishaaskarov@gmail.com

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <stdexcept>

namespace graph {

const int DEF_WEIGHT = 1;

//Because we dont have actions on indevidual vertices we dont need a Node struct
//and just reffere to them as int 1-n (n is the num of vertices)


// Edge structure including source, destination vertices, weight of the edge and the next Edge in the adjList
struct Edge {
    int src;
    int dest;
    int weight;
    Edge* next;
};

class Graph {
private:
    const int numVertices; // numVertices cannot be changed.
    Edge** adjList; // Array of pointers to linked lists for each vertex

public:
    // Constructor
    Graph(int numVertices);

    // Destructor
    ~Graph();

    // addEdge: Adds an undirected edge between src and dest with a given weight or DEF_WEIGHT.
    void addEdge(int src, int dest, int weight = DEF_WEIGHT);

    // removeEdge: Removes the edge between src->dest and dest->src.
    void removeEdge(int src, int dest);

    // printGraph: Prints the graph "vertex: (source,dest,weight)"
    void printGraph() const;
    
    // Returns the number of vertices.
    int getNumVertices() const;

    // Returns the head of the adjacency list for a given vertex.
    Edge* getAdjList(int vertex) const;
};

}

#endif
