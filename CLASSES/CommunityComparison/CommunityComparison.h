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


    set<T> getCommonNodes(Community<T> community1, Community<T> community2) {
        set<T> commonNodes;
        for(const auto& node: community1.getNodes()) {
            if(community2.containsNode(node)) {
                commonNodes.insert(node);
            }
        }
        return commonNodes;
    }

    pair<vector<int>, vector<int>> convertCommunitiesToLabelVectors(vector<Community<T>>& trueCommunities, vector<Community<T>>& predCommunities) {
        
        set<T> trueCommunityNodes;
        set<T> predictedCommunityNodes;


        set<T> trueNodes = getNodeSet(trueCommunities);
        set<T> predNodes = getNodeSet(predCommunities);

        set<T> commonNodes;

        for(const auto& node: trueNodes) {
            if(predNodes.find(node) != predNodes.end()) {
                commonNodes.insert(node);
            }
        }

        vector<pair<T, int>> trueMappings;
        for (const T& commonNode : commonNodes) {
            for (size_t communityId = 0; communityId < trueCommunities.size(); communityId++) {
                const Community<T>& community = trueCommunities[communityId];
                if (community.containsNode(commonNode)) {
                    trueMappings.push_back(make_pair(commonNode, communityId));
                    break;  
                }
            }
        }

        vector<pair<T,int>> predMappings;
        for(const T& commonNode: commonNodes) {
            for(size_t communityId2 = 0; communityId2 < predCommunities.size(); communityId2++) {
                const Community<T>& community = predCommunities[communityId2];
                if(community.containsNode(commonNode)) {
                    predMappings.push_back(make_pair(commonNode,communityId2));
                    break;
                }
            }
        }
        
        // Create a map to ensure consistent ordering of nodes
        std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>> nodeIndexMap;
        int index = 0;
        for(const auto& node : commonNodes) {
            nodeIndexMap[node] = index++;
        }
        
        // Initialize the label vectors with the size of common nodes
        vector<int> trueLabels(commonNodes.size(), -1);
        vector<int> predLabels(commonNodes.size(), -1);
        
        // Fill the label vectors based on the mappings
        for(const auto& mapping : trueMappings) {
            int nodeIndex = nodeIndexMap[mapping.first];
            trueLabels[nodeIndex] = mapping.second;
        }
        
        for(const auto& mapping : predMappings) {
            int nodeIndex = nodeIndexMap[mapping.first];
            predLabels[nodeIndex] = mapping.second;
        }
        
        return make_pair(trueLabels, predLabels);
    }
    
    /**
     * Calculates the Normalized Mutual Information (NMI) between true and predicted communities
     * @param trueCommunities Vector of true communities
     * @param predCommunities Vector of predicted communities
     * @return NMI value between 0 and 1
     */
    double calculateNMI(const vector<Community<T>>& trueCommunities, const vector<Community<T>>& predCommunities) {
        // Convert communities to label vectors
        vector<Community<T>> trueCommsCopy = trueCommunities;
        vector<Community<T>> predCommsCopy = predCommunities;
        auto labelVectors = convertCommunitiesToLabelVectors(trueCommsCopy, predCommsCopy);
        
        vector<int>& trueLabels = labelVectors.first;
        vector<int>& predLabels = labelVectors.second;
        
        // Calculate NMI using the label vectors
        return normalizedMutualInfo(trueLabels, predLabels);
    }
    
    /**
     * Creates a mapping from nodes to their community IDs
     * @param communities Vector of communities
     * @return Map from nodes to community IDs
     */
    std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>> createNodeToCommunityMap(const vector<Community<T>>& communities) {
        std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>> nodeToCommunity;
        
        for (size_t communityId = 0; communityId < communities.size(); communityId++) {
            const Community<T>& community = communities[communityId];
            for (const T& node : community.getNodes()) {
                nodeToCommunity[node] = communityId;
            }
        }
        
        return nodeToCommunity;
    }
    
    /**
     * Converts node-to-community maps to label vectors
     * @param map1 First node-to-community map
     * @param map2 Second node-to-community map
     * @return Pair of label vectors
     */
    pair<vector<int>, vector<int>> convertMapsToLabelVectors(
        const std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>>& map1, 
        const std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>>& map2) {
        // Find common nodes
        set<T> commonNodes;
        for (const auto& pair : map1) {
            if (map2.find(pair.first) != map2.end()) {
                commonNodes.insert(pair.first);
            }
        }
        
        // Create a map to ensure consistent ordering of nodes
        std::map<T, int, std::less<T>, std::allocator<std::pair<const T, int>>> nodeIndexMap;
        int index = 0;
        for (const auto& node : commonNodes) {
            nodeIndexMap[node] = index++;
        }
        
        // Initialize the label vectors
        vector<int> labels1(commonNodes.size(), -1);
        vector<int> labels2(commonNodes.size(), -1);
        
        // Fill the label vectors
        for (const auto& node : commonNodes) {
            int nodeIndex = nodeIndexMap[node];
            labels1[nodeIndex] = map1.at(node);
            labels2[nodeIndex] = map2.at(node);
        }
        
        return make_pair(labels1, labels2);
    }
    
    /**
     * Prints statistics about the communities
     * @param communities Vector of communities
     */
    void printCommunityStatistics(const vector<Community<T>>& communities) {
        cout << "Community Statistics:" << endl;
        cout << "Number of communities: " << communities.size() << endl;
        
        // Calculate total nodes
        set<T> allNodes;
        for (const auto& community : communities) {
            for (const auto& node : community.getNodes()) {
                allNodes.insert(node);
            }
        }
        cout << "Total unique nodes: " << allNodes.size() << endl;
        
        // Calculate community size statistics
        vector<size_t> communitySizes;
        for (const auto& community : communities) {
            communitySizes.push_back(community.size());
        }
        
        // Sort for median calculation
        sort(communitySizes.begin(), communitySizes.end());
        
        // Calculate statistics
        double avgSize = 0;
        for (size_t size : communitySizes) {
            avgSize += size;
        }
        avgSize /= communitySizes.size();
        
        size_t medianSize = communitySizes[communitySizes.size() / 2];
        size_t minSize = communitySizes.front();
        size_t maxSize = communitySizes.back();
        
        cout << "Average community size: " << avgSize << endl;
        cout << "Median community size: " << medianSize << endl;
        cout << "Minimum community size: " << minSize << endl;
        cout << "Maximum community size: " << maxSize << endl;
    }
    
    /**
     * Compares community assignments between true and predicted communities
     * @param trueCommunities Vector of true communities
     * @param predCommunities Vector of predicted communities
     */
    void compareCommunityAssignments(const vector<Community<T>>& trueCommunities, const vector<Community<T>>& predCommunities) {
        // Get node sets
        set<T> trueNodes = getNodeSet(trueCommunities);
        set<T> predNodes = getNodeSet(predCommunities);
        
        // Find common nodes
        set<T> commonNodes;
        for (const auto& node : trueNodes) {
            if (predNodes.find(node) != predNodes.end()) {
                commonNodes.insert(node);
            }
        }
        
        // Find nodes in true but not in predicted
        set<T> missingNodes;
        for (const auto& node : trueNodes) {
            if (predNodes.find(node) == predNodes.end()) {
                missingNodes.insert(node);
            }
        }
        
        // Find nodes in predicted but not in true
        set<T> extraNodes;
        for (const auto& node : predNodes) {
            if (trueNodes.find(node) == trueNodes.end()) {
                extraNodes.insert(node);
            }
        }
        
        // Print statistics
        cout << "Community Assignment Comparison:" << endl;
        cout << "True communities: " << trueCommunities.size() << endl;
        cout << "Predicted communities: " << predCommunities.size() << endl;
        cout << "True nodes: " << trueNodes.size() << endl;
        cout << "Predicted nodes: " << predNodes.size() << endl;
        cout << "Common nodes: " << commonNodes.size() << endl;
        cout << "Missing nodes: " << missingNodes.size() << endl;
        cout << "Extra nodes: " << extraNodes.size() << endl;
        
        // Calculate NMI
        double nmi = calculateNMI(trueCommunities, predCommunities);
        cout << "Normalized Mutual Information: " << nmi << endl;
    }
    
    /**
     * Calculates the normalized mutual information between two label vectors
     * @param labels_true True labels
     * @param labels_pred Predicted labels
     * @return NMI value between 0 and 1
     */
    double normalizedMutualInfo(const vector<int>& labels_true, const vector<int>& labels_pred) {
        // Count occurrences of each label
        std::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>> trueCounts;
        std::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>> predCounts;
        std::map<pair<int, int>, int, std::less<pair<int, int>>, std::allocator<std::pair<const pair<int, int>, int>>> jointCounts;
        
        for (size_t i = 0; i < labels_true.size(); i++) {
            int trueLabel = labels_true[i];
            int predLabel = labels_pred[i];
            
            trueCounts[trueLabel]++;
            predCounts[predLabel]++;
            jointCounts[make_pair(trueLabel, predLabel)]++;
        }
        
        // Calculate probabilities
        double n = static_cast<double>(labels_true.size());
        vector<double> p_true, p_pred, p_joint;
        
        for (const auto& pair : trueCounts) {
            p_true.push_back(pair.second / n);
        }
        
        for (const auto& pair : predCounts) {
            p_pred.push_back(pair.second / n);
        }
        
        for (const auto& pair : jointCounts) {
            p_joint.push_back(pair.second / n);
        }
        
        // Calculate entropies
        double h_true = shannonEntropy(p_true);
        double h_pred = shannonEntropy(p_pred);
        
        // Calculate joint entropy
        double h_joint = 0;
        for (const auto& p : p_joint) {
            if (p > 0) {
                h_joint -= p * log2(p);
            }
        }
        
        // Calculate mutual information
        double mi = h_true + h_pred - h_joint;
        
        // Calculate normalized mutual information
        if (h_true + h_pred == 0) {
            return 0;
        }
        
        return 2 * mi / (h_true + h_pred);
    }
    
    /**
     * Calculates the Shannon entropy of a probability distribution
     * @param probabilities Vector of probabilities
     * @return Entropy value
     */
    double shannonEntropy(const vector<double>& probabilities) {
        double entropy = 0;
        for (double p : probabilities) {
            if (p > 0) {
                entropy -= p * log2(p);
            }
        }
        return entropy;
    }

    
};


#endif



