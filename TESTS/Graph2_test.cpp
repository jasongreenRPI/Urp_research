#include "../CLASSES/Graph2/Graph2.h"
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

// Utility function to create a simple test graph
template <typename T>
Graph<T> createTestGraph() {
    Graph<T> g;
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    g.addVertex(4);
    
    g.addEdge(1, 2, 1.0);
    g.addEdge(2, 3, 2.0);
    g.addEdge(3, 4, 3.0);
    g.addEdge(4, 1, 4.0);
    
    return g;
}

// Test vertex operations
void testVertexOperations() {
    std::cout << "Testing vertex operations..." << std::endl;
    Graph<int> g;
    
    // Test adding vertices
    g.addVertex(1);
    g.addVertex(2);
    assert(g.hasVertex(1));
    assert(g.hasVertex(2));
    assert(!g.hasVertex(3));
    assert(g.getVertexCount() == 2);
    
    // Test removing vertices
    g.removeVertex(1);
    assert(!g.hasVertex(1));
    assert(g.hasVertex(2));
    assert(g.getVertexCount() == 1);
    
    std::cout << "Vertex operations test passed!" << std::endl;
}

// Test edge operations
void testEdgeOperations() {
    std::cout << "Testing edge operations..." << std::endl;
    Graph<int> g;
    
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    
    // Test adding edges
    g.addEdge(1, 2, 5.0);
    assert(g.hasEdge(1, 2));
    assert(g.hasEdge(2, 1)); // Undirected graph
    assert(g.getEdgeWeight(1, 2) == 5.0);
    assert(g.getEdgeCount() == 1);
    
    g.addEdge(2, 3, 10.0);
    assert(g.hasEdge(2, 3));
    assert(g.getEdgeWeight(2, 3) == 10.0);
    assert(g.getEdgeCount() == 2);
    
    // Test removing edges
    g.removeEdge(1, 2);
    assert(!g.hasEdge(1, 2));
    assert(!g.hasEdge(2, 1));
    assert(g.getEdgeCount() == 1);
    
    std::cout << "Edge operations test passed!" << std::endl;
}

// Test degree and weighted degree
void testDegreeOperations() {
    std::cout << "Testing degree operations..." << std::endl;
    Graph<int> g;
    
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);
    
    g.addEdge(1, 2, 5.0);
    g.addEdge(1, 3, 10.0);
    
    assert(g.getDegree(1) == 2);
    assert(g.getDegree(2) == 1);
    assert(g.getDegree(3) == 1);
    
    assert(g.getWeightedDegree(1) == 15.0);
    assert(g.getWeightedDegree(2) == 5.0);
    assert(g.getWeightedDegree(3) == 10.0);
    
    std::cout << "Degree operations test passed!" << std::endl;
}

// Test graph file I/O
void testFileIO() {
    std::cout << "Testing file I/O..." << std::endl;
    Graph<int> g = createTestGraph<int>();
    
    // Save graph to file
    g.saveToFile("test_graph.txt");
    
    // Load graph from file
    Graph<int> loadedGraph("test_graph.txt");
    
    // Verify loaded graph
    assert(loadedGraph.getVertexCount() == 4);
    assert(loadedGraph.getEdgeCount() == 4);
    assert(loadedGraph.hasEdge(1, 2));
    assert(loadedGraph.hasEdge(2, 3));
    assert(loadedGraph.hasEdge(3, 4));
    assert(loadedGraph.hasEdge(4, 1));
    
    std::cout << "File I/O test passed!" << std::endl;
}

// Test subgraph creation
void testSubgraph() {
    std::cout << "Testing subgraph creation..." << std::endl;
    Graph<int> g = createTestGraph<int>();
    
    std::set<int> vertices = {1, 2, 3};
    std::shared_ptr<Graph<int>> subgraph = g.createSubGraph(vertices);
    
    assert(subgraph->getVertexCount() == 3);
    assert(subgraph->getEdgeCount() == 2);
    assert(subgraph->hasEdge(1, 2));
    assert(subgraph->hasEdge(2, 3));
    assert(!subgraph->hasEdge(3, 4));
    
    std::cout << "Subgraph test passed!" << std::endl;
}

// Test modularity calculation with communities
void testModularity() {
    std::cout << "Testing modularity calculation..." << std::endl;
    Graph<int> g = createTestGraph<int>();
    
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    
    Community<int> community2;
    community2.addNode(3);
    community2.addNode(4);
    
    std::vector<Community<int>> communities = {community1, community2};
    double modularity = g.calculateModularity(communities);
    
    // Expected modularity for this particular graph and community split
    // would need to be manually calculated, this is just a test of the API
    std::cout << "Modularity: " << modularity << std::endl;
    assert(modularity >= -1.0 && modularity <= 1.0);
    
    std::cout << "Modularity test passed!" << std::endl;
}

int main() {
    std::cout << "Running Graph2 tests..." << std::endl;
    
    testVertexOperations();
    testEdgeOperations();
    testDegreeOperations();
    testFileIO();
    testSubgraph();
    testModularity();
    
    std::cout << "All Graph2 tests passed!" << std::endl;
    return 0;
}