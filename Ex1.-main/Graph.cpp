//mishaaskarov@gmail.com

#include "Graph.h"

namespace graph {

Graph::Graph(int vertices) : numVertices(vertices) {//given numVertices is cont we have to use the member initializer list
    
    if (vertices <= 0)
        throw std::invalid_argument("Number of vertices must be positive");

    adjList = new Edge*[numVertices];
    for (int i = 0; i < numVertices; i++)
        adjList[i] = nullptr;
}

Graph::~Graph() {

    // Delete each Edge
    for (int i = 0; i < numVertices; i++) {
        Edge* current = adjList[i];
        while (current) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete [] adjList;
}

void Graph::addEdge(int src, int dest, int weight) {

    if(src < 0 || src >= numVertices || dest < 0 || dest >= numVertices || src == dest)
        throw std::out_of_range("Invalid vertex index");

    // Check if edge already exists from src to dest to prevent duplicates
    Edge* current = adjList[src];
    while (current != nullptr) {
        if (current->dest == dest) {
            throw std::runtime_error("Edge already exists");
        }
        current = current->next;
    }

    // Add edge from src to dest
    Edge* newEdge = new Edge{src, dest, weight, adjList[src]};
    adjList[src] = newEdge;

    // Since the graph is undirected, add edge from dest to src as well
    newEdge = new Edge{dest, src, weight, adjList[dest]};
    adjList[dest] = newEdge;
}

void Graph::removeEdge(int src, int dest) {

    if(src < 0 || src >= numVertices || dest < 0 || dest >= numVertices || src == dest)
        throw std::out_of_range("Invalid vertex index");

    bool removed = false;
    // Remove edge from src to dest
    Edge** current = &adjList[src];
    while (*current) {
        if ((*current)->dest == dest) {
            Edge* temp = *current;
            *current = (*current)->next;
            delete temp;
            removed = true;
            break;
        }
        current = &((*current)->next);
    }
    if (!removed)
        throw std::runtime_error("Edge does not exist in graph");

    // Remove edge from dest to src (for undirected graph)
    removed = false;
    current = &adjList[dest];
    while (*current) {
        if ((*current)->dest == src) {
            Edge* temp = *current;
            *current = (*current)->next;
            delete temp;
            removed = true;
            break;
        }
        current = &((*current)->next);
    }
    if (!removed)
        throw std::runtime_error("Error: In an undirected graph (v,u) edge was present while(u,v) wasn't.");
}

void Graph::printGraph() const {
    
    for (int i = 0; i < numVertices; i++) {
        std::cout << i << ": ";
        Edge* current = adjList[i];
        while (current) {
            std::cout << "(" << current->src << ", " << current->dest << ", " << current->weight << ") ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}
// const getters
int Graph::getNumVertices() const {
    return numVertices;
}

Edge* Graph::getAdjList(int vertex) const {
    if (vertex < 0 || vertex >= numVertices)
        throw std::out_of_range("Invalid vertex index");
    return adjList[vertex];
}

} // namespace graph
