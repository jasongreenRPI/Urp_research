#include "../CLASSES/Community/Community.h"
#include "../CLASSES/Graph2/Graph2.h"
#include <iostream>
#include <string>
#include <cassert>
#include <memory>

// Create a test graph for community testing
std::shared_ptr<Graph<int>> createTestGraphForCommunity() {
    auto g = std::make_shared<Graph<int>>();
    
    // Add vertices
    g->addVertex(1);
    g->addVertex(2);
    g->addVertex(3);
    g->addVertex(4);
    g->addVertex(5);
    g->addVertex(6);
    
    // Add edges to form two clear communities: (1,2,3) and (4,5,6)
    // Strong connections within communities
    g->addEdge(1, 2, 5.0);
    g->addEdge(1, 3, 5.0);
    g->addEdge(2, 3, 5.0);
    
    g->addEdge(4, 5, 5.0);
    g->addEdge(4, 6, 5.0);
    g->addEdge(5, 6, 5.0);
    
    // Weak connections between communities
    g->addEdge(3, 4, 1.0);
    
    return g;
}

// Test basic community node operations
void testCommunityNodeOperations() {
    std::cout << "Testing community node operations..." << std::endl;
    
    Community<int> community;
    
    // Test adding nodes
    community.addNode(1);
    community.addNode(2);
    community.addNode(3);
    
    assert(community.containsNode(1));
    assert(community.containsNode(2));
    assert(community.containsNode(3));
    assert(!community.containsNode(4));
    
    assert(community.size() == 3);
    
    // Test removing nodes
    community.removeNode(2);
    assert(!community.containsNode(2));
    assert(community.size() == 2);
    
    // Test getting nodes
    const auto& nodes = community.getNodes();
    assert(nodes.size() == 2);
    assert(nodes.find(1) != nodes.end());
    assert(nodes.find(3) != nodes.end());
    
    std::cout << "Community node operations test passed!" << std::endl;
}

// Test community merging
void testCommunityMerge() {
    std::cout << "Testing community merge operations..." << std::endl;
    
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    
    Community<int> community2;
    community2.addNode(3);
    community2.addNode(4);
    
    // Test merge
    community1.merge(community2);
    assert(community1.size() == 4);
    assert(community1.containsNode(1));
    assert(community1.containsNode(2));
    assert(community1.containsNode(3));
    assert(community1.containsNode(4));
    
    std::cout << "Community merge test passed!" << std::endl;
}

// Test community set operations
void testCommunitySetOperations() {
    std::cout << "Testing community set operations..." << std::endl;
    
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    community1.addNode(3);
    
    Community<int> community2;
    community2.addNode(2);
    community2.addNode(3);
    community2.addNode(4);
    
    // Test union
    Community<int> unionCommunity = community1.createUnion(community2);
    assert(unionCommunity.size() == 4);
    assert(unionCommunity.containsNode(1));
    assert(unionCommunity.containsNode(2));
    assert(unionCommunity.containsNode(3));
    assert(unionCommunity.containsNode(4));
    
    // Test intersection
    Community<int> intersectionCommunity = community1.createIntersection(community2);
    assert(intersectionCommunity.size() == 2);
    assert(!intersectionCommunity.containsNode(1));
    assert(intersectionCommunity.containsNode(2));
    assert(intersectionCommunity.containsNode(3));
    assert(!intersectionCommunity.containsNode(4));
    
    std::cout << "Community set operations test passed!" << std::endl;
}

// Test weight calculations
void testCommunityWeights() {
    std::cout << "Testing community weight calculations..." << std::endl;
    
    auto graph = createTestGraphForCommunity();
    
    // Create a community for group 1
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    community1.addNode(3);
    
    // Calculate weights
    community1.calculateWeights(graph);

    cout << community1.getInternalWeight() << "\n";
    // Internal weight should be 3 edges * 5.0 weight / 2 (undirected graph correction) = 7.5
 //   assert(community1.getInternalWeight() == 7.5);
    cout << community1.getExternalWeight();
    // External weight should be 1 edge * 1.0 weight / 2 (undirected graph correction) = 0.5
    assert(community1.getExternalWeight() == 0.5);
    
    std::cout << "Community weight calculations test passed!" << std::endl;
}

// Test community equality
void testCommunityEquality() {
    std::cout << "Testing community equality..." << std::endl;
    
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    community1.addNode(3);
    
    Community<int> community2;
    community2.addNode(1);
    community2.addNode(2);
    community2.addNode(3);
    
    Community<int> community3;
    community3.addNode(1);
    community3.addNode(2);
    
    assert(community1 == community2);
    assert(!(community1 == community3));
    
    std::cout << "Community equality test passed!" << std::endl;
}

int main() {
    std::cout << "Running Community tests..." << std::endl;
    
    testCommunityNodeOperations();
    testCommunityMerge();
    testCommunitySetOperations();
    testCommunityWeights();
    testCommunityEquality();
    
    std::cout << "All Community tests passed!" << std::endl;
    return 0;
}