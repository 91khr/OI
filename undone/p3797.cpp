#include <cstdio>
#include <algorithm>
#ifdef DEBUG
#include "../debug.hpp"
#else
template<typename First, typename ...Rest>
void VarLog(First, Rest...) {}
template<typename First, typename ...Rest>
void FormatLog(First, Rest...) {}
#endif
using std::max;
using std::min;

const int MaxN = static_cast<int>(2e5 + 7);

int n, m;

class SegTree
{
private:
    int pairs[MaxN << 2];
    int lclose[MaxN << 2];
    int ropen[MaxN << 2];

    int lchild(int o) { return o << 1; }
    int rchild(int o) { return o << 1 | 1; }
    void pull(int now)
    {
        // VarLog(now);
        // VarLog(lclose[lchild(now)], ropen[lchild(now)], lclose[rchild(now)], ropen[rchild(now)]);
        if (lclose[lchild(now)] == -1 && ropen[lchild(now)] == -1)
        {
            lclose[now] = lclose[rchild(now)];
        }
        else
        {
            lclose[now] = lclose[lchild(now)];
        }
        if (ropen[rchild(now)] == -1 && lclose[rchild(now)] == -1)
        {
            ropen[now] = ropen[lchild(now)];
        }
        else
        {
            ropen[now] = ropen[rchild(now)];
        }
        pairs[now] = pairs[lchild(now)] + pairs[rchild(now)];
        if (ropen[lchild(now)] != -1 && lclose[rchild(now)] != -1)
        {
            ++pairs[now];
        }
        // VarLog(lclose[now], ropen[now], pairs[now]);
    }

public:
    void build(int now, int lb, int rb)
    {
        if (lb == rb)
        {
            lclose[now] = -1;
            ropen[now] = -1;
            pairs[now] = 0;
            return;
        }
        int mid = (lb + rb) >> 1;
        build(lchild(now), lb, mid);
        build(rchild(now), mid + 1, rb);
        pull(now);
    }

    void set(int now, int lb, int rb, int pos, char type)
    {
        if (lb == rb)
        {
            if (type == '(')
            {
                ropen[now] = pos;
                lclose[now] = -1;
            }
            else if (type == ')')
            {
                ropen[now] = -1;
                lclose[now] = pos;
            }
            else
            {
                ropen[now] = -1;
                lclose[now] = -1;
            }
            return;
        }
        //FormatLog("set % % %\n", now, lb, rb);
        int mid = (lb + rb) >> 1;
        if (pos <= mid)
        {
            set(lchild(now), lb, mid, pos, type);
        }
        else
        {
            set(rchild(now), mid + 1, rb, pos, type);
        }
        pull(now);
        //VarLog(lclose[now], ropen[now], pairs[now]);
    }

    int query(int now, int lb, int rb, int invl, int invr)
    {
        if (invl <= lb && rb <= invr)
        {
            return pairs[now];
        }
        int mid = (lb + rb) >> 1;
        int ans = 0;
        if (invl <= mid)
        {
            ans += query(lchild(now), lb, mid, invl, invr);
        }
        if (mid < invr)
        {
            ans += query(rchild(now), mid + 1, rb, invl, invr);
        }
        if (ropen[lchild(now)] != -1 && ropen[lchild(now)] >= invl)
        {
            if (lclose[rchild(now)] != -1 && lclose[rchild(now)] <= invr)
            {
                ++ans;
            }
        }
        return ans;
    }
};

SegTree tree;

int main(void)
{
    scanf("%d %d", &n, &m);
    tree.build(1, 1, n);
    // FormatLog("End build\n");
    tree.set(1, 1, n, 1, '(');
    tree.set(1, 1, n, n, ')');
    // FormatLog("End preprocess\n");

    while (m--)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int pos;
            char ty;
            scanf("%d %c", &pos, &ty);
            tree.set(1, 1, n, pos, ty);
        }
        else
        {
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%d\n", tree.query(1, 1, n, l, r));
        }
    }

    return 0;
}

