#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>

typedef long long laolin;

const int MaxN = 5000 + 7;
const int MaxM = 50000 + 7;
const laolin Inf = 0x7f7f7f7f7f7f7f7f;

int n, m, s, t;

struct Edge
{
    int dst, next;
    laolin cap, cost;
};
Edge eset[MaxM * 2];
int via[MaxN];
int emax = 1;
void addedge(int src, int dst, int cap, int cost)
{
    eset[++emax].dst = dst;
    eset[emax].next = via[src];
    via[src] = emax;
    eset[emax].cap = cap;
    eset[emax].cost = cost;
}

struct Info
{
    laolin dist;
    int idx;
    Info(laolin d = 0, int i = 0) : dist(d), idx(i) {}
    Info(const Info &o) : dist(o.dist), idx(o.idx) {}
    Info &operator=(const Info &o) { dist = o.dist; idx = o.idx; return *this; }
    friend bool operator<(const Info &a, const Info &b)
    {
        return a.dist > b.dist;
    }
};
laolin dist[MaxN];
bool used[MaxN];
laolin weight[MaxN];
int euse[MaxN], prev[MaxN];
laolin flow[MaxN];
class BFSQ
{
private:
    Info heap[MaxN];
    int bound;

    int touch(int now, int next)
    {
        return next <= bound && heap[now] < heap[next] ? next : now;
    }

public:
    BFSQ() : bound(0) {}
    void push(Info now)
    {
        heap[++bound] = now;
        for (int pos = bound; pos > 1 && heap[pos / 2] < heap[pos]; pos /= 2)
            std::swap(heap[pos], heap[pos / 2]);
    }
    void pop()
    {
        heap[1] = heap[bound--];
        for (int last = 1, pos = 1;
                (pos = touch(touch(pos, pos * 2), pos * 2 + 1)) != last;
                last = pos)
            std::swap(heap[last], heap[pos]);
    }
    Info top() { return heap[1]; }
    bool empty() { return bound <= 0; }
};
//std::priority_queue<Info> bfsq;
BFSQ bfsq;
bool update()
{
    memset(dist + 1, 0x7f, n * sizeof(*dist));
    memset(used + 1, false, n * sizeof(*used));
    dist[s] = 0;
    bfsq.push(Info(dist[s], s));
    while (!bfsq.empty())
    {
        const int now = bfsq.top().idx;
        bfsq.pop();
        if (used[now])
            continue;
        used[now] = true;
        for (int ed = via[now]; ed; ed = eset[ed].next)
        {
            const int next = eset[ed].dst;
            if (dist[next] + weight[next] > dist[now] + weight[now] + eset[ed].cost
                    && eset[ed].cap > 0)
            {
                dist[next] = dist[now] + weight[now] + eset[ed].cost - weight[next];
                flow[next] = std::min(flow[now], eset[ed].cap);
                prev[next] = now;
                euse[next] = ed;
                bfsq.push(Info{dist[next], next});
            }
        }
    }
    return dist[t] != Inf;
}

int main()
{
    scanf("%d%d%d%d", &n, &m, &s, &t);
    laolin maxcost = 0;
    for (int i = 0; i < m; ++i)
    {
        int src, dst, cap, cost;
        scanf("%d%d%d%d", &src, &dst, &cap, &cost);
        addedge(src, dst, cap, cost);
        addedge(dst, src, 0, -cost);
        maxcost = std::max(maxcost, laolin(cost));
    }
    flow[s] = Inf;

    laolin totflow = 0, totcost = 0;
    while (update())
    {
        const laolin cap = flow[t];
        totflow += cap;
        totcost += cap * (dist[t] + weight[t] - weight[s]);
        for (int now = t; now != s; now = prev[now])
        {
            const int ed = euse[now];
            eset[ed].cap -= cap;
            eset[ed ^ 1].cap += cap;
            //printf("edge %d to %lld, %d to %lld(%lld)\n",
                    //ed, eset[ed].cap, ed ^ 1, eset[ed ^ 1].cap, cap);
        }
        for (int i = 1; i <= n; ++i)
            weight[i] += dist[i];
        //printf("update...\n");
    }
    printf("%lld %lld\n", totflow, totcost);
    return 0;
}

