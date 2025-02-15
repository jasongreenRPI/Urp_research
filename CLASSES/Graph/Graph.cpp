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

template <typename T>
void Graph<T>::removeEdge(const T& from, const T& to) {
    // Check if both vertices exist
    if (!hasVertex(from) || !hasVertex(to)) {
        error << "removeEdge: One or both vertices do not exist in graph." << endl;
        return;
    }

    // Remove edge in both directions (since it's an undirected graph)
    // Remove to from from's neighbor list
    adjacencyList[from].erase(
        remove_if(adjacencyList[from].begin(), adjacencyList[from].end(),
            [&](const pair<T, int>& edge) {
                return edge.first == to;
            }),
        adjacencyList[from].end()
    );

    // Remove from from to's neighbor list
    adjacencyList[to].erase(
        remove_if(adjacencyList[to].begin(), adjacencyList[to].end(),
            [&](const pair<T, int>& edge) {
                return edge.first == from;
            }),
        adjacencyList[to].end()
    );
}