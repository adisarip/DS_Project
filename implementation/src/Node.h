

#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;


enum NodeState
{
    STATE_IDLE    =  0, // Node is in IDLE state (can receive any message)
    STATE_ACTIVE  =  1, // Once a message is received it goes into ACTIVE state.
    STATE_UNKNOWN = -1, // Node state while creation.
};

enum MessageType
{
    MSG_CONFIG  = 1, // A configuration message sent by the master process to worker process
    MSG_COMPUTE = 2, // A message sent by a node to another during computation phase
    MSG_TOKEN   = 3, // A token message sent by each process/node to its parent after termination
    MSG_REPEAT  = 4, // A repeat signal is sent by the root node if it receives a BLACK token from one of its child node
};

enum TokenColor
{
    TOKEN_BLACK = 0,
    TOKEN_WHITE = 1,
};

class Node
{
public:
    Node(int nodeId);
    ~Node();

    void setRootNode(int rootNode);
    void setNodeStatus(NodeState nodeStatus);
    void setRouteMap(int* buffer, int size); // will also set the parent node
    void setChildNodes(int* buffer, int size);

    int getNodeId();
    int getRootNode();
    int getParentNode();
    int getNextNode(int destinationNodeId);

protected:
    void runComputations(int seed);

private:
    int mId;
    int mRootNode;
    int mParentNode;
    NodeState mNodeStatus;
    vector<int> mRouteMap;
    vector<int> mChildNodes;
};

#endif // NODE_H
