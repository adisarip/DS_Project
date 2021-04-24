

#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;


enum NodeStatus
{
    STATE_IDLE    =  0, // Node is in IDLE state (can receive any message)
    STATE_ACTIVE  =  1, // Once a message is received it goes into ACTIVE state.
    STATE_UNKNOWN = -1, // Node state while creation.
};

enum MessageType
{
    MSG_CONFIG  = 1, // A configuration message sent by the master process to worker process
    MSG_DONE    = 2, // A done will be sent by the root node once it confirms that all the childs have terminated
    MSG_KILL    = 3, // A kill message is sent by the master process to a worker process to stop and exit
    MSG_COMPUTE = 4, // A message sent by a node to another during computation phase
    MSG_TOKEN   = 5, // A token message sent by each process/node to its parent after termination
    MSG_REPEAT  = 6, // A repeat signal is sent by the root node if it receives a BLACK token from one of its child node
};

enum Token
{
    TOKEN_BLACK = 0, // A token turns black if the node sends a compute message to any other node.
    TOKEN_WHITE = 1, // A token is white by default
};

enum ReturnCode
{
    RC_SUCCESS = 0,
    RC_FAILURE = 1,
    RC_RETRY   = 2,
};

class Node
{
public:
    Node(int nodeId);
    ~Node();

    void setRootNode(int rootNode);
    void setRouteMap(int* buffer, int size);
    // RouteMap should be set before this function call
    void setChildNodes(int* buffer, int size);
    void setNodeStatus(NodeStatus nodeStatus);
    void setToken(Token token);

    int getNodeId();
    int getRootNode();
    int getParentNode();
    int getNextNode(int destinationNodeId);
    int getChildNodesCount();
    void getChildNodes(vector<int>* pChildNodes);

    bool isLeafNode();
    bool isRootNode();

    Token getToken();
    NodeStatus getNodeStatus();

    void runComputations();

    void displayNodeData();

private:
    int mId;
    int mRootNode;
    Token mToken;
    NodeStatus mNodeStatus;
    vector<int> mRouteMap;
    vector<int> mChildNodes;
};

#endif // NODE_H
