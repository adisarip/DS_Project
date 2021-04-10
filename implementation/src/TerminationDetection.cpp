

#include "Graph.h"
#include <iostream>
using namespace std;

int main (int argc, char* argv[])
{
    string sInputFileName = string(argv[1]);
    Graph sGraph(sInputFileName);
    sGraph.computeMST();
    sGraph.displayMST();

    // Start the OpenMPI code for termination detection
}
