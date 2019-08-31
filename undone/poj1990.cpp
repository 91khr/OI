#include <cstdio>
#include <cstring>
#include <algorithm>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif

typedef std::pair<int, int> handle;
typedef long long laolin;

const int MaxN = 2e4 + 7;

int n;
handle cow[MaxN];
int tot[MaxN], vol[MaxN];

int query(int *arr, int pos);
void add(int *arr, int pos, int d);

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d %d", &cow[i].first, &cow[i].second);
    std::sort(cow + 1, cow + n + 1);
    laolin ans = 0;
    for (int now = 1, sum = 0; now <= n; sum += cow[now++].second)
    {
        int val = cow[now].second;
        int les = query(tot, val);
        laolin sm = query(vol, val);
        //ans += les * (now * val - sm) + (now - les) * (sum - sm - now * val);
        ans += cow[now].first * ((les * val - sm) + (sum - sm - val * (now - 1 - les)));
        add(tot, val, 1);
        add(vol, val, val);
    }
    printf("%lld\n", ans);
    return 0;
}

int lowbit(int x) { return x & (-x); }

int query(int *arr, int pos)
{
    int res = 0;
    for (; pos > 0; pos -= lowbit(pos))
        res += arr[pos];
    return res;
}

void add(int *arr, int pos, int d)
{
    for (; pos < MaxN; pos += lowbit(pos))
        arr[pos] += d;
}

