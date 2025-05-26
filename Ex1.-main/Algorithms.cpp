//mishaaskarov@gmail.com


#include "Algorithms.h"
#include <iostream>
#include <stdexcept>
#include <limits>  // For INF


namespace graph {

// ***Data Structures***

// --- Simple Queue for integers (BFS) ---

class Queue {

private:
    int* arr;
    int capacity;
    int first_element, last_element, count;

public:
    Queue(int size): capacity(size), first_element(0), last_element(-1), count(0) {
        arr = new int[capacity];
    }

    ~Queue() { delete [] arr; }

    // Checks if the queue is empty
    bool isEmpty() const { return count == 0; }

    // Adds an item to the back of the queue
    void inqueue(int item) {
        if (count == capacity)
            throw std::runtime_error("Queue full");
        last_element = (last_element + 1) % capacity;//Allows to loop back to arr[0]
        arr[last_element] = item;
        count++;
    }

    // Removes and returns the front item of the queue
    int dequeue() {
        if (isEmpty())
            throw std::runtime_error("Queue empty");
        int item = arr[first_element];
        first_element = (first_element + 1) % capacity;//Allows to loop back to arr[0]
        count--;
        return item;
    }
};

// --- Priority Queue for (vertex, distance) pairs (Dijkstra and Prim) ---

struct PQNode {
    int vertex;
    int key;  // For Dijkstra this is the distance; for Prim, the minimum edge weight.
};

class PriorityQueue {

private:
    PQNode* heap;
    int capacity; // Maximum capacity of the heap
    int size; // Current number of elements

    // Restores heap property by moving a node up if its key is smaller than its parent's
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].key <= heap[index].key)
                break;
            PQNode temp = heap[parent];
            heap[parent] = heap[index];
            heap[index] = temp;
            index = parent;
        }
    }

    // Restores heap property by moving a node down if its key is greater than its children's
    void heapifyDown(int index) {
        while (2 * index + 1 < size) {
            int left = 2 * index + 1;
            int right = left + 1;
            int smallest = index;
            if (left < size && heap[left].key < heap[smallest].key)
                smallest = left;
            if (right < size && heap[right].key < heap[smallest].key)
                smallest = right;
            if (smallest == index)
                break;
            PQNode temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            index = smallest;
        }
    }

public:
    PriorityQueue(int cap): capacity(cap), size(0) {
        heap = new PQNode[capacity];
    }

    ~PriorityQueue() { delete [] heap; }

    bool isEmpty() const { return size == 0; }

    //Insert a new pair
    void push(int vertex, int key) {
        if (size == capacity)
            throw std::runtime_error("PriorityQueue full");
        heap[size] = {vertex, key};
        heapifyUp(size);
        size++;
    }

    //Return the node with the minimum key
    PQNode pop() {
        if (isEmpty())
            throw std::runtime_error("PriorityQueue empty");
        PQNode top = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        return top;
    }
};

// --- Union-Find structure (for Kruskal) ---

class UnionFind {

private:
    int* parent;
    int* rank;

public:

    UnionFind(int n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i; // Each node is its own parent
            rank[i] = 0;   // Initially, all trees have rank 0
        }
    }

    ~UnionFind() {
        delete [] parent;
        delete [] rank;
    }

    // Find the root of the set that x belongs to
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    // Merge (union) the sets containing x and y
    void merge(int x, int y) {
        int xroot = find(x);
        int yroot = find(y);
        if (xroot == yroot)
            return;
        if (rank[xroot] < rank[yroot])
            parent[xroot] = yroot;
        else if (rank[xroot] > rank[yroot])
            parent[yroot] = xroot;
        else {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }
};


// ***Algorithms Implementations***

// --- BFS ---

Graph Algorithms::bfs(const Graph& graph, int src) {

    int V = graph.getNumVertices();
    if (src < 0 || src >= V)
        throw std::out_of_range("Invalid source for BFS");
    
    Graph tree(V);  // Tree with BFS edges
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++) 
        visited[i] = false;

    Queue q(V);
    visited[src] = true;
    q.inqueue(src);

    while (!q.isEmpty()) {
        int u = q.dequeue();
        //Go over all neighbors of u
        for (Edge* neighborEdge = graph.getAdjList(u); neighborEdge != nullptr; neighborEdge = neighborEdge->next) {
            int v = neighborEdge->dest;
            if (!visited[v]) {
                visited[v] = true;
                // Add edge to the BFS tree
                tree.addEdge(u, v, neighborEdge->weight);
                q.inqueue(v);
            }
        }
    }
    delete [] visited;
    return tree;
}

// --- DFS ---
// Helper recursive function for DFS.

void dfsHelper(const Graph& g, int current, bool* visited, Graph& tree) {

    visited[current] = true;
    // Iterate over all neighbors of current vertex.
    for (Edge* neighborEdge = g.getAdjList(current); neighborEdge != nullptr; neighborEdge = neighborEdge->next) {
        int v = neighborEdge->dest;
        if (!visited[v]) {
            // Add edge to the DFS tree.
            tree.addEdge(current, v, neighborEdge->weight);
            dfsHelper(g, v, visited, tree);
        }
    }
}

