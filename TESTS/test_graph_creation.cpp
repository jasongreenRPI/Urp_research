
#include "../CLASSES/Graph/Graph.h"
#include <iostream>
#include <cassert>

void runTests(const std::string& FILE_NAME) {
    std::cout << "Starting Graph tests...\n\n";
    
    // Initialize graph
    Graph<int> graph = Graph<int>(FILE_NAME);
    
    // Test vertex count
    std::cout << "Vertex count: " << graph.getVertexCount() << "\n";
    
    // Test edge count
    std::cout << "Edge count: " << graph.getEdgeCount() << "\n";
    
    // Get all vertices and test individual vertex operations
    auto vertices = graph.getVertices();
    std::cout << "\nTesting all vertices:\n";
    for(const auto& vertex : vertices) {
        std::cout << "Vertex " << vertex << ":\n";
        std::cout << "  Degree: " << graph.getDegree(vertex) << "\n";
        
        // Get and display neighbors
        auto neighbors = graph.getNeighbors(vertex);
        std::cout << "  Neighbors (vertex, weight): ";
        for(const auto& [neighbor, weight] : neighbors) {
            std::cout << "(" << neighbor << ", " << weight << ") ";
            
            // Verify edge exists and weight matches
            assert(graph.hasEdge(vertex, neighbor));
            assert(graph.getEdgeWeight(vertex, neighbor) == weight);
        }
        std::cout << "\n";
    }
    
    // Test edge operations
    std::cout << "\nTesting edge operations:\n";
    
    // Test adding new vertex and edge
    int newVertex = 9999;
    std::cout << "Adding new vertex " << newVertex << "\n";
    graph.addVertex(newVertex);
    assert(graph.hasVertex(newVertex));
    
    // Test adding edge to new vertex
    int existingVertex = vertices[0];
    int edgeWeight = 42;
    std::cout << "Adding edge between " << existingVertex << " and " << newVertex << "\n";
    graph.addEdge(existingVertex, newVertex, edgeWeight);
    assert(graph.hasEdge(existingVertex, newVertex));
    assert(graph.hasEdge(newVertex, existingVertex));  // Test bidirectional
    assert(graph.getEdgeWeight(existingVertex, newVertex) == edgeWeight);
    
    // Test removing edge
    std::cout << "Removing edge between " << existingVertex << " and " << newVertex << "\n";
    graph.removeEdge(existingVertex, newVertex);
    assert(!graph.hasEdge(existingVertex, newVertex));
    assert(!graph.hasEdge(newVertex, existingVertex));
    
    // Test removing vertex
    std::cout << "Removing vertex " << newVertex << "\n";
    graph.removeVertex(newVertex);
    assert(!graph.hasVertex(newVertex));
    
    // Test error conditions
    std::cout << "\nTesting error conditions:\n";
    try {
        graph.getEdgeWeight(newVertex, existingVertex);
        std::cout << "ERROR: Should have thrown exception for non-existent edge\n";
    } catch(const std::invalid_argument& e) {
        std::cout << "Successfully caught exception: " << e.what() << "\n";
    }
    
    std::cout << "\nAll tests completed successfully!\n";
}

int main() {
    const std::string FILE_NAME = "../DATA/edges_and_weights.txt";
    runTests(FILE_NAME);
    return 0;
};