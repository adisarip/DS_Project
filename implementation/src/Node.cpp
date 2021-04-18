


#include "Node.h"
using namespace std;


Node::Node(int nodeId)
:mId(nodeId)
,mRootNode(-1)
,mParentNode(-1)
,mNodeStatus(STATE_UNKNOWN)
{
}

Node::~Node()
{
}

void Node::setRootNode(int rootNode)
{
}

void Node::setNodeStatus(NodeState nodeStatus)
{
}

void Node::setRouteMap(int* buffer, int size)
{
}

void Node::setChildNodes(int* buffer, int size)
{
}

int Node::getNodeId()
{
    return -1;
}

int Node::getRootNode()
{
    return -1;
}

int Node::getParentNode()
{
    return -1;
}

int Node::getNextNode(int destinationNodeId)
{
    return -1;
}

// protected member Definitions

void Node::runComputations(int seed)
{
}
