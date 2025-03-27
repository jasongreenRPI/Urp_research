#ifndef COMMUNITYCOMPARISON_H
#define COMMUNITYCOMPARISON_H



using namespace std;
#include <iostream>
#include <vector>
#include <utility> 
#include <cassert>
#include <unordered_map>
#include <set>           // For ordered set (std::set)
#include <unordered_set> // For hash-based set (std::unordered_set)
#include <fstream>       // For file I/O
#include <sstream>       // For string stream
#include <string>        // For string operations
#include <stdexcept>     // For exceptions
#include <memory>        // For shared_ptr
#include "../Community/Community.h"

template <typename T> 
class CommunityComparison {
private:
    std::map<T, int> nodeToTrueCommunity; // Maps each node to its ground truth community ID
    std::map<T,int> nodeToPredCommunity; // Maps each node to its predicted community ID

     

public:

    vector<Community<T>> loadCommunities(string filename) {
        ifstream myfile(filename);
        
        if (!myfile.is_open()) {
            throw runtime_error("Could not open file: " + filename);
        }

        map<int, Community<T>> communityMap;
        
        string line;
        while (getline(myfile, line)) {
            
            istringstream iss(line);
            T nodeId;
            int communityId;
            
            if (!(iss >> nodeId >> communityId)) {
                continue; 
            }
            
        
            if (communityMap.find(communityId) == communityMap.end()) {
                communityMap[communityId] = Community<T>();
            }
            
            communityMap[communityId].addNode(nodeId);
        }
        
        vector<Community<T>> communities;
        for (const auto& pair : communityMap) {
            communities.push_back(pair.second);
        }
        
        myfile.close();
        return communities;
    }
    
    set<T> getNodeSet(const vector<Community<T>>& communities) {
        set<T> nodeSet;
        for(const auto& community: communities) {
            for(const T& node: community.getNodes()) {
                nodeSet.insert(node);
            }
        }
        return nodeSet;
    }

    void handleMissingNodes(vector<Community<T>>& readComms, const set<T>& actualNodes, const set<T>& predNodes) {
        // Purpose: Address any discrepancies in node coverage between true and predicted communities
        // Implementation steps:

        // Find nodes that are in actualNodes but not in predNodes (missing nodes)
        // Create a new community containing all missing nodes (or individual communities)
        // Add the new community(ies) to the readComms vector
        Community<T> missingNodes;
        for(const auto& node: actualNodes) {
            if(predNodes.find(node) == predNodes.end()) {
                missingNodes.addNode(node);
            }
        }

         // Only add the community if there are missing nodes
        if(missingNodes.size() > 0) {
            readComms.push_back(missingNodes);
            
            // Log info about missing nodes
            cout << "Added " << missingNodes.size() << " missing nodes to a new community" << endl;
        }
        // log info about missing nodes // TO-DO LATER
    }

    map<T, int> createNodeToCommunityMap(const vector<Community<T>>& communities) {
        map<T, int> nodeToCommMap;
        
        // Iterate through all communities
        // i = communityID
        for (int i = 0; i < communities.size(); i++) {
            const auto& community = communities[i];
            
            // For each node in the community, map it to this community ID
            for (const T& node : community.getNodes()) {
                // Note: If a node exists in multiple communities, 
                // this will overwrite with the last community ID
                nodeToCommMap[node] = i;
            }
        }
        
        return nodeToCommMap;
    }

    vector<int> convertCommunityToLabelVectors(const vector<Community<T>>& communities) {
        map<T, int> nodeToLabel;  // Stores node -> community ID
        vector<int> labels;        // Final output label vector
        vector<T> nodeOrder;       // To ensure consistent ordering
    
        // Assign each node to a community ID
        // i = community ID
        for (int i = 0; i < communities.size(); i++) {
            for (const T& node : communities[i].getNodes()) {
                nodeToLabel[node] = i;
            }
        }

        // Extract nodes in a consistent order (sorted or insertion order)
        for (const auto& [node, _] : nodeToLabel) {
            nodeOrder.push_back(node);
        }

        // Convert to label vector
        for (const T& node : nodeOrder) {
            labels.push_back(nodeToLabel[node]);
        }

        return labels;
    }


    
};


#endif