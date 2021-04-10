

#include "Graph.h"
#include <iostream>
using namespace std;

void printUsageAndExit()
{
    cout << "Usage: sttd <graph_input_file>" << endl;
    exit(-1);
}

int main (int argc, char* argv[])
{
    string sInputFileName;
    // verify the inout arguments
    if (argc < 2)
    {
        cout << "[ERRR] Input file missing" << endl;
        printUsageAndExit();
    }
    else if (argc > 2)
    {
        cout << "[ERRR] Invalid arguments" << endl;
        printUsageAndExit();
    }
    else
    {
        sInputFileName = string(argv[1]);
    }

    Graph sGraph(sInputFileName);
    //sGraph.displayGraph();
    sGraph.computeMST();
    sGraph.displayMST();
}
