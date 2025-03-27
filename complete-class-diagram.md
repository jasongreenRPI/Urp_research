# Network Analytics C++ Implementation Structure

## Complete Class Diagram with Explicit Template Usage

```
┌───────────────────────────────────────────────────────────┐
│ Graph<T>                                                  │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - adjacencyList: unordered_map<T, vector<pair<T, double>>>│
│ - totalWeight: double                                     │
│ - weightedDegrees: unordered_map<T, double>               │
│ - edgeLookup: unordered_map<pair<T,T>, double, PairHash<T>>│
│ - vertices: set<T>                                        │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + Graph()                                                 │
│ + Graph(string filename)                                  │
│ + addVertex(T vertex)                                     │
│ + removeVertex(T vertex)                                  │
│ + addEdge(T from, T to, double weight)                    │
│ + removeEdge(T from, T to)                                │
│ + hasVertex(T vertex) -> bool                             │
│ + hasEdge(T from, T to) -> bool                           │
│ + getEdgeWeight(T from, T to) -> double                   │
│ + getNeighbors(T vertex) -> vector<pair<T, double>>       │
│ + getVertices() -> const set<T>&                          │
│ + getVertexCount() -> size_t                              │
│ + getEdgeCount() -> size_t                                │
│ + getTotalWeight() -> double                              │
│ + getDegree(T vertex) -> int                              │
│ + getWeightedDegree(T vertex) -> double                   │
│ + createSubgraph(set<T> vertices) -> shared_ptr<Graph<T>> │
│ + saveToFile(string filename)                             │
│ + calculateModularity(const vector<Community<T>>&) -> double│
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ Community<T>                                              │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - nodes: set<T>                                           │
│ - internalWeight: double                                  │
│ - externalWeight: double                                  │
│ - cachedGraphRef: shared_ptr<Graph<T>>                    │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + Community()                                             │
│ + addNode(const T& node)                                  │
│ + removeNode(const T& node)                               │
│ + containsNode(const T& node) -> bool                     │
│ + size() -> size_t                                        │
│ + getNodes() -> const set<T>&                             │
│ + getNodesSorted() -> vector<T>                           │
│ + merge(Community<T>& other)                              │
│ + createUnion(const Community<T>& other) -> Community<T>  │
│ + createIntersection(const Community<T>& other) -> Community<T>│
│ + operator==(const Community<T>& other) -> bool           │
│ + calculateWeights(shared_ptr<Graph<T>>& graph)           │
│ + getInternalWeight() -> double                           │
│ + getExternalWeight() -> double                           │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ CommunityComparison<T>                                    │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - labelsTrue: vector<int>                                 │
│ - labelsPred: vector<int>                                 │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + loadCommunities(string filename)                        │
│   -> vector<Community<T>>                                 │
│ + getNodeSet(const vector<Community<T>>& communities)     │
│   -> set<T>                                               │
│ + handleMissingNodes(vector<Community<T>>& readComms,     │
│                    const set<T>& actualNodes,             │
│                    const set<T>& predNodes)               │
│ + calculateNMI(const vector<Community<T>>& trueCommunities,│
│               const vector<Community<T>>& predCommunities)│
│   -> double                                               │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ DataPreparation<T>                                        │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - nodes: vector<T>                                        │
│ - totalDegree: int                                        │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + findNumberOfUniqueNodesAndTotalDegree(                  │
│     string networkEdgesFile,                              │
│     string networkGroups)                                 │
│   -> pair<vector<T>, int>                                 │
│ + readGroundTruthCommunities(                             │
│     const vector<T>& nodes,                               │
│     string networkEdgesFile,                              │
│     string networkGroups,                                 │
│     string manual_auto,                                   │
│     string manualHierarchyFile)                           │
│   -> tuple<int, vector<Community<T>>,                     │
│           vector<Community<T>>,                           │
│           vector<Community<T>>>                           │
│ + findEdgesBetweenGroups(                                 │
│     const vector<Community<T>>& communities,              │
│     const vector<T>& nodes,                               │
│     string networkEdgesFile)                              │
│   -> map<pair<int,int>, vector<double>>                   │
│ + calculatePreference(                                    │
│     const vector<Community<T>>& hierarchicalCommunities,  │
│     string networkEdgesFile)                              │
│ + manualEdgesBetweenGroups(                               │
│     const vector<Community<T>>& communities,              │
│     const vector<T>& nodes,                               │
│     string manualEdgesFile,                               │
│     string manualPreferencesFile)                         │
│   -> map<T, vector<pair<T, int>>>                         │
│ + findEdgesToOtherNodes(                                  │
│     string networkEdgesFile,                              │
│     const vector<T>& nodes)                               │
│   -> map<T, vector<pair<T, int>>>                         │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ NetworkGenerator                                          │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - dataPrep: unique_ptr<DataPreparation<string>>           │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + BWRN(map<string, vector<pair<string, int>>>& nodeEdges, │
│      vector<Community<string>>& hierarchicalCommunities,  │
│      double probabilityOfSuccess)                         │
│   -> vector<tuple<string, string, int>>                   │
│ + WRG(map<string, vector<pair<string, int>>>& nodeEdges,  │
│     vector<Community<string>>& hierarchicalCommunities)   │
│   -> vector<tuple<string, string, int>>                   │
│ + randomizeNodeIds(                                       │
│     vector<tuple<string, string, int>>& totalGeneratedEdges,│
│     string randomNodeIdsFile,                             │
│     int numNodes)                                         │
│   -> vector<pair<string, string>>                         │
│ + convertNetworkForBWRN(string inputFile, string outputFile)│
│ + convertCommunityForBWRN(string inputFile, string outputFile)│
│ + generateNetworks(string version, string model,          │
│                  double probability, string randomization,│
│                  vector<string>& args)                    │
│ + generateGraph(vector<tuple<string, string, int>>& edges)│
│   -> shared_ptr<Graph<string>>                            │
│ + parseArguments(int argc, char* argv[])                  │
│   -> map<string, string>                                  │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ FreqFracHelper<T>                                         │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - networksGroupsDict: map<Community<T>, double>           │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + readOriginalCommunityNodes(string nodesOriginalCommunities)│
│   -> vector<T>                                            │
│ + readNetworkGroups(string networkGroupsFile,             │
│                   const vector<T>& listOfNodesFile)       │
│   -> vector<Community<T>>                                 │
│ + computeStructurePenalty(                                │
│     const vector<Community<T>>& predictedCommunityStructure,│
│     const map<Community<T>, double>& networksGroupsDict,  │
│     const vector<T>& nodes,                               │
│     string freqFrac,                                      │
│     const vector<T>& changedNodes)                        │
│   -> vector<double>                                       │
│ + findPenaltyForEachNode(                                 │
│     const map<Community<T>, double>& networksGroupsDict,  │
│     const vector<T>& nodes,                               │
│     string freqFrac)                                      │
│   -> vector<double>                                       │
│ + freqFracBasedPredictions(                               │
│     const map<Community<T>, double>& networksGroupsDict,  │
│     const vector<T>& nodes,                               │
│     string freqFrac)                                      │
│   -> pair<vector<Community<T>>, string>                   │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ FreqFracCommunities                                       │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - originalNetworkNodes: vector<string>                    │
│ - networksGroupsDict: map<Community<string>, double>      │
│ - helper: unique_ptr<FreqFracHelper<string>>              │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + FreqFracCommunities(string nodesOriginalCommunitiesFile,│
│                     string pathToGeneratedGroupsFiles,    │
│                     int numOfFiles,                       │
│                     string nameOfFiles)                   │
│ + processCommunities(string outputPrefix)                 │
│ + generateFrequencyBasedCommunities()                     │
│   -> pair<vector<Community<string>>, string>              │
│ + generateFractionBasedCommunities()                      │
│   -> pair<vector<Community<string>>, string>              │
│ + writeCommunities(const vector<Community<string>>& communities,│
│                  string filename)                         │
│ + writeScore(string score, string filename)               │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ LouvainDetection                                          │
├───────────────────────────────────────────────────────────┤
│ Data Members:                                             │
│ - graph: shared_ptr<Graph<string>>                        │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + LouvainDetection(shared_ptr<Graph<string>> g)           │
│ + detectCommunities(string edgeListFile)                  │
│   -> vector<Community<string>>                            │
│ + detectCommunitiesFromGraph(shared_ptr<Graph<string>> g) │
│   -> vector<Community<string>>                            │
│ + writeCommunities(const vector<Community<string>>& communities,│
│                  string outputFile)                       │
│ + batchProcess(const vector<string>& inputFiles,          │
│              const vector<string>& outputFiles)           │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ FileHandler                                               │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + readSpaceSeparatedValues(string line)                   │
│   -> vector<string>                                       │
│ + readCommunityFile(string filename)                      │
│   -> vector<Community<string>>                            │
│ + writeCommunityFile(const vector<Community<string>>& communities,│
│                    string filename)                       │
│ + readEdgeList(string filename)                           │
│   -> vector<tuple<string, string, double>>                │
│ + writeEdgeList(                                          │
│     const vector<tuple<string, string, double>>& edges,   │
│     string filename)                                      │
│ + convertVectorToCommunitiesString(vector<Community<string>>)│
│   -> vector<vector<string>>                               │
│ + convertVectorStringToCommunities(vector<vector<string>>)│
│   -> vector<Community<string>>                            │
└───────────────────────────────────────────────────────────┘
                           ▲
                           │
                           │ uses
                           │
┌───────────────────────────────────────────────────────────┐
│ Metrics                                                   │
├───────────────────────────────────────────────────────────┤
│ Methods:                                                  │
│ + normalizedMutualInfo(const vector<int>& labels_true,    │
│                      const vector<int>& labels_pred)      │
│   -> double                                               │
│ + communityToLabels(const vector<Community<string>>& communities)│
│   -> vector<int>                                          │
│ + shannonEntropy(const vector<double>& probabilities)     │
│   -> double                                               │
└───────────────────────────────────────────────────────────┘
```

## Implementation Notes

1. **Template Strategy**:
   - Core classes (Graph<T>, Community<T>) are fully templated
   - Algorithm classes (CommunityComparison<T>, DataPreparation<T>, FreqFracHelper<T>) are templated
   - Application classes (NetworkGenerator, FreqFracCommunities, LouvainDetection) use concrete types (string)

2. **Type Consistency**:
   - All methods explicitly use Community<T> and Graph<T> where appropriate
   - All container types properly reflect the template parameter T
   - Higher-level classes use Community<string> concrete type

3. **Conversion Utilities**:
   - FileHandler includes methods to convert between raw vectors and Community objects
   - Metrics includes a method to convert communities to label vectors

4. **Parameter Passing**:
   - Large objects are passed by const reference
   - Return values use move semantics where appropriate

This design explicitly uses the Graph<T> and Community<T> classes throughout the class hierarchy while providing a practical implementation approach with concrete types at the application level.
