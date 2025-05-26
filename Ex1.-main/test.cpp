// mishaaskarov@gmail.com

// Provide doctest implementation and auto generating main.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Graph.h"
#include "Algorithms.h"
#include <sstream>
#include <iostream>

// A helper function to capture the printed graph output
std::string captureGraphOutput(const graph::Graph &g) {

    std::ostringstream outpStringStream;// String stream to capture output
    std::streambuf *oldCoutStreamBuf = std::cout.rdbuf();// Save original cout buffer
    std::cout.rdbuf(outpStringStream.rdbuf()); // Redirect cout to stringstream
    g.printGraph();
    std::cout.rdbuf(oldCoutStreamBuf);// Restore cout to original buffer
    return outpStringStream.str();
}

TEST_CASE("Graph: addEdge and printGraph") {

    using namespace graph;
    Graph g(3);
    //Check invalid vertex inputs
    CHECK_THROWS(g.addEdge(10, 1, 5));
    CHECK_THROWS(g.addEdge(-1, 1, 5));
    CHECK_THROWS(g.addEdge(0, 0, 5));
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 3);
    CHECK_THROWS(g.addEdge(1, 2, 5));//No duplicate edges
    CHECK_THROWS(g.addEdge(2, 1, 5));//No duplicate edges
    std::string output = captureGraphOutput(g);
    // Basic checks.
    CHECK(output.find("(0, 1, 5)") != std::string::npos);
    CHECK(output.find("(1, 0, 5)") != std::string::npos);
    CHECK(output.find("(1, 2, 3)") != std::string::npos);
    CHECK(output.find("(2, 1, 3)") != std::string::npos);
    CHECK(g.getNumVertices() == 3);
}

TEST_CASE("Graph: removeEdge throws if edge doesn't exist") {

    using namespace graph;
    Graph g(2);
    g.addEdge(0, 1, 10);
    // Removing the edge should succeed.
    CHECK_NOTHROW(g.removeEdge(0, 1));
    // Trying to remove it again should throw an exception.
    CHECK_THROWS(g.removeEdge(0, 1));
    CHECK_THROWS(g.removeEdge(-1, 1));
    CHECK_THROWS(g.removeEdge(5, 1));
    
}

TEST_CASE("BFS generates a valid tree") {

    using namespace graph;
    Graph g(6);
    g.addEdge(0, 1, 5);
    g.addEdge(0, 2, 5);
    g.addEdge(1, 3, 5);
    // Adding a disconnected graph
    g.addEdge(4, 5, 5);
    Graph bfsTree = Algorithms::bfs(g, 0);
    // For a simple graph, we expect vertex 0 to be connected to 1 and 2 and then 1 to connect to 3.
    std::string output = captureGraphOutput(bfsTree);
    CHECK(output.find("(0, 1, 5)") != std::string::npos);
    CHECK(output.find("(0, 2, 5)") != std::string::npos);
    CHECK(output.find("(1, 3, 5)") != std::string::npos);
    CHECK(output.find("(1, 0, 5)") != std::string::npos);//Given the graph is undirected the result should have both edges from v to u and from u to v
    CHECK(output.find("(4, 5, 5)") == std::string::npos);//Disconnected graph {V=(4,5),E=(4,5,5)} shouldn't be included in bfs that starts at vertex 0

    Graph bfsTree2 = Algorithms::bfs(g, 5);
    // For a simple graph, we expect vertex 0 to be connected to 1 and 2 and then 1 to connect to 3.
    output = captureGraphOutput(bfsTree2);
    CHECK(output.find("(0, 1, 5)") == std::string::npos);
    CHECK(output.find("(0, 2, 5)") == std::string::npos);
    CHECK(output.find("(1, 3, 5)") == std::string::npos);
    CHECK(output.find("(1, 0, 5)") == std::string::npos);
    CHECK(output.find("(4, 5, 5)") != std::string::npos);

}


