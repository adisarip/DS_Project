

#include <iostream>
#include <iomanip>
#include <cstring>
#include "mpi.h"
#include "Graph.h"
#include "Node.h"
using namespace std;

const int MASTER = 0;
const int MAX_MSG_SIZE = 1024;

int main (int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int procId;
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Master / Manager process to perform initial config operations
    if (MASTER == procId)
    {
        string sInputFileName = string(argv[1]);
        Graph sGraph(sInputFileName);
        sGraph.createAndConfigureSpanningTree();
        sGraph.displayMST();

        const int sSize = sGraph.getNodesCount() + 1;
        int sRoutingMap[sSize][sSize];
        fill(*sRoutingMap, (*sRoutingMap + sSize * sSize), -1);

        int sRootNode = sGraph.getRootNode();
        cout << "Root Node: " << sRootNode << endl;
        sGraph.fillMessageRoutingTable(sRootNode,
                                       sRootNode,
                                       (int*)sRoutingMap);

        cout << "==== Routing Table ====" << endl;
        sGraph.displayRoutingTable((int*)sRoutingMap);

        // sent routing data to all the worker processes

        int sMsgBuffer[MAX_MSG_SIZE];
        int adjNodesCount = -1;
        vector<int>* sAdjList = sGraph.getMSTAdjList();
        for (int sDestProcId = 1; sDestProcId < world_size; sDestProcId++)
        {
            // Send the root node information to each process
            MPI_Send(&sRootNode,
                     1,
                     MPI_INT,
                     sDestProcId,
                     MSG_CONFIG,
                     MPI_COMM_WORLD);

            // Send adjacent nodes list for each node
            memset(sMsgBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
            adjNodesCount = sAdjList[sDestProcId].size();
            for (int i=1; i < adjNodesCount; i++)
            {
                sMsgBuffer[i] = sAdjList[sDestProcId][i];
            }
            MPI_Send(sMsgBuffer,
                     adjNodesCount,
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
        }

        // Initiate termination detection
        //int sLeafNodes[sSize-2]; // max no of leaf nodes = nodeCount - 1
        //int sLeafNodesCount = sGraph.getLeafNodes(sLeafNodes);
        //inititiateTerminationDetection(sLeafNodes, sLeafNodesCount);
    }
    else
    {
        int sMsgSize = -1;
        MPI_Status sMsgStatus;
        int sReceiveBuffer[MAX_MSG_SIZE];

        /// Process/Node Intialization
        Node sNode(procId);

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
        sNode.setRootNode(sRootNode);

        // get the adj nodes list
        memset(sReceiveBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        MPI_Recv(sReceiveBuffer,
                 MAX_MSG_SIZE,
                 MPI_INT,
                 MASTER,
                 MSG_CONFIG,
                 MPI_COMM_WORLD,
                 &sMsgStatus);
        MPI_Get_count(&sMsgStatus, MPI_INT, &sMsgSize);
        sNode.setChildNodes(sReceiveBuffer, sMsgSize);

        // get the node/process route map
        memset(sReceiveBuffer, -1, sizeof(int) * MAX_MSG_SIZE);
        MPI_Recv(sReceiveBuffer,
                 MAX_MSG_SIZE,
                 MPI_INT,
                 MASTER,
                 MSG_CONFIG,
                 MPI_COMM_WORLD,
                 &sMsgStatus);
        MPI_Get_count(&sMsgStatus, MPI_INT, &sMsgSize);
        sNode.setRouteMap(sReceiveBuffer, sMsgSize);

        /*
        string s = string("ProcessId:") + to_string(procId) + string(" | RouteMap: ");
        for (int i=1; i<sMsgSize; i++)
        {
            s += to_string(sProcessRouteMap[i]) + string(" ");
        }
        cout << s << endl;

        s = string("ProcessId:") + to_string(procId) + string(" | RootNode: ") + to_string(sRootNode);
        cout << s << endl;
        */

        //
    }

    MPI_Finalize();
}
