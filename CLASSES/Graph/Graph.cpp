#include "Graph.h"
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

template <typename T>
bool Graph<T>::hasVertex(const T& vertex) const {
    // Returns true if vertex exists in the graph
    return adjacencyList.find(vertex) != adjacencyList.end();
}

template <typename T>
Graph<T>::Graph(const std::string& filename) {
    adjacencyList.clear();
    std::ifstream file(filename);
    if(!file.is_open()) {
        throw runtime_error("Could not open file" + filename);
    }

    std::string line;
    while (std::getline(file,line)) {
        std::istringstream iss(line);
        T from, to;
        int weight;

        if(iss >> from >> to >> weight) {
            // from, to , weight

            if(!hasVertex(from)) {
                addVertex(from);
            }

            if(!hasVertex(to)) {
                addVertex(to);
            }

            if(!hasEdge(from,to) || !hasEdge(to,from)) {
                addEdge(from,to,weight);
            }
        } else {
            cout << "INVALID LINE FORMAT -> " << line;
        }
    }

}

template <typename T>
void Graph<T>::addVertex(const T &vertex)
{
    // Throws exception if vertex already exists
    if(hasVertex(vertex)) {
        throw std::invalid_argument("Cannot add vertex: Vertex already exists in graph");
    }
    // Creates new empty vector for vertex's neighbors
    adjacencyList[vertex] = std::vector<std::pair<T,int>>();
}

template <typename T>
size_t Graph<T>::getVertexCount() const {
    // Returns total number of vertices in graph
    return adjacencyList.size();
}

template <typename T>
void Graph<T>::removeVertex(const T& vertex) {
    // Throws exception if vertex doesn't exist
    if (!hasVertex(vertex)) {
        throw std::invalid_argument("Cannot remove vertex: Vertex does not exist in graph");
    }

    // Remove vertex from adjacency list
    adjacencyList.erase(vertex);
    
    // Remove all edges pointing to this vertex from remaining vertices
    for (auto& [v, neighbors] : adjacencyList) {
        // For each vertex's neighbor list:
        // 1. Use remove_if to move matching edges to end
        // 2. Erase moved edges
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                     [&](const pair<T, int>& edge) {
                         return edge.first == vertex;
                     }),
            neighbors.end());
    }
}


template <typename T>
void Graph<T>::addEdge(const T& from, const T& to, int weight) {
    // Validate source vertex exists
    
    if (!hasVertex(from)) {
        throw std::invalid_argument("Cannot add edge: Source vertex does not exist in graph");
    }
    // Validate destination vertex exists
    if (!hasVertex(to)) {
        throw std::invalid_argument("Cannot add edge: Destination vertex does not exist in graph");
    }
    // Validate weight is non-negative
    if (weight < 0) {
        throw std::invalid_argument("Cannot add edge: Weight cannot be negative");
    }

    // Add edge to graph
    adjacencyList[from].push_back({to, weight});
    adjacencyList[to].push_back({from,weight});
}

template <typename T>
void Graph<T>::removeEdge(const T& from, const T& to) {
    // Validate source vertex exists
    if (!hasVertex(from)) {
        throw std::invalid_argument("Cannot remove edge: Source vertex does not exist in graph");
    }
    // Validate destination vertex exists
    if (!hasVertex(to)) {
        throw std::invalid_argument("Cannot remove edge: Destination vertex does not exist in graph");
    }

    if(!hasEdge(from,to)) {
        throw std::invalid_argument("Cannot remove edge: Edge does not exist");
    }

    // Get reference to source vertex's neighbors
    auto& fromNeighbors = adjacencyList[from];
    size_t initialSizeOfFromNeighbors = fromNeighbors.size();

    
    // Remove edge from source vertex's neighbor list
    fromNeighbors.erase(
        remove_if(fromNeighbors.begin(), fromNeighbors.end(),
            [&to](const pair<T, int>& edge) {
                return edge.first == to;
            }),
        fromNeighbors.end()
    );


    // Remove reciprocal edge for undirected graph
    auto& toNeighbors = adjacencyList[to];
    size_t initialSizeOfToNeighbors = toNeighbors.size();
    toNeighbors.erase(
        remove_if(toNeighbors.begin(), toNeighbors.end(),
            [&from](const pair<T, int>& edge) {
                return edge.first == from;
            }),
        toNeighbors.end()
    );
}

template <typename T>
bool Graph<T>::hasEdge(const T& from, const T& to) const {
    if (!hasVertex(from) || !hasVertex(to)) {
        throw std::invalid_argument("Cannot check edge: One or both vertices do not exist");
    }
    
    const auto& fromNeighbors = adjacencyList.at(from);
    return std::find_if(fromNeighbors.begin(), fromNeighbors.end(),
        [&to](const auto& pair) { 
            return pair.first == to; 
        }) != fromNeighbors.end();
}

template <typename T>
int Graph<T>::getEdgeWeight(const T& from, const T& to) const {
    if (!hasVertex(from) || !hasVertex(to)) {
        throw std::invalid_argument("Cannot get edge weight: One or both vertices do not exist");
    }

    const auto& neighbors = adjacencyList.at(from);
    
    auto it = find_if(neighbors.begin(), neighbors.end(),
        [&to](const pair<T, int>& edge) {
            return edge.first == to;
        });
    
    if (it == neighbors.end()) {
        throw std::invalid_argument("Cannot get edge weight: Edge does not exist");
    }
    return it->second;
}

template <typename T>
std::vector<std::pair<T, int>> Graph<T>::getNeighbors(const T& vertex) const {
    if (!hasVertex(vertex)) {
        throw std::invalid_argument("Cannot get neighbors: Vertex does not exist");
    }
    return adjacencyList.at(vertex);
}

template <typename T>
std::vector<T> Graph<T>::getVertices() const {
    std::vector<T> verticesVector;
    
    for (const auto& [vertex, neighbors] : adjacencyList) {
        verticesVector.push_back(vertex);
    }
    return verticesVector;
}

template <typename T>
int Graph<T>::getDegree(const T& vertex) const {
    if (!hasVertex(vertex)) {
        throw std::invalid_argument("Cannot get degree: Vertex does not exist");
    }
    
    //  return the size of the neighbor vector
    return adjacencyList.at(vertex).size();
}

template <typename T>
size_t Graph<T>::getEdgeCount() const {
    size_t totalEdges = 0;
    
    // Sum up the number of edges from each vertex
    for (const auto& [vertex, neighbors] : adjacencyList) {
        totalEdges += neighbors.size();
    }
    
    // Since this is an undirected graph, each edge is counted twice
    // (once for each direction), so we divide by 2
    return totalEdges / 2;
}
