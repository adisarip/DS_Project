
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

// Global data
const int MASTER = 0;
const int MAX_MSG_SIZE = 1024;

void configureAndInitiateMasterProcess(std::string fileName, int worldSize);
void configureAndInitiateNodeProcess(int procId);
void configureProcessNode(Node* pNode);
ReturnCode initiateRootNodeProcess(Node* pNode);
ReturnCode initiateInternalNodeProcess(Node* pNode);
ReturnCode initiateTerminationDetection(Node* pNode);

#endif // TERMINATIONDETECTION_H
