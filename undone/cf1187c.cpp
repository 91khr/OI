#include <cstdio>
#include <algorithm>

const int MaxN = 1000 + 7;

int n, m;

struct Fact
{
    bool cover;
    int l, r;
    friend bool operator<(const Fact &a, const Fact &b)
    {
        return a.cover && !b.cover;
    }
};
Fact prop[MaxN];

int ans[MaxN];

class SegTree
{
private:
    bool ord[MaxN << 3];
    bool iscov[MaxN << 3];

    int lchild(int now) { return now * 2; }
    int rchild(int now) { return now * 2 + 1; }

    void push(int now)
    {
        if (iscov[now])
        {
            iscov[lchild(now)] = iscov[rchild(now)] = true;
            ord[rchild(now)] = true;
        }
        ord[lchild(now)] |= ord[now];
    }

public:
    void build(int now, int lb, int rb)
    {
        ord[now] = iscov[now] = false;
        if (lb == rb)
            return;
        int mid = (lb + rb) >> 1;
        build(lchild(now), lb, mid);
        build(rchild(now), mid + 1, rb);
    }

    void set(int ql, int qr, int now, int lb, int rb)
    {
        if (ql <= lb && rb <= qr)
        {
            iscov[now] = true;
            if (ql < lb)
                ord[now] = true;
            return;
        }
        push(now);
        int mid = (lb + rb) >> 1;
        if (ql <= mid)
            set(ql, qr, lchild(now), lb, mid);
        if (mid < qr)
            set(ql, qr, rchild(now), mid + 1, rb);
    }

    bool cover(int ql, int qr, int now, int lb, int rb)
    {
        push(now);
        if (ql <= lb && rb <= qr)
        {
            if (!ord[lchild(now)])
            {
                --ans[lb];
                return true;
            }
            else
            {
                int mid = (lb + rb) >> 1;
                return mid < rb ? cover(ql, qr, rchild(now), mid + 1, rb) : false;
            }
        }
        int mid = (lb + rb) >> 1;
        bool ok = false;
        if (ql <= mid)
            ok = cover(ql, qr, lchild(now), lb, mid);
        if (mid < qr && !ok)
            ok = cover(ql, qr, rchild(now), mid + 1, rb);
        return ok;
    }
};
SegTree tree;

int main()
{
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i)
    {
        int cov;
        scanf("%d%d%d", &cov, &prop[i].l, &prop[i].r);
        prop[i].cover = cov;
    }
    std::sort(prop, prop + m);
    tree.build(1, 1, n);

    Fact *fac = prop;
    for (; fac->cover; ++fac)
        tree.set(fac->l, fac->r, 1, 1, n);
    for (; fac < prop + m; ++fac)
        if (!tree.cover(fac->l, fac->r, 1, 1, n))
        {
            printf("NO\n");
            return 0;
        }
    printf("YES\n");
    for (int i = 1; i <= n; ++i)
        printf("%d ", ans[i] + n);
    printf("\n");
    return 0;
}

