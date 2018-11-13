#include <cstdio>

typedef long long laolin;

const int MaxN = static_cast<int>(1e5 + 7);

class SegTree
{
private:
    int amount[MaxN << 2];
    int tag[MaxN << 2];
    int lchild(int now) { return now << 1; }
    int rchild(int now) { return now << 1 | 1; }
    void push(int now, int delta)
    {
        if (tag[now])
        {
            amount[lchild(now)] = delta / 2 + 1 - amount[lchild(now)];
            amount[rchild(now)] = delta / 2 - amount[rchild(now)];
            tag[lchild(now)] = !tag[lchild(now)];
            tag[rchild(now)] = !tag[rchild(now)];
        }
        tag[now] = 0;
    }
    void pull(int now)
    {
        amount[now] = amount[lchild(now)] + amount[rchild(now)];
    }

  public:
    void set(int now, int lb, int rb, int invl, int invr)
    {
        if (invl <= lb && rb <= invr)
        {
            amount[now] = rb - lb + 1 - amount[now];
            tag[now] = !tag[now];
            return;
        }
        int mid = (lb + rb) >> 1;
        push(now, rb - lb + 1);
        if (invl <= mid)
        {
            set(lchild(now), lb, mid, invl, invr);
        }
        if (mid < invr)
        {
            set(rchild(now), mid + 1, rb, invl, invr);
        }
        pull(now);
    }
    laolin query(int now, int lb, int rb, int invl, int invr)
    {
        if (invl <= lb && rb <= invr)
        {
            return amount[now];
        }
        int mid = (lb + rb) >> 1;
        laolin ans = 0;
        push(now, rb - lb + 1);
        if (invl <= mid)
        {
            ans += query(lchild(now), lb, mid, invl, invr);
        }
        if (mid < invr)
        {
            ans += query(rchild(now), mid + 1, rb, invl, invr);
        }
        return ans;
    }
};

SegTree line, col;
int n, m, q;

int main(void)
{
    scanf("%d %d %d", &n, &m, &q);
    while (q--)
    {
        int ty;
        scanf("%d", &ty);
        if (ty == 1)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            line.set(1, 1, n, x, x);
            col.set(1, 1, m, y, y);
        }
        else
        {
            int x1, y1, x2, y2;
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            laolin linum = line.query(1, 1, n, x1, x2);
            laolin colnum = col.query(1, 1, m, y1, y2);
            printf("%lld\n", linum * (y2 - y1 + 1) + colnum * (x2 - x1 + 1) - 2 * linum * colnum);
        }
    }

    return 0;
}