TEST_CASE("DFS generates a valid tree") {

    using namespace graph;
    Graph g(6);
    // Create a graph with a cycle
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 3, 1);
    g.addEdge(2, 3, 1);
    // Adding a disconnected graph
    g.addEdge(4, 5, 5);
    
    Graph dfsTree = Algorithms::dfs(g, 0);
    std::string output = captureGraphOutput(dfsTree);
    // Verify that the DFS tree contains edges leading to all vertices.
    CHECK(output.find("(0, 2, 1)") != std::string::npos);
    CHECK(output.find("(2, 3, 1)") != std::string::npos);
    CHECK(output.find("(3, 1, 1)") != std::string::npos);

    //The dfs algo starts with the most recently added neighbor, given (0, 2, 1) was added after (0, 1, 1) 
    //The adjList[0] is adjList[0] -> (0,2,1) -> (0,1,1) -> nullptr
    //Which means we go through (0,2,1) ->(2, 3, 1) ->(3, 1, 1) and (1, 0, 1) should not be included in the final tree
    CHECK(output.find("(1, 0, 1)") == std::string::npos);
    CHECK(output.find("(4, 5, 5)") == std::string::npos);//Disconnected graph

    Graph dfsTree2 = Algorithms::dfs(g, 5);
    output = captureGraphOutput(dfsTree2);
    // Verify that the DFS tree contains edges leading to all vertices.
    CHECK(output.find("(0, 2, 1)") == std::string::npos);
    CHECK(output.find("(2, 3, 1)") == std::string::npos);
    CHECK(output.find("(3, 1, 1)") == std::string::npos);
    CHECK(output.find("(1, 0, 1)") == std::string::npos);
    CHECK(output.find("(4, 5, 5)") != std::string::npos);//Disconnected graph

}

TEST_CASE("Dijkstra returns shortest path tree with 5 vertices") {

    using namespace graph;
    Graph g(5);
    g.addEdge(0, 1, 6);
    g.addEdge(0, 2, 1);
    g.addEdge(2, 3, 2);
    g.addEdge(3, 1, 0);
    Graph dijkstraTree = Algorithms::dijkstra(g, 0);
    std::string output = captureGraphOutput(dijkstraTree);
    // We expect the shortest path from 0 to 1 is 0->2->3->1
    CHECK(output.find("(0, 2, 1)") != std::string::npos);
    CHECK(output.find("(2, 3, 2)") != std::string::npos);
    CHECK(output.find("(3, 1, 0)") != std::string::npos);
    CHECK(output.find("(0, 1, 7)") == std::string::npos);
    CHECK(output.find("4: (") == std::string::npos);//Vertex 4 has no edges
}

TEST_CASE("Prim generates minimum spanning tree") {

    using namespace graph;
    Graph g(6);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 1);
    g.addEdge(4, 5, 0);//Disconnected graph
    
    Graph primTree = Algorithms::prim(g);
    std::string output = captureGraphOutput(primTree);
    // The expected MST should have three edges: (0,1,1),(1,2,1) and (2,3,1).
    CHECK(output.find("(0, 1, 1)") != std::string::npos);
    CHECK(output.find("(1, 2, 1)") != std::string::npos);
    CHECK(output.find("(2, 3, 1)") != std::string::npos);
    CHECK(output.find("(0, 2, 5)") == std::string::npos);
    CHECK(output.find("(1, 3, 2)") == std::string::npos);
    CHECK(output.find("(4, 5, 0)") == std::string::npos);//Shouldn't be included given we dont take a source vertex(as instructed) so we start from vertex 0
}

TEST_CASE("Kruskal returns minimum spanning tree") {

    using namespace graph;
    Graph g(6);
    // Use the same graph as in the Prim test
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 1);
    g.addEdge(4, 5, 0);//Disconnected graph
    
    Graph kruskalTree = Algorithms::kruskal(g);
    std::string output = captureGraphOutput(kruskalTree);
    // The expected MST should have three edges: (0,1,1),(1,2,1) and (2,3,1).
    CHECK(output.find("(0, 1, 1)") != std::string::npos);
    CHECK(output.find("(1, 2, 1)") != std::string::npos);
    CHECK(output.find("(2, 3, 1)") != std::string::npos);
    CHECK(output.find("(0, 2, 5)") == std::string::npos);
    CHECK(output.find("(1, 3, 2)") == std::string::npos);
    CHECK(output.find("(4, 5, 0)") != std::string::npos);
}
