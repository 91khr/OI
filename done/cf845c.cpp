#include <cstdio>
#include <algorithm>

const int MaxN = 2e5 + 7;

struct Range
{
    int beg, end;
    Range(int l = 0, int r = 0)
        : beg(l), end(r) {}
    friend bool operator < (const Range &a, const Range &b)
    {
        return a.beg < b.beg;
    }
};

struct TV
{
    int nextfree;
    bool free(int time) { return nextfree <= time; }
    TV(int fr = -1): nextfree(fr) {}
};

int n;
Range anime[MaxN];
TV tv1, tv2;

int main(void)
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d %d", &anime[i].beg, &anime[i].end);
        ++anime[i].end;  // So ignore the limitation
    }
    std::sort(anime + 1, anime + n + 1);
    // Exam every anime
    for (int i = 1; i <= n; ++i)
    {
        //printf("get %d %d, tv1 = %d, tv2 = %d\n", anime[i].beg, anime[i].end, tv1.nextfree, tv2.nextfree);
        //printf("1 free: %d, 2 free: %d\n", tv1.free(anime[i].beg), tv2.free(anime[i].beg));
        // Watch it if can
        if (tv1.free(anime[i].beg))
            tv1.nextfree = anime[i].end;
        else if (tv2.free(anime[i].beg))
            tv2.nextfree = anime[i].end;
        else
        {
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    return 0;
}
