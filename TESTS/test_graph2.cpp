#include "../CLASSES/Graph2/Graph2.h"
#include <iostream>
#include <cassert>
#include <string>

// Helper to get test values of the right type
template <typename T>
T getValue(int i) {
    if constexpr (std::is_same_v<T, std::string>) {
        return std::to_string(i); // Convert int to string
    } else {
        return static_cast<T>(i); // For int and other numeric types
    }
}

// Helper function to print test sections
void printTestHeader(const std::string& testName) {
    std::cout << "\n=== Testing " << testName << " ===\n";
}

// Test hasVertex function
template <typename T>
void testHasVertex() {
    printTestHeader("hasVertex");
    
    Graph<T> graph;
    
    // Test on empty graph
    assert(!graph.hasVertex(getValue<T>(1)));
    
    // Add a vertex
    graph.addVertex(getValue<T>(1));
    assert(graph.hasVertex(getValue<T>(1)));
    assert(!graph.hasVertex(getValue<T>(2)));
    
    std::cout << "hasVertex tests passed!\n";
}

// Test addVertex function
template <typename T>
void testAddVertex() {
    printTestHeader("addVertex");
    
    Graph<T> graph;
    
    // Add a vertex
    graph.addVertex(getValue<T>(1));
    assert(graph.hasVertex(getValue<T>(1)));
    
    // Add more vertices
    graph.addVertex(getValue<T>(2));
    graph.addVertex(getValue<T>(3));
    assert(graph.hasVertex(getValue<T>(2)));
    assert(graph.hasVertex(getValue<T>(3)));
    
    // Test exception on duplicate vertex
    try {
        graph.addVertex(getValue<T>(1));
        std::cerr << "ERROR: Should have thrown exception for duplicate vertex\n";
        assert(false);
    } catch(const std::invalid_argument& e) {
        std::cout << "Successfully caught exception: " << e.what() << "\n";
    }
    
    std::cout << "addVertex tests passed!\n";
}

// Test removeVertex function
template <typename T>
void testRemoveVertex() {
    printTestHeader("removeVertex");
    
    Graph<T> graph;
    
    // Add some vertices
    graph.addVertex(getValue<T>(1));
    graph.addVertex(getValue<T>(2));
    graph.addVertex(getValue<T>(3));
    
    // Test removing vertex
    graph.removeVertex(getValue<T>(2));
    assert(graph.hasVertex(getValue<T>(1)));
    assert(!graph.hasVertex(getValue<T>(2)));
    assert(graph.hasVertex(getValue<T>(3)));
    
    // Test removing non-existent vertex (should not throw)
    graph.removeVertex(getValue<T>(2));
    
    std::cout << "removeVertex tests passed!\n";
}

// Test hasEdge function
template <typename T>
void testHasEdge() {
    printTestHeader("hasEdge");
    
    Graph<T> graph;
    
    // Add vertices
    graph.addVertex(getValue<T>(1));
    graph.addVertex(getValue<T>(2));
    
    // Test on graph without edges
    assert(!graph.hasEdge(getValue<T>(1), getValue<T>(2)));
    
    std::cout << "hasEdge tests passed (limited)!\n";
}

int main() {
    std::cout << "Starting Graph2 Tests" << std::endl;
    
    // Test with int vertices
    std::cout << "\nTesting with int vertices:\n";
    testHasVertex<int>();
    testAddVertex<int>();
    testRemoveVertex<int>();
    testHasEdge<int>();
    
    // Test with string vertices
    std::cout << "\nTesting with string vertices:\n";
    testHasVertex<std::string>();
    testAddVertex<std::string>();
    testRemoveVertex<std::string>();
    testHasEdge<std::string>();
    
    std::cout << "\nAll implemented Graph2 tests completed successfully!\n";
    return 0;
}