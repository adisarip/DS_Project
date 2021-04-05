
#ifndef GRAPH_H
#define GRAPH_H

// Defining a Graph class
// Features supported:
// 1. Kruskals Algorithm to compute the Minimum Spanning Tree using union-find data structure
// 2. Spanning tree based termination detection

#include <vector>
#include <unordered_set>
using namespace std;

// Graph Class - Declarations
struct Edge
{
    Edge (){};
    Edge (int _src, int _dest, int _weight):src(_src),dest(_dest),weight(_weight){};
    int src;    // Source Vertex
    int dest;   // Destination Vertex
    int weight; // Edge Weight
};

class Graph
{
public:
    Graph(int nodeCount);
    ~Graph();
    void addEdge(const int u, const int v, const int w);
    void displayMST();
    void computeMST();
    bool isMSTValid();
private:
    int mNodesCount;
    int mEdgesCount;
    int mMSTSize;
    bool mIsMSTValid;
    vector<Edge*> mEdgeList;
    vector<Edge*> mMST;
    unordered_set<int> mNodes;
};

#endif
