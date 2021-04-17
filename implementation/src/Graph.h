
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
    void displayMST();
    void displayGraph();
    void createAndConfigureSpanningTree();
    int getLeafNodes(int* leafNodes); // return value = the count of leaf nodes
    int getRootNode();
protected:
    void addEdge(const int u, const int v, const int w);
    bool isMSTValid();
    void parseInputData(string fileName);
private:
    int mNodesCount;
    int mEdgesCount;
    bool mIsMSTValid;
    vector<Edge*> mEdgeList;
    vector<int>* mMSTAdjList;
    unordered_set<int> mNodes;
};

#endif
