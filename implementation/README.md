
# Spanning Tree Based Termination Detection

## Introduction
There are many spanning tree based termination detection algorithms in the distributed systems literature. Of all of them below mentioned are shortlisted for the discussion (temporarily adding details - we will update again later).
>- Rodney. W. Topor
>- Chandrashekhar Venkatesan
>- R.K Arora & M.N Gupta

## Solution Approach
>- Using Kruskal's algorithm for constructing a MST from a given graph, using "Union-Find" data structure
>- Using OpenMPI framework for simulating the distributed environment.
>- Each process will denote a node in the MST.
>- **<to be continued ...>**

## Environment Details
> Operating System:  _Mac OSX / Linux_  
> Package Installer : _Homebrew/apt-get_  
> Compiler : _mpic++_  
> Scripting : _python3_  
> Additonal Packages : _OpenMPI_  

## Source Tree
>- Given below is the source tree for the current assignment.
>```
> ├── Makefile
> ├── README.md
> ├── bin
> │   ├── README.md
> │   └── sttd
> ├── design
> │   └── STBTD_Design.docx
> ├── obj
> │   └── README.md
> ├── run_td.py
> ├── src
> │   ├── Graph.cpp
> │   ├── Graph.h
> │   ├── GraphUtils.cpp
> │   ├── GraphUtils.h
> │   ├── README.md
> │   └── TerminationDetection.cpp
> └── test
>     ├── README.md
>     └── input.txt
>
> 5 directories, 15 files
>```

>- './src/' : Directory containing all the source code & utility files.
>- './obj/' : Directory containing all temporary object files.
>- './bin/' : Directory containing the executable binary files.
>- './test/' : Directory containing the test data
>- Graph.h/cpp : A class implementing the graph algorithm for computing Minimum Spanning Tree.
>- GraphUtils.h/cpp : Implementation of Union-Find data structure, used while computing MST.
>- TerminationDetection.cpp : Main file to compute the MST and initiate the termination detection algorithm.
>- Run the command **'make clean; make'** from the parent directory to create the executable binaries.

## Input Data Format & Execution Runs

**_Sample input file format:_**
>```
> #####################################################
> # Input file format for the creation of graph
> # <no_of_nodes>
> # <source_vertex> <destination_vertex> <edge_weight>
> # <source_vertex> <destination_vertex> <edge_weight>
> # ...
> #####################################################
> 12
> 1 2 10
> 1 3 7
> 1 4 6
> 2 3 4
> 2 4 3
> 2 5 8
> 2 6 11
> 3 8 6
> 4 9 5
> 5 6 5
> 5 7 5
> 5 9 2
> 6 8 2
> 6 12 13
> 7 10 6
> 7 11 10
> 7 12 9
> 8 12 7
> 9 10 17
> 10 11 9
> 11 12 3
>```

**_Sample Execution Run:_**
>- Use the wrapper script **run_td.py** to build and execute the binaries.
>- Go to the implementation directory and run the following command  
> **./run_td.py <input_file\> <no_of_processes\>**
>
>```
> $ ./run_td.py test/input.txt 1
> [INFO] Building binaries ...
> [MAKE] Cleaning all the object files and binaries.
> [MAKE] Compiled src/Graph.cpp successfully.
> [MAKE] Compiled src/Node.cpp successfully.
> [MAKE] Compiled src/TerminationDetection.cpp successfully.
> [MAKE] Linking Complete.
> [INFO] No of Nodes in the given graph: 11
> [INFO] Initiating Termination Detection with 12 processes (1 process per node and 1 additional master/manager process).
> [INFO] MASTER Process configuring and setting the process(s) environment
> [INFO] Displaying MST of the given graph as an Adjacency List:
> 1 -> 4
> 2 -> 4,3
> 3 -> 2
> 4 -> 2,9,1
> 5 -> 9,6,7
> 6 -> 8,5
> 7 -> 5,10
> 8 -> 6,11
> 9 -> 5,4
> 10 -> 7
> 11 -> 8
> [INFO] Root Node: 5
> [INFO] Process#3 is done with internal computations
> [INFO] LeafNode[3] intitiating Termination Detection
> [INFO] Process#2 is done with internal computations
> [INFO] InternalNode[2] Received all tokens from child nodes. Sending token to ParentNode[4]
> [INFO] Process#11 is done with internal computations
> [INFO] LeafNode[11] intitiating Termination Detection
> [INFO] Process#7 is done with internal computations
> [INFO] Process#4 is done with internal computations
> [INFO] Process#9 is done with internal computations
> [INFO] Process#1 is done with internal computations
> [INFO] LeafNode[1] intitiating Termination Detection
> [INFO] InternalNode[4] Received all tokens from child nodes. Sending token to ParentNode[9]
> [INFO] InternalNode[9] Received all tokens from child nodes. Sending token to ParentNode[5]
> [INFO] Process#6 is done with internal computations
> [INFO] Process#8 is done with internal computations
> [INFO] InternalNode[8] Received all tokens from child nodes. Sending token to ParentNode[6]
> [INFO] InternalNode[6] Received all tokens from child nodes. Sending token to ParentNode[5]
> [INFO] Process#10 is done with internal computations
> [INFO] LeafNode[10] intitiating Termination Detection
> [INFO] InternalNode[7] Received all tokens from child nodes. Sending token to ParentNode[5]
> [INFO] Process#5 is done with internal computations
> [INFO] RootNode[5] Received all tokens from child nodes.
> [INFO] Termination Detection completed
> $
>```

**_Trace Messages:_**
>```
> [INFO]  -> Informational traces
> [MAKE]  -> Build traces
> [ERROR] -> Error traces
> [USAGE] -> Command usage
>```
