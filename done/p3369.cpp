#include <cstdio>
#include <cmath>

const int MaxN = 1e5 + 7;

class FHQTreap
{
public:

    int value[MaxN], child[2][MaxN], weight[MaxN], width[MaxN];
    int root, max;

    FHQTreap(void) : root(0), max(0) {}

    void update(int now)
    {
        width[now] = width[child[0][now]] + width[child[1][now]] + 1;
    }

    void valsplit(int now, int pivot, int &left, int &right)
    {
        if (!now)
        {
            left = right = 0;
            return;
        }
        if (value[now] < pivot)
        {
            left = now;
            valsplit(child[1][now], pivot, child[1][now], right);
        }
        else
        {
            right = now;
            valsplit(child[0][now], pivot, left, child[0][now]);
        }
        update(now);
    }
    void sizsplit(int now, int bound, int &left, int &right)
    {
        if (!now)
        {
            left = right = 0;
            return;
        }
        if (width[child[0][now]] < bound)
        {
            left = now;
            sizsplit(child[1][now], bound - width[child[0][now]] - 1, child[1][now], right);
        }
        else
        {
            right = now;
            sizsplit(child[0][now], bound, left, child[0][now]);
        }
        update(now);
    }

    int merge(int left, int right)
    {
        if (!left || !right)
            return left + right;
        int res;
        if (weight[left] > weight[right])
        {
            res = left;
            child[1][res] = merge(child[1][res], right);
        }
        else
        {
            res = right;
            child[0][res] = merge(left, child[0][res]);
        }
        update(res);
        return res;
    }

    int newnode(int val)
    {
        int res = ++max;
        value[res] = val;
        weight[res] = rand();
        width[res] = 1;
        return res;
    }

    void insert(int val)
    {
        int left, right;
        valsplit(root, val, left, right);
        root = merge(merge(left, newnode(val)), right);
    }

    void remove(int val)
    {
        int left, right, del;
        valsplit(root, val, left, right);
        valsplit(right, val + 1, del, right);
        del = merge(child[0][del], child[1][del]);
        root = merge(left, merge(del, right));
    }

    int rankof(int val)
    {
        int left, right;
        valsplit(root, val, left, right);
        int res = width[left] + 1;
        root = merge(left, right);
        return res;
    }

    int inrank(int rank)
    {
        int left, right, mid;
        sizsplit(root, rank, left, right);
        sizsplit(left, rank - 1, left, mid);
        root = merge(merge(left, mid), right);
        return value[mid];
    }

    int prae(int val)
    {
        int left, right;
        valsplit(root, val, left, right);
        int res = left;
        while (child[1][res])
            res = child[1][res];
        root = merge(left, right);
        return value[res];
    }

    int post(int val)
    {
        int left, right;
        valsplit(root, val + 1, left, right);
        int res = right;
        while (child[0][res])
            res = child[0][res];
        root = merge(left, right);
        return value[res];
    }
};

int n;
FHQTreap tree;

int main(void)
{
    srand(33697011);
    scanf("%d", &n);
    while (n--)
    {
        int opt, val;
        scanf("%d %d", &opt, &val);
        if (opt == 1)
            tree.insert(val);
        else if (opt == 2)
            tree.remove(val);
        else if (opt == 3)
            printf("%d\n", tree.rankof(val));
        else if (opt == 4)
            printf("%d\n", tree.inrank(val));
        else if (opt == 5)
            printf("%d\n", tree.prae(val));
        else if (opt == 6)
            printf("%d\n", tree.post(val));
    }
    return 0;
}

