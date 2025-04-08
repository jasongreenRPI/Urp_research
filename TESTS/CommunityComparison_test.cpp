#include "../CLASSES/CommunityComparison/CommunityComparison.h"
#include "../CLASSES/Community/Community.h"
#include <iostream>
#include <string>
#include <cassert>
#include <memory>
#include <fstream>
#include <sstream>

// Helper function to create test communities
std::vector<Community<int>> createTestCommunities() {
    std::vector<Community<int>> communities;
    
    // Create community 1
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    community1.addNode(3);
    communities.push_back(community1);
    
    // Create community 2
    Community<int> community2;
    community2.addNode(4);
    community2.addNode(5);
    community2.addNode(6);
    communities.push_back(community2);
    
    // Create community 3
    Community<int> community3;
    community3.addNode(7);
    community3.addNode(8);
    community3.addNode(9);
    communities.push_back(community3);
    
    return communities;
}

// Helper function to create a test file for loadCommunities
void createTestCommunityFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        // Format: nodeId communityId
        file << "1 0\n";  // Node 1 in community 0
        file << "2 0\n";  // Node 2 in community 0
        file << "3 0\n";  // Node 3 in community 0
        file << "4 1\n";  // Node 4 in community 1
        file << "5 1\n";  // Node 5 in community 1
        file << "6 1\n";  // Node 6 in community 1
        file << "7 2\n";  // Node 7 in community 2
        file << "8 2\n";  // Node 8 in community 2
        file << "9 2\n";  // Node 9 in community 2
        file.close();
    }
}

// Test loadCommunities method
void testLoadCommunities() {
    std::cout << "Testing loadCommunities method..." << std::endl;
    
    // Create a test file
    std::string filename = "test_communities.txt";
    createTestCommunityFile(filename);
    
    // Create CommunityComparison object and load communities
    CommunityComparison<int> cc;
    std::vector<Community<int>> communities = cc.loadCommunities(filename);
    
    // Verify the loaded communities
    assert(communities.size() == 3);
    
    // Check community 0
    assert(communities[0].containsNode(1));
    assert(communities[0].containsNode(2));
    assert(communities[0].containsNode(3));
    
    // Check community 1
    assert(communities[1].containsNode(4));
    assert(communities[1].containsNode(5));
    assert(communities[1].containsNode(6));
    
    // Check community 2
    assert(communities[2].containsNode(7));
    assert(communities[2].containsNode(8));
    assert(communities[2].containsNode(9));
    
    // Clean up
    std::remove(filename.c_str());
    
    std::cout << "loadCommunities test passed!" << std::endl;
}

// Test getNodeSet method
void testGetNodeSet() {
    std::cout << "Testing getNodeSet method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> communities = createTestCommunities();
    
    // Create CommunityComparison object and get node set
    CommunityComparison<int> cc;
    std::set<int> nodeSet = cc.getNodeSet(communities);
    
    // Verify the node set
    assert(nodeSet.size() == 9);
    for (int i = 1; i <= 9; i++) {
        assert(nodeSet.find(i) != nodeSet.end());
    }
    
    std::cout << "getNodeSet test passed!" << std::endl;
}

// Test handleMissingNodes method
void testHandleMissingNodes() {
    std::cout << "Testing handleMissingNodes method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> communities = createTestCommunities();
    
    // Create sets of actual and predicted nodes
    std::set<int> actualNodes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::set<int> predNodes = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Create CommunityComparison object and handle missing nodes
    CommunityComparison<int> cc;
    cc.handleMissingNodes(communities, actualNodes, predNodes);
    
    // Verify that a new community was added with the missing nodes
    assert(communities.size() == 4);
    assert(communities[3].containsNode(10));
    assert(communities[3].containsNode(11));
    
    std::cout << "handleMissingNodes test passed!" << std::endl;
}

