

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include "mpi.h"
#include "Graph.h"
#include "Node.h"
using namespace std;

// Global data
const int MASTER = 0;
const int MAX_MSG_SIZE = 1024;
int gMsgBuffer[MAX_MSG_SIZE];
MPI_Status gMsgStatus;

// Read the data file
// Create the graph and compute the MST
// Compute the Routing table for each process
// send the process node data to each process
// Wait for the root node in the MST to respond after the termination detection algorithm is
// completed.
// Once received, send a kill message to all the nodes.
void configureAndInitiateMasterProcess(string fileName,
                                       int worldSize)
{
    cout << "[INFO] MASTER Process configuring and setting the process(s) environment" << endl;
    Graph sGraph(fileName);
    sGraph.createAndConfigureSpanningTree();
    sGraph.displayMST();

    const int sSize = sGraph.getNodesCount() + 1;
    int sRoutingMap[sSize][sSize];
    fill(*sRoutingMap, (*sRoutingMap + sSize * sSize), -1);

    //int sRootNode = sGraph.getRootNode();
    int sRootNode = 5; // using '5' just for better tree structure with more leaf nodes.
    printf("[INFO] Root Node: %d\n", sRootNode);
    sGraph.fillMessageRoutingTable(sRootNode,
                                   sRootNode,
                                   (int*)sRoutingMap);

    //printf("[INFO] The message rounting is:\n");
    //sGraph.displayRoutingTable((int*)sRoutingMap);

    // sent routing data to all the worker processes
    int adjNodesCount = -1;
    vector<int>* sAdjList = sGraph.getMSTAdjList();
    for (int sDestProcId = 1; sDestProcId < worldSize; sDestProcId++)
    {
        // Send the root node information to each process
        MPI_Send(&sRootNode,
                 1,
                 MPI_INT,
                 sDestProcId,
                 MSG_CONFIG,
                 MPI_COMM_WORLD);

        // send the adjacent nodes list for each process
        memset(gMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        memcpy(gMsgBuffer, sRoutingMap[sDestProcId], sizeof(int) * sSize);
        MPI_Send(gMsgBuffer,
                 sSize,
                 MPI_INT,
                 sDestProcId,
                 MSG_CONFIG,
                 MPI_COMM_WORLD);

        // Send adjacent nodes list for each node
        memset(gMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        adjNodesCount = sAdjList[sDestProcId].size();
        for (int i=0; i < adjNodesCount; i++)
        {
            gMsgBuffer[i] = sAdjList[sDestProcId][i];
        }
        MPI_Send(gMsgBuffer,
                 adjNodesCount,
                 MPI_INT,
                 sDestProcId,
                 MSG_CONFIG,
                 MPI_COMM_WORLD);
    }

    // Now wait for a signal from the root node after the termination detection algorithm is
    // completed
    int msg = 0;
    MPI_Recv(&msg,
             1,
             MPI_INT,
             sRootNode,
             MSG_DONE,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    cout << "[INFO] Termination Detection completed" << endl;
}

// Process Node Intialization
void configureProcessNode(Node* pNode)
{
    // receive the process data from the master process
    // get the root node
    int sRootNode = -1;
    MPI_Recv(&sRootNode,
             1,
             MPI_INT,
             MASTER,
             MSG_CONFIG,
             MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    pNode->setRootNode(sRootNode);

    // get the node/process route map
    int sMsgSize = -1;
    memset(gMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
    MPI_Recv(gMsgBuffer,
             MAX_MSG_SIZE,
             MPI_INT,
             MASTER,
             MSG_CONFIG,
             MPI_COMM_WORLD,
             &gMsgStatus);
    MPI_Get_count(&gMsgStatus, MPI_INT, &sMsgSize);
    pNode->setRouteMap(gMsgBuffer, sMsgSize);

    // get the adj nodes list
    memset(gMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
    MPI_Recv(gMsgBuffer,
             MAX_MSG_SIZE,
             MPI_INT,
             MASTER,
             MSG_CONFIG,
             MPI_COMM_WORLD,
             &gMsgStatus);
    MPI_Get_count(&gMsgStatus, MPI_INT, &sMsgSize);
    pNode->setChildNodes(gMsgBuffer, sMsgSize);
}

ReturnCode initiateTerminationDetection(Node* pNode)
{
    ReturnCode sRc = RC_SUCCESS;
    // 3 types of nodes present
    // Root Node, Internal Node, Leaf Node.
    if (pNode->isLeafNode())
    {
        // starting the termination detection from the leaf nodes
        // sending the token to the immediate parent node
        pNode->setNodeStatus(STATE_IDLE);
        int sToken = pNode->getToken();
        printf("[INFO] LeafNode[%d] intitiating Termination Detection\n", pNode->getNodeId());
        MPI_Send(&sToken,
                 1,
                 MPI_INT,
                 pNode->getParentNode(),
                 MSG_TOKEN,
                 MPI_COMM_WORLD);

        // Now wait if there is a REPEAT request for re-initiating termination detection
        // or any COMPUTE messages sent by any other process.
    }
    else if (pNode->isRootNode())
    {
        // wait for all the child nodes to respond with termination token messages
        // once received all ... then check if any black token is received
        // If all are white then inform the master process that the termination detection is complete.
        // If any black token is received then send a REPEAT message to all the child nodes to re-inititate
        // the termination detection algorithm.
        int sNodeCount = pNode->getChildNodesCount();
        while (sNodeCount > 0)
        {
            memset(gMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
            MPI_Recv(gMsgBuffer,
                     MAX_MSG_SIZE,
                     MPI_INT,
                     MPI_ANY_SOURCE,
                     MPI_ANY_TAG,
                     MPI_COMM_WORLD,
                     &gMsgStatus);
            if (MSG_TOKEN == gMsgStatus.MPI_TAG)
            {
                sNodeCount--;
            }
        }
        // inform master process about the completion
        printf("[INFO] RootNode[%d] Received all tokens from child nodes.\n", pNode->getNodeId());
        int msg = 0;
        MPI_Send(&msg,
                 1,
                 MPI_INT,
                 MASTER,
                 MSG_DONE,
                 MPI_COMM_WORLD);
    }
    else // Internal Node
    {
        int sNodeCount = pNode->getChildNodesCount();
        while (sNodeCount > 0)
        {
            memset(gMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
            MPI_Recv(gMsgBuffer,
                     MAX_MSG_SIZE,
                     MPI_INT,
                     MPI_ANY_SOURCE,
                     MPI_ANY_TAG,
                     MPI_COMM_WORLD,
                     &gMsgStatus);
            if (MSG_TOKEN == gMsgStatus.MPI_TAG)
            {
                pNode->setToken((Token)gMsgBuffer[0]);
                sNodeCount--;
            }
        }
        printf("[INFO] InternalNode[%d] Received all tokens from child nodes. Sending token to ParentNode[%d]\n",
                pNode->getNodeId(), pNode->getParentNode());
        // forward the token to parent node
        int sToken = pNode->getToken();
        MPI_Send(&sToken,
                 1,
                 MPI_INT,
                 pNode->getParentNode(),
                 MSG_TOKEN,
                 MPI_COMM_WORLD);
    }
    return sRc;
}

int main (int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int sProcId;
    MPI_Comm_rank(MPI_COMM_WORLD, &sProcId);
    int sWorldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &sWorldSize);

    // Master / Manager process to perform initial config operations
    if (MASTER == sProcId)
    {
        string sFileName = string(argv[1]);
        configureAndInitiateMasterProcess(sFileName, sWorldSize);
    }
    else
    {
        Node sNode(sProcId);
        configureProcessNode(&sNode);

        // set node status and token type
        sNode.setNodeStatus(STATE_ACTIVE);
        sNode.setToken(TOKEN_WHITE);

        // run node/process specific computations
        sNode.runComputations();
        printf("[INFO] Process#%d is done with internal computations\n", sProcId);

        // post computations initiate the termination detection protocol
        initiateTerminationDetection(&sNode);
    }

    MPI_Finalize();
}
