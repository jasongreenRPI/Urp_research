#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>  // for set operations
#include <memory>     // for unique_ptr
#include <utility>


using namespace std;

// Forward declaration - assuming Graph will be defined elsewhere
template <typename T>
class Graph;

template <typename T>
class Community {
private:
    set<T> nodes;  // Changed from unordered_set to set
    double internalWeight = 0.0;
    double externalWeight = 0.0;
    shared_ptr<Graph<T>> cachedGraphRef = nullptr; // last graph we calculated a score for

public:
    Community() = default;
    
    // Node operations
    void addNode(const T& node) {
        nodes.insert(node);
    }
    
    void removeNode(const T& node) {
        nodes.erase(node);
    }
    
    bool containsNode(const T& node) const {
        return nodes.find(node) != nodes.end();
    }
    
    size_t size() const {
        return nodes.size();
    }
    
    const set<T>& getNodes() const {  // Changed return type
        return nodes;
    }
    
    vector<T> getNodesSorted() const {
        return vector<T>(nodes.begin(), nodes.end());
    }
    
    void merge(Community<T>& other) {
        for(const auto& node: other.getNodes()) {
            nodes.insert(node);
        }
        
        if(cachedGraphRef) {
            // Recalculate score with new nodes and old cached graph
            calculateWeights(cachedGraphRef);
        }
    }
    
    Community<T> createUnion(const Community<T>& other) const {
        Community<T> result;
        std::set_union(
            nodes.begin(), nodes.end(),
            other.getNodes().begin(), other.getNodes().end(),
            std::inserter(result.nodes, result.nodes.begin())
        );
        return result;
    }

    Community<T> createIntersection(const Community<T>& other) const {
        Community<T> result;
        std::set_intersection(
            nodes.begin(), nodes.end(),
            other.getNodes().begin(), other.getNodes().end(),
            std::inserter(result.nodes, result.nodes.begin())
        );
        return result;
    }

    bool operator==(const Community<T>& other) const {
        return nodes==other.getNodes();
    }

    void calculateWeights(shared_ptr<Graph<T>>& graph) {
        cachedGraphRef = graph;
        
        // Reset weights before calculation
        internalWeight = 0.0;
        externalWeight = 0.0;
        
        const auto& edges = graph->getEdgesWithWeight();
        
        for(const auto& [pair, weight]: edges) {
            bool firstInCommunity = containsNode(pair.first);
            bool secondInCommunity = containsNode(pair.second);
            
            if(firstInCommunity && secondInCommunity) {
                // Both endpoints in community - internal edge
                internalWeight += weight;
            } else if(firstInCommunity || secondInCommunity) {
                // Only one endpoint in community - external edge
                externalWeight += weight;
            }
            // Ignore edges where neither endpoint is in the community
        }
        
        internalWeight /= 2;
        externalWeight /= 2;
    }

    double getInternalWeight() const {
        return internalWeight;
    }

    double getExternalWeight() const {
        return externalWeight;
    }

};

#endif