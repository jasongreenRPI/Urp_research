#ifndef COMMUNITYCOMPARISON_H
#define COMMUNITYCOMPARISON_H

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
#include <map>           // For std::map
#include <cmath>         // For log2
#include <algorithm>     // For sort
#include "../Community/Community.h"

template <typename T> 
class CommunityComparison {
private:
    std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>> nodeToTrueCommunity; // Maps each node to its ground truth community ID
    std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>> nodeToPredCommunity; // Maps each node to its predicted community ID

public:

    vector<Community<T>> loadCommunities(string filename) {
        ifstream myfile(filename);
        
        if (!myfile.is_open()) {
            throw runtime_error("Could not open file: " + filename);
        }

        std::map<int, Community<T>, std::less<int>, std::allocator<std::pair<const int, Community<T>>>> communityMap;
        
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

    
};


#endif