
#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

// Union-Find Data Structure - Interface
#define MAX_CHILDREN 10
struct Subset
{
    int rank;
    int parent;
};
int Find(Subset subsets[], int idx);
void Union(Subset subsets[], int x, int y);
#endif
