

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "mpi.h"
#include "Graph.h"
#include "Node.h"
#include "TerminationDetection.h"
using namespace std;

void getRandomSourceDestinationPair(ComputeMessage* pMessage, int worldSize)
{
    srand(time(0));
    pMessage->srcNode = rand() % (worldSize) + 1;
    pMessage->dstNode = rand() % (worldSize) + 1;
}

// Read the data file
// Create the graph and compute the MST
// Compute the Routing table for each process
// send the process node data to each process
// Wait for the root node in the MST to respond after the termination detection algorithm is
// completed.
// Once received, send a kill message to all the nodes.
void configureAndInitiateMasterProcess(string fileName, int worldSize)
{
    printf("[INFO] MASTER Process configuring and setting the process(s) environment\n");
    fflush(stdout);
    Graph sGraph(fileName);
    sGraph.createAndConfigureSpanningTree();
    sGraph.displayMST();

    const int sSize = sGraph.getNodesCount() + 1;
    int sRoutingMap[sSize][sSize];
    fill(*sRoutingMap, (*sRoutingMap + sSize * sSize), -1);

    int sRootNode = sGraph.getRootNode();
    printf("[INFO] Root Node: %d\n", sRootNode);
    fflush(stdout);
    sGraph.fillMessageRoutingTable(sRootNode,
                                   sRootNode,
                                   (int*)sRoutingMap);

    int sMsgBuffer[MAX_MSG_SIZE];
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
        memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        memcpy(sMsgBuffer, sRoutingMap[sDestProcId], sizeof(int) * sSize);
        MPI_Send(sMsgBuffer,
                 sSize,
                 MPI_INT,
                 sDestProcId,
                 MSG_CONFIG,
                 MPI_COMM_WORLD);

        // Send adjacent nodes list for each node
        memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        adjNodesCount = sAdjList[sDestProcId].size();
        for (int i=0; i < adjNodesCount; i++)
        {
            sMsgBuffer[i] = sAdjList[sDestProcId][i];
        }
        MPI_Send(sMsgBuffer,
                 adjNodesCount,
                 MPI_INT,
                 sDestProcId,
                 MSG_CONFIG,
                 MPI_COMM_WORLD);

        // Randomly generate a source-destination node pair for sending a COMPUTE message.
        ComputeMessage sComputeMessage;
        getRandomSourceDestinationPair(&sComputeMessage, worldSize);
        memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        sMsgBuffer[0] = sComputeMessage.srcNode;
        sMsgBuffer[1] = sComputeMessage.dstNode;
        MPI_Send(sMsgBuffer,
                 2,
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

    // Gracefully terminating all the worker processes
    for (int sDestProcId = 1; sDestProcId < worldSize; sDestProcId++)
    {
        int msg = 0;
        MPI_Send(&msg,
                 1,
                 MPI_INT,
                 sDestProcId,
                 MSG_KILL,
                 MPI_COMM_WORLD);
    }

    printf("[INFO] Termination Detection completed\n");
    fflush(stdout);
}


// Process Node Intialization
void configureNodeProcess(Node* pNode)
{
    int sMsgBuffer[MAX_MSG_SIZE];
    MPI_Status sMsgStatus;

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
    memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
    MPI_Recv(sMsgBuffer,
             MAX_MSG_SIZE,
             MPI_INT,
             MASTER,
             MSG_CONFIG,
             MPI_COMM_WORLD,
             &sMsgStatus);
    MPI_Get_count(&sMsgStatus, MPI_INT, &sMsgSize);
    pNode->setRouteMap(sMsgBuffer, sMsgSize);

    // get the adj nodes list
    memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
    MPI_Recv(sMsgBuffer,
             MAX_MSG_SIZE,
             MPI_INT,
             MASTER,
             MSG_CONFIG,
             MPI_COMM_WORLD,
             &sMsgStatus);
    MPI_Get_count(&sMsgStatus, MPI_INT, &sMsgSize);
    pNode->setChildNodes(sMsgBuffer, sMsgSize);

    // set the random compute message details
    memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
    MPI_Recv(sMsgBuffer,
             MAX_MSG_SIZE,
             MPI_INT,
             MASTER,
             MSG_CONFIG,
             MPI_COMM_WORLD,
             &sMsgStatus);
    if (pNode->getNodeId() == sMsgBuffer[0])
    {
        ComputeMessage sMsg;
        sMsg.srcNode = sMsgBuffer[0];
        sMsg.dstNode = sMsgBuffer[1];
        pNode->setComputeMessage(&sMsg);
    }
}

// Create a Node object for each node process, and save the relevant data in it.
void configureAndInitiateNodeProcess(int procId)
{
    Node sNode(procId);
    configureNodeProcess(&sNode);

    // set node status and token type
    sNode.setNodeStatus(STATE_ACTIVE);
    sNode.setToken(TOKEN_WHITE);

    // run node/process specific computations
    sNode.runComputations();
    // A COMPUTE message is sent as part of the above computations
    ComputeMessage sCMsg;
    sNode.getComputeMessage(&sCMsg);
    if (sCMsg.srcNode == sNode.getNodeId())
    {
        int msg = 0;
        MPI_Send(&msg,
                 1,
                 MPI_INT,
                 sCMsg.dstNode,
                 MSG_COMPUTE,
                 MPI_COMM_WORLD);
        printf("[INFO] Node[%d] sent a COMPUTE message to Node[%d]\n", sCMsg.srcNode, sCMsg.dstNode);
        fflush(stdout);
        sNode.setToken(TOKEN_BLACK);
    }
    printf("[INFO] Node[%d] is done with internal computations\n", procId);
    fflush(stdout);

    // post computations initiate the termination detection protocol
    // There would be three types of nodes
    // Root Node, Internal Node, Leaf Node.
    // Termination detection is initiated only by the leaf nodes
    ReturnCode sRc = RC_SUCCESS;
    if (sNode.isLeafNode())
    {
        do
        {
            sRc = initiateTerminationDetection(&sNode);
        }while (RC_RETRY == sRc);
    }
    else if (sNode.isRootNode())
    {
        initiateRootNodeProcess(&sNode);
    }
    else
    {
        // Internal Node
        initiateInternalNodeProcess(&sNode);
    }
}


// initiate and run leaf node process - termination detection starts here
ReturnCode initiateTerminationDetection(Node* pNode)
{
    ReturnCode sRc = RC_SUCCESS;
    int sMsgBuffer[MAX_MSG_SIZE];
    MPI_Status sMsgStatus;

    // starting the termination detection from the leaf nodes
    // sending the token to the immediate parent node
    int sToken = pNode->getToken();
    printf("[INFO] LeafNode[%d] initiating Termination Detection\n", pNode->getNodeId());
    fflush(stdout);
    MPI_Send(&sToken,
             1,
             MPI_INT,
             pNode->getParentNode(),
             MSG_TOKEN,
             MPI_COMM_WORLD);
    // Turn the process, which sent the BLACK token to the parent,
    // into a WHITE after sending the TOKEN.
    if (pNode->getToken() == TOKEN_BLACK)
    {
        pNode->setToken(TOKEN_WHITE);
    }
    pNode->setNodeStatus(STATE_IDLE);

    do
    {
        memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        MPI_Recv(sMsgBuffer,
                 MAX_MSG_SIZE,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &sMsgStatus);

        if (MSG_REPEAT == sMsgStatus.MPI_TAG)
        {
            // re-initiate the termination detection
            sRc = RC_RETRY;
            printf("[INFO] LeafNode[%d] Received a REPEAT request from ParentNode[%d]\n",
                    pNode->getNodeId(), pNode->getParentNode());
            fflush(stdout);
            break;
        }
        else if (MSG_COMPUTE == sMsgStatus.MPI_TAG)
        {
            // Received a COMPUTE message from another node.
            // Just acknowledge that it is received.
            printf("[INFO] LeafNode[%d] Received the COMPUTE message from Node[%d]\n",
                    pNode->getNodeId(), sMsgStatus.MPI_SOURCE);
        }
        else if (MSG_KILL == sMsgStatus.MPI_TAG)
        {
            // received a KILL message from MASTER process.
            // we are done for now.
            sRc = RC_SUCCESS;
            break;
        }
    }while (1);

    return sRc;
}

// initiate and run root node process
ReturnCode initiateRootNodeProcess(Node* pNode)
{
    ReturnCode sRc = RC_SUCCESS;
    int sMsgBuffer[MAX_MSG_SIZE];
    MPI_Status sMsgStatus;
    // wait for all the child nodes to respond with termination token messages
    // once received all ... then check if any black token is received
    // If all are white then inform the master process that the termination detection is complete.
    // If any BLACK token is received then send a REPEAT message to all the child nodes to
    // re-inititate the termination detection algorithm.
    vector<int> sChildNodes;
    pNode->getChildNodes(&sChildNodes);
    int sNodeCountLeft = pNode->getChildNodesCount();
    int sToken = TOKEN_WHITE;
    do
    {
        memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        MPI_Recv(sMsgBuffer,
                 MAX_MSG_SIZE,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &sMsgStatus);

        if (MSG_TOKEN == sMsgStatus.MPI_TAG)
        {
            printf("[INFO] RootNode[%d] Received Token[%d] from ChildNode[%d]\n",
                    pNode->getNodeId(), sMsgBuffer[0], sMsgStatus.MPI_SOURCE);
            fflush(stdout);

            sToken = sToken * sMsgBuffer[0];
            sNodeCountLeft--;
            if (sNodeCountLeft == 0)
            {
                sToken = sToken * pNode->getToken();
                if (TOKEN_WHITE == sToken)
                {
                    // WHITE Token received from all children
                    // inform master process about the completion
                    printf("[INFO] RootNode[%d] Received all tokens from child nodes\n",
                            pNode->getNodeId());
                    fflush(stdout);
                    int msg = 0;
                    MPI_Send(&msg,
                             1,
                             MPI_INT,
                             MASTER,
                             MSG_DONE,
                             MPI_COMM_WORLD);
                }
                else
                {
                    // BLACK token received from one of the child nodes.
                    // Send a REPEAT signal to restart termination detection
                    printf("[INFO] RootNode[%d] Received a BLACK token. Initiating REPEAT Signal\n",
                            pNode->getNodeId());
                    fflush(stdout);
                    vector<int> sChildNodes;
                    pNode->getChildNodes(&sChildNodes);
                    for (int child : sChildNodes)
                    {
                        int msg = 0;
                        MPI_Send(&msg,
                                 1,
                                 MPI_INT,
                                 child,
                                 MSG_REPEAT,
                                 MPI_COMM_WORLD);
                        printf("[INFO] RootNode[%d] Sent REPEAT Signal to ChildNode[%d]\n",
                                pNode->getNodeId(), child);
                        fflush(stdout);
                    }
                    // reset the child nodes count
                    sNodeCountLeft = pNode->getChildNodesCount();
                    sToken = TOKEN_WHITE;
                    // now wait for the child nodes to come back with their tokens
                }
            }
        }
        else if (MSG_COMPUTE == sMsgStatus.MPI_TAG)
        {
            // Received a COMPUTE message from another node.
            // Just acknowledge that it is received.
            printf("[INFO] RootNode[%d] Received the COMPUTE message from Node[%d]\n",
                    pNode->getNodeId(), sMsgStatus.MPI_SOURCE);
            fflush(stdout);
        }
        else if (MSG_KILL == sMsgStatus.MPI_TAG)
        {
            sRc = RC_SUCCESS;
            break;
        }
    }while (1);

    return sRc;
}


// initiate and run internal node process
ReturnCode initiateInternalNodeProcess(Node* pNode)
{
    ReturnCode sRc = RC_SUCCESS;
    int sMsgBuffer[MAX_MSG_SIZE];
    MPI_Status sMsgStatus;

    vector<int> sChildNodes;
    pNode->getChildNodes(&sChildNodes);
    int sNodeCountLeft = pNode->getChildNodesCount();
    int sToken = TOKEN_WHITE;
    do
    {
        memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        MPI_Recv(sMsgBuffer,
                 MAX_MSG_SIZE,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &sMsgStatus);

        if (MSG_REPEAT == sMsgStatus.MPI_TAG)
        {
            printf("[INFO] InternalNode[%d] Received a REPEAT request from ParentNode[%d]\n",
                    pNode->getNodeId(), pNode->getParentNode());
            fflush(stdout);
            for (int child : sChildNodes)
            {
                printf("[INFO] InternalNode[%d] Forwarding REPEAT signal to ChildNode[%d]\n",
                        pNode->getNodeId(), child);
                fflush(stdout);
                int msg = 0;
                MPI_Send(&msg,
                         1,
                         MPI_INT,
                         child,
                         MSG_REPEAT,
                         MPI_COMM_WORLD);
            }
            // reset the child nodes count & default Token
            sNodeCountLeft = pNode->getChildNodesCount();
            sToken = TOKEN_WHITE;
        }
        else if (MSG_TOKEN == sMsgStatus.MPI_TAG)
        {
            printf("[INFO] InternalNode[%d] Received Token[%d] from ChildNode[%d]\n",
                    pNode->getNodeId(), sMsgBuffer[0], sMsgStatus.MPI_SOURCE);
            fflush(stdout);

            sToken = sToken * sMsgBuffer[0];
            sNodeCountLeft--;
            if (sNodeCountLeft == 0)
            {
                // forward the token to parent node
                sToken = sToken * (int)pNode->getToken();
                printf("[INFO] InternalNode[%d] Received all tokens from child nodes. Sending Token[%d] to ParentNode[%d]\n",
                        pNode->getNodeId(), sToken, pNode->getParentNode());
                fflush(stdout);
                MPI_Send(&sToken,
                         1,
                         MPI_INT,
                         pNode->getParentNode(),
                         MSG_TOKEN,
                         MPI_COMM_WORLD);
                // Turn the process, which sent the BLACK token to the parent,
                // into a WHITE after sending the TOKEN.
                if (pNode->getToken() == TOKEN_BLACK)
                {
                    pNode->setToken(TOKEN_WHITE);
                }
            }
        }
        else if (MSG_COMPUTE == sMsgStatus.MPI_TAG)
        {
            // Received a COMPUTE message from another node.
            // Just acknowledge that it is received.
            printf("[INFO] InternalNode[%d] Received the COMPUTE message from Node[%d]\n",
                    pNode->getNodeId(), sMsgStatus.MPI_SOURCE);
            fflush(stdout);
        }
        else if (MSG_KILL == sMsgStatus.MPI_TAG)
        {
            // received a KILL message from MASTER process.
            // we are done for now.
            sRc = RC_SUCCESS;
            break;
        }
    }while (1);

    return sRc;
}
