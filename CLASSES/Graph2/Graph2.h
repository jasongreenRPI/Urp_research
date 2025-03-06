#ifndef GRAPH2_H
#define GRAPH2_H

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
using namespace std;

// Define the hash function for std::pair
// Built in std hashing
// unordered_map expects a type for it's third arg, thats why we are using this Struct with a override on the () operator.
// PairHash is a now a functor, meaning we can call it like this:
    // PairHash<T> hasher; // Create the functor
    // size_t hash_value = hasher(key); // Call operator()
template <typename T>
struct PairHash {
    std::size_t operator()(const std::pair<T, T>& p) const {
        // Get the hash of each element
        auto h1 = std::hash<T>{}(p.first);
        auto h2 = std::hash<T>{}(p.second);
        
        // Combine the hashes
        return h1 ^ (h2 << 1);
    }
};

template <typename T>
class Graph {
private:
    unordered_map<T, vector<pair<T,double>>> adjacencyList;
    
    double totalWeight = 0;
    unordered_map<T, double> weightedDegrees; // v , sum(all connected edges)
    unordered_map<pair<T, T>, double, PairHash<T>> edgeLookup; // for quick lookup,
    set<T> vertices;
    //stores a pair of 2 vertices and their corresponding edge weight,
    // we also define the "type" for the hash in the unordered_map
    // **WHEN YOU STORE IT SHOULD BE a,b where a<b**.
public:
    Graph() = default;
    
    // Constructor to load a graph from a file
    Graph(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
        
        string line;
        
        // Read the number of vertices
        size_t numVertices;
        if (getline(file, line)) {
            numVertices = stoul(line);
        } else {
            throw std::runtime_error("Error reading number of vertices");
        }
        
        // Read the vertices
        if (getline(file, line)) {
            stringstream ss(line);
            T vertex;
            while (ss >> vertex) {
                addVertex(vertex);
            }
        } else {
            throw std::runtime_error("Error reading vertices");
        }
        
        // Check if we read the correct number of vertices
        if (getVertexCount() != numVertices) {
            throw std::runtime_error("Mismatch in vertex count: expected " + 
                                     to_string(numVertices) + ", got " + 
                                     to_string(getVertexCount()));
        }
        
        // Read the number of edges
        size_t numEdges;
        if (getline(file, line)) {
            numEdges = stoul(line);
        } else {
            throw std::runtime_error("Error reading number of edges");
        }
        
        // Read the edges
        for (size_t i = 0; i < numEdges; ++i) {
            if (getline(file, line)) {
                stringstream ss(line);
                T from, to;
                double weight;
                if (ss >> from >> to >> weight) {
                    // Only add the edge if both vertices exist and the edge doesn't already exist
                    if (!hasVertex(from)) {
                        throw std::runtime_error("Vertex not found: " + to_string(from));
                    }
                    if (!hasVertex(to)) {
                        throw std::runtime_error("Vertex not found: " + to_string(to));
                    }
                    if (hasEdge(from, to)) {
                        throw std::runtime_error("Duplicate edge: " + to_string(from) + " - " + to_string(to));
                    }
                    
                    addEdge(from, to, weight);
                } else {
                    throw std::runtime_error("Error parsing edge at line " + to_string(i+4));
                }
            } else {
                throw std::runtime_error("Expected " + to_string(numEdges) + 
                                         " edges, but only found " + to_string(i));
            }
        }
        
        file.close();
    }
    
    //vertex operations

    bool hasVertex(const T& vertex) const {
        return adjacencyList.find(vertex) != adjacencyList.end();
    }
    void addVertex(const T& vertex) {
        if (hasVertex(vertex)) {
            throw std::invalid_argument("Vertex already exists in graph");
        }
        vertices.insert(vertex);
        adjacencyList[vertex] = std::vector<std::pair<T,double>>();
        weightedDegrees[vertex] = 0.0;  // Initialize other data structures
    }
    void removeVertex(const T& vertex) {
        // check if vertex exists
        if(!hasVertex(vertex)) { return; }
        
        //2 remove the sum of all edges for vertex from totalWeight
        double weightedDegreeForVertex = weightedDegrees.at(vertex);
        totalWeight -= weightedDegreeForVertex;
        
        std::vector<std::pair<T,T>> keysToRemove;
        
        //3 find keys to remove from edgeLookup.
        
        //a Decrement weightedDegrees[neighbor] by weight for each neighbor
        for(const auto& [neighbor, weight] : adjacencyList.at(vertex)) {
            auto keyToRemove = vertex < neighbor ? make_pair(vertex,neighbor) : make_pair(neighbor,vertex);
            keysToRemove.push_back(keyToRemove);
            
            // Update weightedDegrees for the neighbor
            weightedDegrees.at(neighbor) -= weight;
        }
        
        //4 Remove keys from edgeLookup
        for(const auto& pair : keysToRemove) {
            edgeLookup.erase(pair);
        }
        
        //5 remove v from other vertices neighbors array
        for(auto& [v, neighbors] : adjacencyList) {
            if(v == vertex) {continue;}
            
            neighbors.erase(
                std::remove_if(
                    neighbors.begin(), neighbors.end(),
                    [&vertex](const std::pair<T, double>& edge) {
                        return edge.first == vertex;
                    }
                ),
                neighbors.end()
            );
        }
        
        //6 adjacencyList erase vertex , weightedDegrees erase vertex,
        adjacencyList.erase(vertex);
        weightedDegrees.erase(vertex);
        vertices.erase(vertex);
    }
    size_t getVertexCount() const { return vertices.size(); }
    const set<T>& getVertices() const {
        return vertices;
    }
    double getWeightedDegree(const T& vertex) const {
        if (!hasVertex(vertex)) {
            throw std::logic_error("Vertex does not exist");
        }
        return weightedDegrees.at(vertex);
    }
    int getDegree(const T& vertex) const {
        if (!hasVertex(vertex)) {
            throw std::logic_error("Vertex does not exist");
        }
        return adjacencyList.at(vertex).size();
    }
    
    
    //edge operations

