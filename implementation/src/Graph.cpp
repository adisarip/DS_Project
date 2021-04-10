
#include "Graph.h"
#include "GraphUtils.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
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
,mIsMSTValid(true)
{
}

Graph::Graph(string fileName)
:mNodesCount(0)
,mEdgesCount(0)
,mIsMSTValid(true)
{
    parseInputData(fileName);
}

Graph::~Graph()
{
    // Destructor
    mNodesCount = 0;
    mEdgesCount = 0;
    for(Edge* pEdge : mEdgeList)
    {
        delete pEdge;
    }
}

void Graph::parseInputData(string fileName)
{
    fstream sFile(fileName, ios::in);
    string sLine;
    int sLineCount = 0;
    while (getline(sFile, sLine))
    {
        // skip the comment lines
        if (sLine[0] == '#') continue;

        sLineCount++;
        istringstream iss(sLine);
        if (sLineCount == 1)
        {
            // No of nodes
            iss >> mNodesCount;
            continue;
        }

        int u, v, w;
        iss >> u >> v >> w;
        addEdge(u, v, w);
    }
}

void Graph::displayGraph()
{
    cout << "[INFO] Displaying the input graph : " <<  endl;
    for (int i=0; i < (int)mEdgeList.size(); i++)
    {
        cout << "    [" << mEdgeList[i]->src  << "--" << "(" << mEdgeList[i]->weight << ")" << "--"
             << mEdgeList[i]->dest << "]" << endl;
    }
}

void Graph::displayMST()
{
    cout << "[INFO] Displaying MST of the graph : " << mMST.size() << " edges." <<  endl;
    for (int i=0; i < (int)mMST.size(); i++)
    {
        cout << "    [" << mMST[i]->src  << "--" << "(" << mMST[i]->weight << ")" << "--"
             << mMST[i]->dest << "]" << endl;
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
        mIsMSTValid = false;
        return;
    }

    //mMST.resize(mNodesCount);

    sort(mEdgeList.begin(), mEdgeList.end(), compare);

    Subset* subsets = new Subset[(mNodesCount+1) * sizeof(Subset)];

    // create subsets with single elements
    for (int i = 1; i <= mNodesCount; i++)
    {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    // iterate over all edges
    int mst_size = 0; // for iterating over full list of edges
    int j = 0; // for iterating over sorted edges
    while (mst_size < mNodesCount-1 && j < mEdgesCount)
    {
        Edge* pNextEdge = mEdgeList[j++];
        int x = Find(subsets, pNextEdge->src);
        int y = Find(subsets, pNextEdge->dest);
        if (x != y)
        {
            //mMST[e_mst++] = pNextEdge;
            mMST.push_back(pNextEdge);
            Union(subsets, x, y);
            mst_size++;
        }
    }

    // verify for the edges in the MST Union-Find returns the same subset
    // if not then the graph is not connected and MST donot exists
    int sParent = Find(subsets, mMST[0]->src);
    for (int i = 1; i < (int)mMST.size(); i++)
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
