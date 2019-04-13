#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 1e4 + 7;
const int MaxM = 1e5 + 7;
const int Inf = 0x3f3f3f3f;

int n, m, s, t;
int dist[MaxN];
int curcap[MaxN];

struct Edge
{
    int dst, flow;
    int next;
};
Edge eset[MaxM * 2];
int via[MaxN];
int emax = 1;
void addedge(int src, int dst, int flow)
{
    eset[++emax].dst = dst;
    eset[emax].flow = flow;
    eset[emax].next = via[src];
    via[src] = emax;
}

bool update()
{
    memset(dist, -1, sizeof(dist));
    dist[s] = 0;
    static int bfsq[MaxN];
    int head = 0, tail = 0;
    bfsq[tail++] = s;
    while (head < tail)
    {
        int now = bfsq[head++];
        for (int ed = via[now]; ed; ed = eset[ed].next)
        {
            int next = eset[ed].dst;
            if (dist[next] == -1 && eset[ed].flow > 0)
            {
                dist[next] = dist[now] + 1;
                bfsq[tail++] = next;
            }
        }
    }
    return dist[t] > 0;
}

int walk(int now, int low)
{
    if (now == t)
        return low;
    int rest = 0;
    for (int ed = curcap[now] ? curcap[now] : via[now]; ed; ed = eset[ed].next)
    {
        curcap[now] = ed;
        int next = eset[ed].dst;
        if (dist[now] + 1 == dist[next] && eset[ed].flow > 0
                && (rest = walk(next, std::min(low, eset[ed].flow))) > 0)
        {
            eset[ed].flow -= rest;
            eset[ed ^ 1].flow += rest;
            return rest;
        }
    }
    return 0;
}

int main()
{
    scanf("%d %d %d %d", &n, &m, &s, &t);
    for (int i = 0; i < m; ++i)
    {
        int src, dst, len;
        scanf("%d %d %d", &src, &dst, &len);
        addedge(src, dst, len);
        addedge(dst, src, 0);
    }

    int ans = 0;
    while (update())
    {
        memset(curcap, 0, sizeof(curcap));
        int now;
        while ((now = walk(s, Inf)) > 0)
            ans += now;
    }
    printf("%d\n", ans);
    return 0;
}