// Test getCommonNodes method
void testGetCommonNodes() {
    std::cout << "Testing getCommonNodes method..." << std::endl;
    
    // Create test communities
    Community<int> community1;
    community1.addNode(1);
    community1.addNode(2);
    community1.addNode(3);
    community1.addNode(4);
    
    Community<int> community2;
    community2.addNode(3);
    community2.addNode(4);
    community2.addNode(5);
    community2.addNode(6);
    
    // Create CommunityComparison object and get common nodes
    CommunityComparison<int> cc;
    std::set<int> commonNodes = cc.getCommonNodes(community1, community2);
    
    // Verify the common nodes
    assert(commonNodes.size() == 2);
    assert(commonNodes.find(3) != commonNodes.end());
    assert(commonNodes.find(4) != commonNodes.end());
    
    std::cout << "getCommonNodes test passed!" << std::endl;
}

// Test convertCommunitiesToLabelVectors method
void testConvertCommunitiesToLabelVectors() {
    std::cout << "Testing convertCommunitiesToLabelVectors method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> trueCommunities;
    
    // True community 0: nodes 1, 2, 3
    Community<int> trueCommunity0;
    trueCommunity0.addNode(1);
    trueCommunity0.addNode(2);
    trueCommunity0.addNode(3);
    trueCommunities.push_back(trueCommunity0);
    
    // True community 1: nodes 4, 5, 6
    Community<int> trueCommunity1;
    trueCommunity1.addNode(4);
    trueCommunity1.addNode(5);
    trueCommunity1.addNode(6);
    trueCommunities.push_back(trueCommunity1);
    
    // Predicted communities
    std::vector<Community<int>> predCommunities;
    
    // Predicted community 0: nodes 1, 2, 4 (slightly different from true)
    Community<int> predCommunity0;
    predCommunity0.addNode(1);
    predCommunity0.addNode(2);
    predCommunity0.addNode(4);
    predCommunities.push_back(predCommunity0);
    
    // Predicted community 1: nodes 3, 5, 6
    Community<int> predCommunity1;
    predCommunity1.addNode(3);
    predCommunity1.addNode(5);
    predCommunity1.addNode(6);
    predCommunities.push_back(predCommunity1);
    
    // Create CommunityComparison object and convert to label vectors
    CommunityComparison<int> cc;
    auto labelVectors = cc.convertCommunitiesToLabelVectors(trueCommunities, predCommunities);
    
    // Verify the label vectors
    std::vector<int>& trueLabels = labelVectors.first;
    std::vector<int>& predLabels = labelVectors.second;
    
    // Check that the vectors have the same size
    assert(trueLabels.size() == predLabels.size());
    
    // Check that the vectors contain the correct labels
    // We need to find the indices of each node in the vectors
    std::map<int, int> nodeToIndex;
    int index = 0;
    for (int node : {1, 2, 3, 4, 5, 6}) {
        nodeToIndex[node] = index++;
    }
    
    // Check true labels
    assert(trueLabels[nodeToIndex[1]] == 0);
    assert(trueLabels[nodeToIndex[2]] == 0);
    assert(trueLabels[nodeToIndex[3]] == 0);
    assert(trueLabels[nodeToIndex[4]] == 1);
    assert(trueLabels[nodeToIndex[5]] == 1);
    assert(trueLabels[nodeToIndex[6]] == 1);
    
    // Check predicted labels
    assert(predLabels[nodeToIndex[1]] == 0);
    assert(predLabels[nodeToIndex[2]] == 0);
    assert(predLabels[nodeToIndex[3]] == 1);
    assert(predLabels[nodeToIndex[4]] == 0);
    assert(predLabels[nodeToIndex[5]] == 1);
    assert(predLabels[nodeToIndex[6]] == 1);
    
    std::cout << "convertCommunitiesToLabelVectors test passed!" << std::endl;
}

