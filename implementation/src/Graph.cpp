
#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
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
,mRootNode(-1)
,mIsMSTValid(true)
{
}

Graph::Graph(string fileName)
:mNodesCount(0)
,mEdgesCount(0)
,mRootNode(-1)
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

void Graph::displayMST()
{
    cout << "[INFO] Displaying MST of the given graph:" <<  endl;
    for (int i=1; i <= mNodesCount; i++)
    {
        int row_size = mMSTAdjList[i].size();
        cout << i << " -> " << flush;
        for (int j=0; j < row_size; j++)
        {
            if (j == row_size-1)
            {
                cout << mMSTAdjList[i][j] << endl;
            }
            else
            {
                cout << mMSTAdjList[i][j] << "," << flush;
            }
        }
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

void Graph::displayRoutingTable(int* routeMap1D)
{
    int size = mNodesCount + 1;
    // typecasting 1-D array to 2-D array representation for ease of use
    int (*sRouteMap)[size] = (int (*)[size])&routeMap1D[0];
    // printing the routing table
    for (int i=1; i < size; i++)
    {
        cout << setw(2) << i << " | " << flush;
        for (int j=1; j < size; j++)
        {
            cout << setw(2) << sRouteMap[i][j] << " " << flush;
        }
        cout << endl;
    }
}

void Graph::createAndConfigureSpanningTree()
{
    if (mEdgeList.size() == 0)
    {
        mIsMSTValid = false;
        return;
    }

    mMSTAdjList = new vector<int>[mNodesCount+1];
    vector<Edge*> sMST;

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
            sMST.push_back(pNextEdge);
            Union(subsets, x, y);
            mst_size++;
            // create the adjacency list of the MST
            mMSTAdjList[pNextEdge->src].push_back(pNextEdge->dest);
            mMSTAdjList[pNextEdge->dest].push_back(pNextEdge->src);
        }
    }

    // verify for the edges in the MST Union-Find returns the same subset
    // if not then the graph is not connected and MST donot exists
    mRootNode = Find(subsets, sMST[0]->src);
    for (int i = 1; i < (int)sMST.size(); i++)
    {
        if (mRootNode != Find(subsets, sMST[i]->src))
        {
            mIsMSTValid = false;
            break;
        }
    }
}

void Graph::fillMessageRoutingTable(int currentNode,
                                    int parentNode,
                                    int* routeMap1D)
{
    int size = mNodesCount + 1;
    // typecasting 1-D array to 2-D array representation for ease of use
    int (*sRouteMap)[size] = (int (*)[size])&routeMap1D[0];

    // check if the current node is leaf-node
    if (mMSTAdjList[currentNode].size() == 1)
    {
        fill(sRouteMap[currentNode]+1, (sRouteMap[currentNode] + size), parentNode);
        sRouteMap[currentNode][currentNode] = currentNode;
        return;
    }
    else
    {
        if (currentNode == parentNode)
        {
            sRouteMap[currentNode][currentNode] = currentNode;
        }
        else
        {
            for (int i=1; i <= mNodesCount; i++)
            {
                if (i == currentNode)
                {
                    sRouteMap[currentNode][i] = currentNode;
                }
                else if (sRouteMap[parentNode][i] != -1)
                {
                    sRouteMap[currentNode][i] = parentNode;
                }
            }
        }

        // iterate over the child nodes of current node and fill the child node arrays

        for (int i=0; i < (int)mMSTAdjList[currentNode].size(); i++)
        {
            int childNode = mMSTAdjList[currentNode][i];
            // skip the parent node from the adjacency list of the current node
            // to only traverse the child nodes
            if (childNode != parentNode)
            {
                // recursive call to update the child node arrays
                fillMessageRoutingTable(childNode,
                                        currentNode,
                                        routeMap1D);

                sRouteMap[currentNode][childNode] = childNode;

                for (int j=1; j <= mNodesCount; j++)
                {
                    if (sRouteMap[currentNode][j] == -1 &&
                        sRouteMap[childNode][j] != -1   &&
                        sRouteMap[childNode][j] != currentNode)
                    {
                        sRouteMap[currentNode][j] = childNode;
                    }
                }
            }
        }

        // fill the leftover array for the current node
        for (int i=1; i <= mNodesCount; i++)
        {
            if (sRouteMap[currentNode][i] == -1)
            {
                sRouteMap[currentNode][i] = parentNode;
            }
            sRouteMap[i][i] = -1;
        }
    }
}

bool Graph::isMSTValid()
{
    return mIsMSTValid;
}

int Graph::getNodesCount()
{
    return mNodesCount;
}

int Graph::getRootNode()
{
    return mRootNode;
}

vector<int>* Graph::getMSTAdjList()
{
    return mMSTAdjList;
}

int Graph::getLeafNodes(int* leafNodes)
{
    int sLeafNodesCount = 0;
    for (int i=0; i < mNodesCount; i++)
    {
        if (mMSTAdjList[i].size() == 1)
        {
            // leaf node
            leafNodes[sLeafNodesCount++] = mMSTAdjList[i][0];
        }
    }
    return sLeafNodesCount;
}

// Protected Helper functions - Definitions

void Graph::addEdge(const int u, const int v, const int w)
{
    // Add edge in the graph
    mEdgeList.push_back(new Edge(u,v,w));
    mNodes.insert(u);
    mNodes.insert(v);
    mNodesCount = (mNodesCount < (int)mNodes.size()) ? mNodes.size() : mNodesCount;
    mEdgesCount++;
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

int Graph::Find(Subset subsets[], int idx)
{
    // find the root and make root as parent of 'idx'
    if (subsets[idx].parent != idx)
    {
        subsets[idx].parent = Find(subsets, subsets[idx].parent);
    }
    return subsets[idx].parent;
}

void Graph::Union(Subset subsets[], int x, int y)
{
    int x_root = Find(subsets, x);
    int y_root = Find(subsets, y);

    if (subsets[x_root].rank < subsets[y_root].rank)
    {
        subsets[x_root].parent = y_root;
    }
    else if (subsets[x_root].rank > subsets[y_root].rank)
    {
        subsets[y_root].parent = x_root;
    }
    else
    {
        // If ranks are same, then make one as root and
        // increment its rank by one
        subsets[y_root].parent = x_root;
        subsets[x_root].rank++;
    }
}
