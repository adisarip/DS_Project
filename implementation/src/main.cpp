
#include <string>
#include "mpi.h"
#include "TerminationDetection.h"
using namespace std;

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
        configureAndInitiateNodeProcess(sProcId);
    }

    MPI_Finalize();
}
