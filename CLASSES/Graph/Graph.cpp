#include "Graph.h"




template <typename T>
bool Graph<T>::hasVertex(const T& vertex) const {
    return adjacencyList.find(vertex) != adjacencyList.end();
}

template <typename T>
void Graph<T>::addVertex(const T& vertex) {
    bool inGraph = hasVertex(vertex);
    
    if(!inGraph) {
        adjacencyList[vertex] = std::vector<std::pair<T,int>>();
    }
}

template <typename T>
size_t Graph<T>::getVertexCount() const {
    return adjacencyList.size();
}

template <typename T>
void Graph<T>::removeVertex(const T& vertex) {
    if (!hasVertex(vertex)) {
        error << "removeVertex: Vertex does not exist in graph." << endl;
        return;
    }

    // Remove the vertex from the adjacency list
    adjacencyList.erase(vertex);
    
    // Remove all edges pointing to this vertex
    for (auto& [v, neighbors] : adjacencyList) {
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(), [&](const pair<T, int>& edge) {
                               return edge.first == vertex;
                           }), 
            neighbors.end());
    }
}

