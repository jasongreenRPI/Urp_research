#ifndef GRAPH_H
#define GRAPH_H
#include "../../includes/includes.cpp"


template <typename T>
class Graph  {
private:
    // Key: Vertx
    // Value of pairs: Neighbor vertex, edge weight
    unordered_map<T, vector<pair<T,int>>> adjacencyList; // Adjacency list to store each vertex, and the list of target nodes and asociate weights for this vertex
public:
    Graph() = default;

    // Core vertex operations
    void addVertex(const T& vertex);
    void removeVertex(const T& vertex);
    bool hasVertex(const T& vertex) const;
    
    // Core edge operations
    void addEdge(const T& from, const T& to, int weight = 1);
    void removeEdge(const T& from, const T& to);
    bool hasEdge(const T& from, const T& to) const;
    int getEdgeWeight(const T& from, const T& to) const;

    // Graph information
    size_t getVertexCount() const;
    size_t getEdgeCount() const;
    std::vector<T> getVertices() const;
    std::vector<std::pair<T, int>> getNeighbors(const T& vertex) const;
    int getDegree(const T& vertex) const;


};

#endif