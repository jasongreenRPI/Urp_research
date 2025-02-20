#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <utility> 
#include <unordered_map>
#include <cassert>


template <typename T>
class Graph  {
private:
    // Key: Vertx
    // Value of pairs: Neighbor vertex, edge weight
    unordered_map<T, vector<pair<T,int>>> adjacencyList; // Adjacency list to store each vertex, and the list of target nodes and asociate weights for this vertex
public:
    Graph() = default;

    // Core vertex operations
    void addVertex(const T& vertex); // adds a vertex to the adjacency list
    void removeVertex(const T& vertex); // removes a vertex from the adjancency list
    bool hasVertex(const T& vertex) const; // checks if a vertex is in the adjacecny list
    
    // Core edge operations
    void addEdge(const T& from, const T& to, int weight = 1); // find neighbors for "from" node, then add "to" node. Then do the same thing for the reciprocal edge
    void removeEdge(const T& from, const T& to); // Removes "to" from the array of neighbors for the "from" node. And the same for the reciprocal
    bool hasEdge(const T& from, const T& to) const; // done
    int getEdgeWeight(const T& from, const T& to) const; // done

    // Graph information
    size_t getVertexCount() const; // done
    size_t getEdgeCount() const;
    std::vector<T> getVertices() const; // done
    std::vector<std::pair<T, int>> getNeighbors(const T& vertex) const; // done
    int getDegree(const T& vertex) const; // done


};

#endif