/*
Problem: Given a graph of n vertecies and m edges,
    in which every vertex has a weight.
    Output the maxinum weight of a path.
*/
#include <cstdio>
#include <cstring>

const int MaxN = 1e4 + 7;
const int MaxM = 1e5 + 7;

struct Edge
{
    int dst, next;
};

int n, m;
int weight[MaxN];
Edge edgeset[MaxM];
int edgenum;
int via[MaxN];

void addedge(int src, int dst)
{
    edgeset[edgenum].dst = dst;
    edgeset[edgenum].next = via[src];
    via[src] = edgenum++;
}

/* Point shrink */
namespace ShrinkPoint
{
    // Merge set
    int parent[MaxN];
    void initset(int siz)
    {
        while (--siz != -1)
        {
            parent[siz] = siz;
        }
    }
    int getpar(int a)
    {
        if (parent[a] == a)
        {
            return a;
        }
        else
        {
            return parent[a] = getpar(parent[a]);
        }
    }
    int merge(int dst, int src)
    {
        parent[getpar(src)] = getpar(dst);
    }

    // Tarjan(?) algorithm
    void Tarjan(void);
}  // End namespace ShrinkPoint

int main(void)
{
    // Scan
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", weight + i);
    }
    memset(via, -1, sizeof(via));
    for (int i = 0; i < m; ++i)
    {
        int s, t;
        scanf("%d %d", &s, &t);
        addedge(s, t);
    }

    return 0;
}