// Test calculateNMI method
void testCalculateNMI() {
    std::cout << "Testing calculateNMI method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> trueCommunities;
    
    // True community 0: nodes 1, 2, 3
    Community<int> trueCommunity0;
    trueCommunity0.addNode(1);
    trueCommunity0.addNode(2);
    trueCommunity0.addNode(3);
    trueCommunities.push_back(trueCommunity0);
    
    // True community 1: nodes 4, 5, 6
    Community<int> trueCommunity1;
    trueCommunity1.addNode(4);
    trueCommunity1.addNode(5);
    trueCommunity1.addNode(6);
    trueCommunities.push_back(trueCommunity1);
    
    // Predicted communities - perfect match
    std::vector<Community<int>> predCommunities;
    
    // Predicted community 0: nodes 1, 2, 3
    Community<int> predCommunity0;
    predCommunity0.addNode(1);
    predCommunity0.addNode(2);
    predCommunity0.addNode(3);
    predCommunities.push_back(predCommunity0);
    
    // Predicted community 1: nodes 4, 5, 6
    Community<int> predCommunity1;
    predCommunity1.addNode(4);
    predCommunity1.addNode(5);
    predCommunity1.addNode(6);
    predCommunities.push_back(predCommunity1);
    
    // Create CommunityComparison object and calculate NMI
    CommunityComparison<int> cc;
    double nmi = cc.calculateNMI(trueCommunities, predCommunities);
    
    // Verify that NMI is 1.0 for perfect match
    assert(nmi == 1.0);
    
    // Now test with imperfect match
    std::vector<Community<int>> imperfectPredCommunities;
    
    // Predicted community 0: nodes 1, 2, 4 (slightly different)
    Community<int> imperfectPredCommunity0;
    imperfectPredCommunity0.addNode(1);
    imperfectPredCommunity0.addNode(2);
    imperfectPredCommunity0.addNode(4);
    imperfectPredCommunities.push_back(imperfectPredCommunity0);
    
    // Predicted community 1: nodes 3, 5, 6
    Community<int> imperfectPredCommunity1;
    imperfectPredCommunity1.addNode(3);
    imperfectPredCommunity1.addNode(5);
    imperfectPredCommunity1.addNode(6);
    imperfectPredCommunities.push_back(imperfectPredCommunity1);
    
    // Calculate NMI for imperfect match
    double imperfectNmi = cc.calculateNMI(trueCommunities, imperfectPredCommunities);
    
    // Verify that NMI is less than 1.0 for imperfect match
    assert(imperfectNmi < 1.0);
    
    std::cout << "calculateNMI test passed!" << std::endl;
}

// Test createNodeToCommunityMap method
void testCreateNodeToCommunityMap() {
    std::cout << "Testing createNodeToCommunityMap method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> communities = createTestCommunities();
    
    // Create CommunityComparison object and create node to community map
    CommunityComparison<int> cc;
    std::map<int, int> nodeToCommunity = cc.createNodeToCommunityMap(communities);
    
    // Verify the map
    assert(nodeToCommunity[1] == 0);
    assert(nodeToCommunity[2] == 0);
    assert(nodeToCommunity[3] == 0);
    assert(nodeToCommunity[4] == 1);
    assert(nodeToCommunity[5] == 1);
    assert(nodeToCommunity[6] == 1);
    assert(nodeToCommunity[7] == 2);
    assert(nodeToCommunity[8] == 2);
    assert(nodeToCommunity[9] == 2);
    
    std::cout << "createNodeToCommunityMap test passed!" << std::endl;
}

