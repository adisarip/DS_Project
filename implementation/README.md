
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
> [MAKE] Compiled src/GraphUtils.cpp successfully.
> [MAKE] Compiled src/TerminationDetection.cpp successfully.
> [MAKE] Linking Complete.
> [INFO] No of Nodes in the given graph: 12
> [INFO] Initiating Termination Detection with 1 process.
> [INFO] Displaying MST of the graph : 11 edges.
>     [6--(2)--8]
>     [5--(2)--9]
>     [11--(3)--12]
>     [2--(3)--4]
>     [2--(4)--3]
>     [5--(5)--7]
>     [5--(5)--6]
>     [4--(5)--9]
>     [1--(6)--4]
>     [7--(6)--10]
>     [8--(7)--12]
> $
>```

**_Trace Messages:_**
>```
> [INFO]  -> Informational traces
> [MAKE]  -> Build traces
> [ERROR] -> Error traces
> [USAGE] -> Command usage
>```
