#include <cstdio>
#include <algorithm>

const int MaxN = 100 + 7;

struct Road
{
    int lower, upper;
    int fac;  // 1 for on, -1 for off, 0 for none
};

struct Range
{
    int lower, upper;
    Range(int l = 0, int u = 0) : lower(l), upper(u) {}
};

int n;
Road rd[MaxN];
int left, right;

void scan()
{
    scanf("%d", &n);
    char buf[10];
    for (int i = 1; i <= n; ++i)
    {
        scanf("%s%d%d", buf, &rd[i].lower, &rd[i].upper);
        if (buf[1] == 'n') rd[i].fac = 1;  // on
        else if (buf[1] == 'f') rd[i].fac = -1;  // off
        else if (buf[1] == 'o') rd[i].fac = 0;  // none
    }
}

int main(void)
{
#ifndef DEBUG
    freopen("traffic.in", "r", stdin);
    freopen("traffic.out", "w", stdout);
#endif
    scan();
    // Find left and right
    for (int i = 1; i <= n; ++i)
        if (rd[i].fac == 0)
        {
            left = i;
            break;
        }
    for (int i = n; i >= 1; --i)
        if (rd[i].fac == 0)
        {
            right = i;
            break;
        }
    // Collect in
    {
        Range in(rd[right].lower, rd[right].upper);
        for (int i = right - 1; i >= 1; --i)
            if (rd[i].fac == 1)
            {
                in.lower -= rd[i].upper;
                in.upper -= rd[i].lower;
            }
            else if (rd[i].fac == -1)
            {
                in.lower += rd[i].lower;
                in.upper += rd[i].upper;
            }
            else if (rd[i].fac == 0)
            {
                in.lower = std::max(in.lower, rd[i].lower);
                in.upper = std::min(in.upper, rd[i].upper);
            }
        printf("%d %d\n", std::max(in.lower, 0), in.upper);
    }
    // Collect out
    {
        Range out(rd[left].lower, rd[left].upper);
        for (int i = left + 1; i <= n; ++i)
            if (rd[i].fac == 1)
            {
                out.lower += rd[i].lower;
                out.upper += rd[i].upper;
            }
            else if (rd[i].fac == -1)
            {
                out.lower -= rd[i].upper;
                out.upper -= rd[i].lower;
            }
            else if (rd[i].fac == 0)
            {
                out.lower = std::max(out.lower, rd[i].lower);
                out.upper = std::min(out.upper, rd[i].upper);
            }
        printf("%d %d\n", std::max(out.lower, 0), out.upper);
    }
    return 0;
}