Graph Algorithms::dfs(const Graph& graph, int src) {
    int V = graph.getNumVertices();
    if (src < 0 || src >= V)
        throw std::out_of_range("Invalid source vertex for DFS");
    
    Graph tree(V);
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++) 
    visited[i] = false;
    
    dfsHelper(graph, src, visited, tree);
    
    delete [] visited;
    return tree;
}

// --- Dijkstra ---
// Returns a weighted tree of shortest paths from source.

Graph Algorithms::dijkstra(const Graph& graph, int src) {
    int V = graph.getNumVertices();
    if (src < 0 || src >= V)
        throw std::out_of_range("Invalid source vertex for Dijkstra");
    
    Graph tree(V);
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[V];
    int* pred = new int[V];      // To store predecessor
    int* predWeight = new int[V]; // To store edge weight from predecessor
    bool* finalized = new bool[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        pred[i] = -1;
        predWeight[i] = 0;
        finalized[i] = false;
    }
    dist[src] = 0;

    PriorityQueue pq(V * 2);
    pq.push(src, 0);

    while (!pq.isEmpty()) {
        PQNode node = pq.pop();
        int u = node.vertex;
        if (finalized[u])
            continue;
        finalized[u] = true;

        //Go over neighbors of u.
        for (Edge* neighborEdge = graph.getAdjList(u); neighborEdge != nullptr; neighborEdge = neighborEdge->next) {
            int v = neighborEdge->dest;
            int weight = neighborEdge->weight;
            if (!finalized[v] && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pred[v] = u;
                predWeight[v] = weight;
                pq.push(v, dist[v]);
            }
        }
    }
    
    // Build the shortest path tree from predecessor info.
    for (int v = 0; v < V; v++) {
        if (v != src && pred[v] != -1)
            tree.addEdge(pred[v], v, predWeight[v]);
    }

    delete [] dist;
    delete [] pred;
    delete [] predWeight;
    delete [] finalized;
    return tree;
}

// --- Prim ---
// Computes a minimum spanning tree (MST) using Prim's algorithm.

Graph Algorithms::prim(const Graph& graph) {

    int V = graph.getNumVertices();
    Graph tree(V);
    const int INF = std::numeric_limits<int>::max();
    int* key = new int[V];    // Best edge weight to reach vertex
    int* parent = new int[V]; // Parent of each vertex in MST
    bool* included = new bool[V]; // Whether the vertex is already included

    for (int i = 0; i < V; i++) {
        key[i] = INF;
        parent[i] = -1;
        included[i] = false;
    }
    // Given we dont take a source vertex(as instructed) we start from vertex 0
    key[0] = 0;
    PriorityQueue pq(V * 2);
    pq.push(0, 0);

    while (!pq.isEmpty()) {
        PQNode node = pq.pop();
        int u = node.vertex;
        if (included[u])
            continue;
        included[u] = true;

        // Go over neighbors of u.
        for (Edge* neighborEdge = graph.getAdjList(u); neighborEdge != nullptr; neighborEdge = neighborEdge->next) {
            int v = neighborEdge->dest;
            int weight = neighborEdge->weight;
            if (!included[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push(v, key[v]);
            }
        }
    }

    // Build the MST tree from parent information.
    for (int v = 1; v < V; v++) { // Start from 1 since 0 has no parent
        if (parent[v] != -1)
            tree.addEdge(parent[v], v, key[v]);
    }

    delete [] key;
    delete [] parent;
    delete [] included;
    return tree;
}

// --- Kruskal ---
// Computes the MST using Kruskal's algorithm.

Graph Algorithms::kruskal(const Graph& graph) {
    
    int V = graph.getNumVertices();
    Graph tree(V);
    
    // First we gather all unique edges. To avoid duplicates (undirected graph),
    // we only take an edge if src < dest.
    int edgeCount = 0;
    for (int i = 0; i < V; i++) {
        for (Edge* e = graph.getAdjList(i); e != nullptr; e = e->next) {
            if (i < e->dest)
                edgeCount++;
        }
    }

    Edge* edges = new Edge[edgeCount];
    int u = 0;
    for (int i = 0; i < V; i++) {
        for (Edge* e = graph.getAdjList(i); e != nullptr; e = e->next) {
            if (i < e->dest) {
                edges[u].src = i;
                edges[u].dest = e->dest;
                edges[u].weight = e->weight;
                u++;
            }
        }
    }
    
    // Sort edges by weight using Selection sort
    for (int i = 0; i < edgeCount - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < edgeCount; j++) {
            if (edges[j].weight < edges[minIndex].weight)
                minIndex = j;
        }
        if (minIndex != i){
            Edge temp = edges[i];
            edges[i] = edges[minIndex];
            edges[minIndex] = temp;
        }
    }
    
    // Initialize Union-Find structure.
    UnionFind uf(V);
    
    // Iterate over sorted edges and add them to the MST if they don't form a cycle.
    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i].src;
        int v = edges[i].dest;
        if (uf.find(u) != uf.find(v)) {
            uf.merge(u, v);
            tree.addEdge(u, v, edges[i].weight);
        }
    }
    
    delete [] edges;
    return tree;
}

} // namespace graph
