#include <cstdio>

typedef long long laolin;

const int MaxN = 50000 + 7;

int n, l;
int len[MaxN];
laolin sum[MaxN];
laolin dp[MaxN];

int fwdq[MaxN];

laolin dy(int last, int now)
{
    laolin lasty = dp[last] + sum[last] * sum[last];
    laolin nowy = dp[now] + sum[now] * sum[now];
    return nowy - lasty;
}
laolin dx(int last, int now)
{
    return sum[now] - sum[last];
}
laolin cost(int lim, int last)
{
    laolin rest = lim - sum[last];
    return dp[last] + rest * rest;
}

int main()
{
    scanf("%d%d", &n, &l);
    ++l;
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", len + i);
        sum[i] = sum[i - 1] + len[i] + 1;
    }

    int *qhead = fwdq + 1, *qtail = fwdq + 1;
    dp[0] = 0;
    *qtail = 0;
    for (int now = 1; now <= n; ++now)
    {
        laolin lim = sum[now] - l;
        //printf("examine %d\n", now);
        while (qhead < qtail && dy(*qhead, qhead[1]) < 2 * lim * dx(*qhead, qhead[1]))
        {
            //printf("retire %d\n", *qhead);
            ++qhead;
        }
        //printf("select %d\n", *qhead);
        dp[now] = cost(lim, *qhead);
        while (qhead < qtail && dy(*qtail, now) * dx(qtail[-1], *qtail) < dy(qtail[-1], *qtail) * dx(*qtail, now))
        {
            //printf("retire %d\n", *qtail);
            --qtail;
        }
        *++qtail = now;
    }

    printf("%lld\n", dp[n]);

    return 0;
}

