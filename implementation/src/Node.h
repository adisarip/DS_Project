

#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;
#define MAX_CHILDREN 10

class Node
{
public:

    enum NodeState
    {
        IDLE,   // Node is in IDLE state (can receive any message)
        ACTIVE, // Once a message is received it goes into ACTIVE state.
    };

    enum MessageType
    {
        MESSAGE, // A message sent by a node to another during computation phase
        TOKEN,   // A token message sent by each process/node to its parent after termination
        REPEAT,  // A repeat signal is sent by the root node if it receives a BLACK token from one of its child node
    };

    enum TokenColor
    {
        WHITE,
        BLACK,
    };

    Node(){};
    ~Node(){};
/*
protected:
    bool isChildTerminated();
    void sendMessage(int srcNodeId,
                     int destNodeId,
                     Message* msg);
    void receiveMessage();
*/
private:
    int mId;
    int mParentId;
    int mChildId[MAX_CHILDREN];
};

#endif // NODE_H
