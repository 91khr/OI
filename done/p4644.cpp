#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 10000 + 7;
const int MaxT = 1e5 + 7;
const int Inf = 0x3f3f3f3f;

int n, beg, end;
struct Cow
{
    int beg, end;
    int cost;
    friend bool operator < (const Cow &a, const Cow &b)
    {
        return a.end == b.end ? a.beg < b.beg : a.end < b.end;
    }
};
Cow cow[MaxN];

class SegTree
{
private:
    int minval[MaxT << 2], fill[MaxT << 2];

    int lchild(int now) { return now << 1; }
    int rchild(int now) { return (now << 1) | 1; }
    void push(int now)
    {
        minval[lchild(now)] = std::min(minval[lchild(now)], fill[now]);
        minval[rchild(now)] = std::min(minval[rchild(now)], fill[now]);
        fill[lchild(now)] = std::min(fill[lchild(now)], fill[now]);
        fill[rchild(now)] = std::min(fill[rchild(now)], fill[now]);
    }
    void update(int now)
    {
        minval[now] = std::min(minval[lchild(now)], minval[rchild(now)]);
    }

public:
    void build(int now, int lb, int rb)
    {
        minval[now] = fill[now] = Inf;
        if (lb == rb)
            return;
        int mid = (lb + rb) >> 1;
        build(lchild(now), lb, mid);
        build(rchild(now), mid + 1, rb);
    }

    int query(int ql, int qr, int now, int lb, int rb)
    {
        if (ql <= lb && rb <= qr)
            return minval[now];
        push(now);
        int mid = (lb + rb) >> 1;
        int ans = Inf;
        if (ql <= mid)
            ans = std::min(ans, query(ql, qr, lchild(now), lb, mid));
        if (mid < qr)
            ans = std::min(ans, query(ql, qr, rchild(now), mid + 1, rb));
        return ans;
    }

    void set(int ql, int qr, int val, int now, int lb, int rb)
    {
        if (ql <= lb && rb <= qr)
        {
            fill[now] = std::min(fill[now], val);
            minval[now] = std::min(minval[now], val);
            return;
        }
        push(now);
        int mid = (lb + rb) >> 1;
        if (ql <= mid)
            set(ql, qr, val, lchild(now), lb, mid);
        if (mid < qr)
            set(ql, qr, val, rchild(now), mid + 1, rb);
        update(now);
    }
};
SegTree tree;

int main()
{
    scanf("%d %d %d", &n, &beg, &end);
    ++beg; ++end;
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d %d %d", &cow[i].beg, &cow[i].end, &cow[i].cost);
        ++cow[i].beg; ++cow[i].end;
    }
    std::sort(cow + 1, cow + n + 1);

    tree.build(1, beg, end);
    int ans;
    if (cow[n].end < end)
    {
        printf("-1\n");
        return 0;
    }
    tree.set(cow[n].beg, cow[n].end, cow[n].cost, 1, beg, end);
    for (int i = n - 1; i > 0; --i)
    {
        //printf("proc %d %d\n", cow[i].beg, cow[i].end);
        ans = tree.query(cow[i].beg, cow[i].end + 1, 1, beg, end);
        //printf("set %d %d to %d(%d)\n", cow[i].beg, cow[i].end + 1, cow[i].cost + ans, ans);
        tree.set(cow[i].beg, cow[i].end, cow[i].cost + ans, 1, beg, end);
    }
    ans = tree.query(beg, beg, 1, beg, end);
    if (ans >= Inf)
        printf("-1\n");
    else
        printf("%d\n", ans);

    return 0;
}

