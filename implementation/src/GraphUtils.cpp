
#include "GraphUtils.h"

// Utility functions for the Graph algorithmic computations

// Implementing Union-Find data structure

// Find the subset of the element with index 'idx'
// using path-compression technique
int Find(Subset subsets[], int idx)
{
    // find the root and make root as parent of 'idx'
    if (subsets[idx].parent != idx)
    {
        subsets[idx].parent = Find(subsets, subsets[idx].parent);
    }
    return subsets[idx].parent;
}

// A function that does union of two sets of x and y
// by using union by rank
void Union(Subset subsets[], int x, int y)
{
    int x_root = Find(subsets, x);
    int y_root = Find(subsets, y);

    if (subsets[x_root].rank < subsets[y_root].rank)
    {
        subsets[x_root].parent = y_root;
    }
    else if (subsets[x_root].rank > subsets[y_root].rank)
    {
        subsets[y_root].parent = x_root;
    }
    else
    {
        // If ranks are same, then make one as root and
        // increment its rank by one
        subsets[y_root].parent = x_root;
        subsets[x_root].rank++;
    }
}
