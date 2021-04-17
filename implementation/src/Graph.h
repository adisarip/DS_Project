
#ifndef GRAPH_H
#define GRAPH_H

// Defining a Graph class
// Features supported:
// 1. Kruskals Algorithm to compute the Minimum Spanning Tree using union-find data structure
// 2. Spanning tree based termination detection

#include <vector>
#include <unordered_set>
#include <string>
using namespace std;

// A subset representation in Union-Find Data Structure
struct Subset
{
    int rank;
    int parent;
};

// Edge of a graph
struct Edge
{
    Edge (){};
    Edge (int _src, int _dest, int _weight):src(_src),dest(_dest),weight(_weight){};
    int src;    // Source Vertex
    int dest;   // Destination Vertex
    int weight; // Edge Weight
};

// Graph Class - Declarations
class Graph
{
public:
    Graph(int nodeCount);
    Graph(string fileName);
    ~Graph();

    // display functions
    void displayMST();
    void displayGraph();
    void displayRoutingTable(int* routeMap1D);

    // getter functions
    int getLeafNodes(int* leafNodes); // return value = the count of leaf nodes
    int getRootNode();
    int getNodesCount();
    vector<int>* getMSTAdjList();

    // Spanning tree functions
    bool isMSTValid();
    void createAndConfigureSpanningTree();
    // Create & fill a message routing array for each proces to send a message to another process
    // (via the spanning tree paths) in O(1) time.
    void fillMessageRoutingTable(int currentNode,
                                 int parentNode,
                                 int* routeMap1D);

protected:
    void addEdge(const int u, const int v, const int w);
    void parseInputData(string fileName);

    // Implementing Union-Find data structure
    // Find the subset of the element with index 'idx'
    // using path-compression technique
    int Find(Subset subsets[], int idx);
    // A function that does union of two sets of x and y
    // by using union by rank
    void Union(Subset subsets[], int x, int y);

private:
    int mNodesCount;
    int mEdgesCount;
    int mRootNode;
    bool mIsMSTValid;
    vector<Edge*> mEdgeList;
    vector<int>* mMSTAdjList;
    unordered_set<int> mNodes;
};

#endif
