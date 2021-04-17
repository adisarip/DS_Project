

#include "Graph.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main (int argc, char* argv[])
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

    // Start the OpenMPI code for termination detection
    cout << "==== Routing Table ====" << endl;
    sGraph.displayRoutingTable((int*)sRoutingMap);
}
