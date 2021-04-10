
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
    Graph(string fileName);
    ~Graph();
    void displayMST();
    void displayGraph();
    void computeMST();
protected:
    void addEdge(const int u, const int v, const int w);
    bool isMSTValid();
    void parseInputData(string fileName);
private:
    int mNodesCount;
    int mEdgesCount;
    bool mIsMSTValid;
    vector<Edge*> mEdgeList;
    vector<Edge*> mMST;
    unordered_set<int> mNodes;
};

#endif
