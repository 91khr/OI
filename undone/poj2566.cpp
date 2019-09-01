#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif
#define logmsg(...) fprintf(stderr, __VA_ARGS__)

const int MaxN = int(1e5) + 7;
const int Inf = 0x7fffffff;

typedef std::pair<int, int> Handle;
Handle mkpr(int l, int r) { return std::make_pair(l, r); }

int n, k;
int arr[MaxN], query[MaxN];

namespace Data
{
int randint(int lo, int hi) { return rand() % (hi - lo + 1) + lo; }
bool scan()
{
    scanf("%d %d", &n, &k);
    if (n == 0)
        return false;
    for (int i = 1; i <= n; ++i)
        scanf("%d", arr + i);
    for (int i = 1; i <= k; ++i)
        scanf("%d", query + i);
    return true;
}
void generate()
{
    n = randint(3, 17);
    k = randint(1, 3);
    for (int i = 1; i <= n; ++i)
        arr[i] = randint(-20, 20);
    for (int i = 1; i <= k; ++i)
        query[i] = randint(0, 50);
}
void print()
{
    printf("%d %d\n", n, k);
    for (int i = 1; i <= n; ++i)
        printf("%d ", arr[i]);
    printf("\n");
    for (int i = 1; i <= k; ++i)
        printf("%d ", query[i]);
    printf("\n");
}
}  // End namespace Data
struct Answer
{
    int val, l, r;
    Answer() : Answer(0, 0, 0) {}
    Answer(int v, int lo, int hi) : val(v), l(lo), r(hi) {}
    friend bool operator==(const Answer &a, const Answer &b)
    {
        return a.val == b.val && a.l == b.l && a.r == b.r;
    }
};

namespace StdSln
{
int sum[MaxN];
void init()
{
    for (int i = 1; i <= n; ++i)
        sum[i] = sum[i - 1] + arr[i];
}

int diff(int l, int r) { return abs(sum[r] - sum[l - 1]); }

Answer solve(int t)
{
    Answer res;
    res.val = Inf;
    for (int lo = 1; lo <= n; ++lo)
        for (int hi = lo; hi <= n; ++hi)
            if (abs(diff(lo, hi) - t) < abs(res.val - t))
            {
                res.val = diff(lo, hi);
                res.l = lo;
                res.r = hi;
            }
    return res;
}
}  // End namespace StdSln

namespace ProgSln
{
void init();
Answer solve(int t);
}  // End namespace ProgSln
void printres(int test, Answer stdans, Answer progans)
{
    Data::print();
    logmsg("for %d:\n", test);
    logmsg("std: %d %d %d\n", stdans.val, stdans.l, stdans.r);
    logmsg("prog: %d %d %d\n", progans.val, progans.l, progans.r);
}
bool verify(int test, Answer stdans, Answer progans)
{
    if (abs(stdans.val - test) != abs(progans.val - test))
    {
        logmsg("\nwrong answer\n");
        printres(test, stdans, progans);
        return false;
    }
    int sum = 0;
    for (int i = progans.l; i <= progans.r; ++i)
        sum += arr[i];
    sum = abs(sum);
    if (sum != progans.val)
    {
        logmsg("\nerror range: sum is %d\n", sum);
        printres(test, stdans, progans);
        return false;
    }
    else
        return true;
}
void check()
{
    for (int data = 1; true; ++data)
    {
        logmsg("\rdata %d", data);
        fflush(stderr);
        Data::generate();
        ProgSln::init();
        StdSln::init();
        Answer stdans, progans;
        int test = -1;
        bool ok = true;
        for (int count = 1; count <= k; ++count)
        {
            test = query[count];
            stdans = StdSln::solve(test);
            progans = ProgSln::solve(test);
            if (!verify(test, stdans, progans))
            {
                ok = false;
                break;
            }
        }
        if (!ok)
            break;
    }
}

int main(int argc, char **)
{
    srand(20030125);
    if (argc <= 1)
    {
        Data::scan();
        ProgSln::init();
        for (int i = 1; i <= k; ++i)
        {
            Answer ans = ProgSln::solve(query[i]);
            printf("%d %d %d\n", ans.val, ans.l, ans.r);
        }
    }
    else if (argc == 2)
    {
        Data::scan();
        StdSln::init();
        for (int i = 1; i <= k; ++i)
        {
            Answer ans = StdSln::solve(query[i]);
            printf("%d %d %d\n", ans.val, ans.l, ans.r);
        }
    }
    else
        check();
    return 0;
}

namespace ProgSln
{
Handle sum[MaxN];
int ansd;
Answer ans;

void init()
{
    sum[0].first = 0;
    for (int i = 1; i <= n; ++i)
    {
        sum[i].first = sum[i - 1].first + arr[i];
        sum[i].second = i;
    }
    std::sort(sum, sum + n + 1);
    for (int i = 1; i <= n; ++i)
        echo("%d:%d ", sum[i].second, sum[i].first);
    echo("\n");
}

int diff(int l, int r) { return abs(sum[r].first - sum[l].first); }
void updans(int l, int r, int t)
{
    echo("upd with range %d %d, diff %d: ", l, r, diff(l, r));
    if (abs(diff(l, r) - t) < ansd)
    {
        ansd = abs(diff(l, r) - t);
        ans.val = diff(l, r);
        int pl = sum[l].second, pr = sum[r].second;
        if (pl > pr)
            std::swap(pl, pr);
        ++pl;
        ans.l = pl;
        ans.r = pr;
        echo("ans to %d %d\n", pl, pr);
    }
    else
        echo("failed\n");
}

Answer solve(int t)
{
    echo("for targe %d:\n", t);
    ansd = Inf;
    int l = 1, r = 2;
    while (r <= n + 1 && l < r)
    {
        for (; r <= n + 1 && diff(l, r) <= t; ++r)
            updans(l, r, t);
        if (r > n && diff(l, r - 1) < t)
            for (--r; l < r && diff(l, r) <= t; ++l)
                updans(l, r, t);
        for (; l < r && r <= n + 1 && diff(l, r) > t; ++l, r += l >= r)
            updans(l, r, t);
    }
    return ans;
}
}

