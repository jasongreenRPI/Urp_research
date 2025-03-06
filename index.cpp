#include "./CLASSES/Graph/Graph.h"
#include <iostream>

const std::string FILE_NAME = "./DATA/edges_and_weights.txt";
int main() {
    Graph<int> graph = Graph<int>(FILE_NAME);
}