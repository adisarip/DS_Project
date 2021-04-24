


#include "Node.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>
using namespace std;


Node::Node(int nodeId)
:mId(nodeId)
,mRootNode(-1)
,mToken(TOKEN_WHITE)
,mNodeStatus(STATE_UNKNOWN)
{
}

Node::~Node()
{
}

void Node::setRootNode(int rootNode)
{
    mRootNode = rootNode;
}

void Node::setNodeStatus(NodeStatus nodeStatus)
{
    mNodeStatus = nodeStatus;
}

void Node::setToken(Token token)
{
    mToken = (Token)token;
}

void Node::setRouteMap(int* buffer, int size)
{
    for (int i=0; i<size; i++)
    {
        mRouteMap.push_back(buffer[i]);
    }
}

void Node::setChildNodes(int* buffer, int size)
{
    int sParentNode = getParentNode();
    for (int i=0; i<size; i++)
    {
        if (buffer[i] != sParentNode)
        {
            mChildNodes.push_back(buffer[i]);
        }
    }
}

int Node::getNodeId()
{
    return mId;
}

int Node::getRootNode()
{
    return mRootNode;
}

int Node::getParentNode()
{
    return mRouteMap[mRootNode];
}

int Node::getNextNode(int destinationNodeId)
{
    return mRouteMap[destinationNodeId];
}

int Node::getChildNodesCount()
{
    return mChildNodes.size();
}

void Node::getChildNodes(vector<int>* pChildNodes)
{
    for (int child : mChildNodes)
    {
        pChildNodes->push_back(child);
    }
}

bool Node::isLeafNode()
{
    return (mChildNodes.size() == 0);
}

bool Node::isRootNode()
{
    return (mRootNode == mId);
}

Token Node::getToken()
{
    return mToken;
}

NodeStatus Node::getNodeStatus()
{
    return mNodeStatus;
}

// protected member Definitions

void Node::runComputations()
{
    // These computations have a run time between 1 sec to 5 secs.
    double x;
    srand(time(0));
    int limit = rand() % (40) + 110;
    for (int i = 0; i < limit; i++)
    {
        for (int j = 0; j < limit; j++)
        {
            for(int k = 0; k < limit; k++)
            {
                for (int l = 0; l < limit; l++)
                {
                    x = pow(-1,l) * (limit * l);
                }
                x =pow(-1,k) * (limit * k);
            }
            x = pow(-1,j) * (limit * j);
        }
        x = pow(-1,i) * (limit * i);
    }
    x = x-1;
    // wasted enough time ... get going with the algorithm now !!!
}

void Node::displayNodeData()
{
    printf("[INFO] Node:%d Root:%d Status:%d Token:%d ChildCount:%d\n",
           mId, mRootNode, (int)mNodeStatus, (int)mToken, (int)mChildNodes.size());
}