    bool hasEdge(const T& from, const T& to) const {
        pair<T,T> key = makeNomimalEdge(from,to);
        
        // key = a,b where a<b
        // this is because edgeLookup stores keys in ascending order.
        return edgeLookup.find(key) != edgeLookup.end();
    }
    void addEdge(const T& from, const T& to, const double weight) {
        if(adjacencyList.find(from) == adjacencyList.end()) {
            addVertex(from);
        }

        if(adjacencyList.find(to) == adjacencyList.end()) {
            addVertex(to);
        }

        if (weight < 0) {
            throw std::invalid_argument("Weight can't be negative");
        }

        if(!hasEdge(from,to)) {
            // Data structures affected: adjancencyList, totalWeight, weightedDegrees, edgeLookup
            
            //1 First update graph's total weight
            totalWeight += weight;

            //2 Update adjancencyList
            adjacencyList.at(from).push_back(make_pair(to, weight));
            adjacencyList.at(to).push_back(make_pair(from, weight));

            //3 Update weightedDegrees
            weightedDegrees.at(from) += weight;
            weightedDegrees.at(to) += weight;

            //4 update edgeLookup
            pair<T,T> key = makeNomimalEdge(from,to);
            edgeLookup[key] = weight;
        } else {
            throw std::logic_error("Edge already exists");
        }
    }
    void removeEdge(const T& from, const T& to) {
        if(!hasEdge(from, to)) {
            throw std::logic_error("Edge does not exist");
        }
        
        // Get the weight
        const auto key = makeNomimalEdge(from, to);
        const double weight = edgeLookup.at(key);
        
        // 1. Update totalWeight
        totalWeight -= weight;
        
        // 2. Update weightedDegrees
        weightedDegrees.at(from) -= weight;
        weightedDegrees.at(to) -= weight;
        
        // 3. Update edgeLookup
        edgeLookup.erase(key);
        
        // 4. Update adjacencyList
        // Remove 'to' from 'from's neighbors
        auto& fromNeighbors = adjacencyList.at(from);
        fromNeighbors.erase(
            std::remove_if(
                fromNeighbors.begin(), 
                fromNeighbors.end(),
                [&to](const pair<T, double>& edge) {
                    return edge.first == to;
                }
            ),
            fromNeighbors.end()
        );
        
        // Remove 'from' from 'to's neighbors
        auto& toNeighbors = adjacencyList.at(to);
        toNeighbors.erase(
            std::remove_if(
                toNeighbors.begin(), 
                toNeighbors.end(),
                [&from](const pair<T, double>& edge) {
                    return edge.first == from;
                }
            ),
            toNeighbors.end()
        );
    }
    double getEdgeWeight(const T& from, const T& to) const {
        if(!hasEdge(from,to)) {
            throw std::logic_error("Edge does not exist");
        }

        const auto key = makeNomimalEdge(from,to);
        return edgeLookup.at(key);
    }
    size_t getEdgeCount() const { return edgeLookup.size(); }
   
   
    // utilities

    const vector<pair<T,double>>& getNeighbors(const T& vertex) const {
        return adjacencyList.at(vertex);
    }
    // make nominal edge
    pair<T,T> makeNomimalEdge(const T& from, const T& to) const {
        return from < to ? make_pair(from,to) : make_pair(to,from);
    }
    double getTotalWeight() const { return totalWeight; }
    shared_ptr<Graph<T>> createSubGraph(set<T> vertices) {
        shared_ptr<Graph<T>> subGraph = make_shared<Graph<T>>();
        
        for(const auto& vertex: vertices) {
            if(!subGraph->hasVertex(vertex)) {
                subGraph->addVertex(vertex);
            }
          
            const vector<pair<T,double>>& neighbors = getNeighbors(vertex);
            for(const auto& [neighbor, weight] : neighbors) {
                if(vertices.find(neighbor) != vertices.end()) {
                    subGraph->addEdge(vertex, neighbor, weight);
                }
            }
        }

        return subGraph;
        
    }
    void saveToFile(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filename);
        }
        
        // Write the number of vertices
        file << getVertexCount() << endl;
        
        // Write all vertices
        for (const auto& vertex : vertices) {
            file << vertex << " ";
        }
        file << endl;
        
        // Write the number of edges
        file << getEdgeCount() << endl;
        
        // Write all edges with their weights
        for (const auto& [vertexPair, weight] : edgeLookup) {
            file << vertexPair.first << " " << vertexPair.second << " " << weight << endl;
        }
        
        file.close();
    }

};

#endif