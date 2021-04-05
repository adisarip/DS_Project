
#include "Graph.h"
#include "GraphUtils.h"
#include <algorithm>
#include <iostream>
using namespace std;


// helper functions for the computations
bool compare(Edge* e1, Edge* e2)
{
    return (e1->weight < e2->weight);
}

// Graph Class - Definitions
// Constructor
Graph::Graph(int nodeCount)
:mNodesCount(nodeCount)
,mEdgesCount(0)
,mMSTSize(0)
,mIsMSTValid(true)
{
}

Graph::~Graph()
{
    // Destructor
    mNodesCount = 0;
    mEdgesCount = 0;
    mMSTSize = 0;
    for(Edge* pEdge : mEdgeList)
    {
        delete pEdge;
    }
}

void Graph::displayMST()
{
    cout << "[INFO] Displaying MST of the graph : " << mMSTSize << " edges." <<  endl;
    for (int i=0; i < mMSTSize; i++)
    {
        cout << "    [" << mMST[i]->src  << "--" << "(" << mMST[i]->weight << ")" << "--" << mMST[i]->dest << "]" << endl;
    }
}

void Graph::addEdge(const int u, const int v, const int w)
{
    // Add edge in the graph
    mEdgeList.push_back(new Edge(u,v,w));
    mNodes.insert(u);
    mNodes.insert(v);
    mNodesCount = (mNodesCount < (int)mNodes.size()) ? mNodes.size() : mNodesCount;
    mEdgesCount++;
}

void Graph::computeMST()
{
    if (mEdgeList.size() == 0)
    {
        mMSTSize = 0;
        mIsMSTValid = false;
        return;
    }

    mMST.resize(mNodesCount);

    sort(mEdgeList.begin(), mEdgeList.end(), compare);

    Subset* subsets = new Subset[(mNodesCount+1) * sizeof(Subset)];

    // create subsets with single elements
    for (int i = 1; i <= mNodesCount; i++)
    {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    // iterate over all edges
    int e_mst = 0; // for iterating over full list of edges
    int j = 0; // for iterating over sorted edges
    while (e_mst < mNodesCount-1 && j < mEdgesCount)
    {
        Edge* pNextEdge = mEdgeList[j++];
        int x = Find(subsets, pNextEdge->src);
        int y = Find(subsets, pNextEdge->dest);
        if (x != y)
        {
            mMST[e_mst++] = pNextEdge;
            Union(subsets, x, y);
        }
    }
    // set the size of MST
    mMSTSize = e_mst;

    // verify for the edges in the MST Union-Find returns the same subset
    // if not then the graph is not connected and MST donot exists
    int sParent = Find(subsets, mMST[0]->src);
    for (int i = 1; i < mMSTSize; i++)
    {
        if (sParent != Find(subsets, mMST[i]->src))
        {
            mIsMSTValid = false;
            break;
        }
    }
}

bool Graph::isMSTValid()
{
    return mIsMSTValid;
}

// Helper functions
