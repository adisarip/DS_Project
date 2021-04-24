

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

enum Token
{
    TOKEN_BLACK = 0, // A token turns black if the node sends a compute message to any other node.
    TOKEN_WHITE = 1, // A token is white by default
};

struct ComputeMessage
{
    int srcNode;
    int dstNode;
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
    void setComputeMessage(ComputeMessage* pComputeMessage);

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
    void getComputeMessage(ComputeMessage* pComputeMessage);

    void runComputations();

    void displayNodeData();

private:
    int mId;
    int mRootNode;
    Token mToken;
    NodeStatus mNodeStatus;
    ComputeMessage mComputeMessage;
    vector<int> mRouteMap;
    vector<int> mChildNodes;
};

#endif // NODE_H
