
#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

// Union-Find Data Structure - Interface
struct Subset
{
    int parent;
    int rank;
};
int Find(Subset subsets[], int idx);
void Union(Subset subsets[], int x, int y);
#endif
