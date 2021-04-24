
// File implementing the Termination detection algorithm
// Declarations

#ifndef TERMINATIONDETECTION_H
#define TERMINATIONDETECTION_H

// forward declarations
class Node;

// enumerations
enum ReturnCode
{
    RC_SUCCESS = 0,
    RC_FAILURE = 1,
    RC_RETRY   = 2,
};

enum MessageType
{
    // Messages used between the master and worker processes for management purpose
    MSG_CONFIG  = 1, // A configuration message sent by the master process to worker process
    MSG_DONE    = 2, // A done will be sent by the root node once it confirms that all the childs have terminated
    MSG_KILL    = 3, // A kill message is sent by the master process to a worker process to stop and exit

    // Messages used in termination detection algorithm
    MSG_COMPUTE = 4, // A message sent by a node to another during computation phase
    MSG_TOKEN   = 5, // A token message sent by each process/node to its parent after termination
    MSG_REPEAT  = 6, // A repeat signal is sent by the root node if it receives a BLACK token from one of its child node
};

// Global data
const int MASTER = 0;
const int MAX_MSG_SIZE = 1024;

void configureAndInitiateMasterProcess(std::string fileName, int worldSize);
void configureAndInitiateNodeProcess(int procId);
void configureNodeProcess(Node* pNode);
ReturnCode initiateRootNodeProcess(Node* pNode);
ReturnCode initiateInternalNodeProcess(Node* pNode);
ReturnCode initiateTerminationDetection(Node* pNode);

#endif // TERMINATIONDETECTION_H