// Test convertMapsToLabelVectors method
void testConvertMapsToLabelVectors() {
    std::cout << "Testing convertMapsToLabelVectors method..." << std::endl;
    
    // Create test maps
    std::map<int, int> map1;
    map1[1] = 0;
    map1[2] = 0;
    map1[3] = 1;
    map1[4] = 1;
    
    std::map<int, int> map2;
    map2[1] = 0;
    map2[2] = 1;
    map2[3] = 0;
    map2[5] = 1;
    
    // Create CommunityComparison object and convert maps to label vectors
    CommunityComparison<int> cc;
    auto labelVectors = cc.convertMapsToLabelVectors(map1, map2);
    
    // Verify the label vectors
    std::vector<int>& labels1 = labelVectors.first;
    std::vector<int>& labels2 = labelVectors.second;
    
    // Check that the vectors have the same size (common nodes: 1, 2, 3)
    assert(labels1.size() == 3);
    assert(labels2.size() == 3);
    
    // Check that the vectors contain the correct labels
    // We need to find the indices of each node in the vectors
    std::map<int, int> nodeToIndex;
    int index = 0;
    for (int node : {1, 2, 3}) {
        nodeToIndex[node] = index++;
    }
    
    // Check labels1
    assert(labels1[nodeToIndex[1]] == 0);
    assert(labels1[nodeToIndex[2]] == 0);
    assert(labels1[nodeToIndex[3]] == 1);
    
    // Check labels2
    assert(labels2[nodeToIndex[1]] == 0);
    assert(labels2[nodeToIndex[2]] == 1);
    assert(labels2[nodeToIndex[3]] == 0);
    
    std::cout << "convertMapsToLabelVectors test passed!" << std::endl;
}

// Test printCommunityStatistics method
void testPrintCommunityStatistics() {
    std::cout << "Testing printCommunityStatistics method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> communities = createTestCommunities();
    
    // Create CommunityComparison object and print community statistics
    CommunityComparison<int> cc;
    cc.printCommunityStatistics(communities);
    
    // This test doesn't have assertions because it's just printing to console
    // We're just making sure it doesn't crash
    
    std::cout << "printCommunityStatistics test passed!" << std::endl;
}

// Test compareCommunityAssignments method
void testCompareCommunityAssignments() {
    std::cout << "Testing compareCommunityAssignments method..." << std::endl;
    
    // Create test communities
    std::vector<Community<int>> trueCommunities;
    
    // True community 0: nodes 1, 2, 3
    Community<int> trueCommunity0;
    trueCommunity0.addNode(1);
    trueCommunity0.addNode(2);
    trueCommunity0.addNode(3);
    trueCommunities.push_back(trueCommunity0);
    
    // True community 1: nodes 4, 5, 6
    Community<int> trueCommunity1;
    trueCommunity1.addNode(4);
    trueCommunity1.addNode(5);
    trueCommunity1.addNode(6);
    trueCommunities.push_back(trueCommunity1);
    
    // Predicted communities - with some differences
    std::vector<Community<int>> predCommunities;
    
    // Predicted community 0: nodes 1, 2, 4
    Community<int> predCommunity0;
    predCommunity0.addNode(1);
    predCommunity0.addNode(2);
    predCommunity0.addNode(4);
    predCommunities.push_back(predCommunity0);
    
    // Predicted community 1: nodes 3, 5, 6
    Community<int> predCommunity1;
    predCommunity1.addNode(3);
    predCommunity1.addNode(5);
    predCommunity1.addNode(6);
    predCommunities.push_back(predCommunity1);
    
    // Create CommunityComparison object and compare community assignments
    CommunityComparison<int> cc;
    cc.compareCommunityAssignments(trueCommunities, predCommunities);
    
    // This test doesn't have assertions because it's just printing to console
    // We're just making sure it doesn't crash
    
    std::cout << "compareCommunityAssignments test passed!" << std::endl;
}

int main() {
    std::cout << "Running CommunityComparison tests..." << std::endl;
    
    testLoadCommunities();
    testGetNodeSet();
    testHandleMissingNodes();
    testGetCommonNodes();
    testConvertCommunitiesToLabelVectors();
    testCalculateNMI();
    testCreateNodeToCommunityMap();
    testConvertMapsToLabelVectors();
    testPrintCommunityStatistics();
    testCompareCommunityAssignments();
    
    std::cout << "All CommunityComparison tests passed!" << std::endl;
    return 0;
} 