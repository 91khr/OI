#include <cstdio>
#include <cmath>
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <algorithm>
namespace Useful_Helpers {
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif
#define Reps(var, init, delim, step) for (int var = (init); var <= (delim); var += (step))
#define Rep(var, init, delim) Reps(var, init, delim, 1)
#define Repr(var, init, delim) Reps(var, init, delim, -1)
template<typename DstTy, typename SrcTy>
DstTy set(DstTy &dst, const SrcTy &src) { return dst = static_cast<DstTy>(src); }
//
typedef long long i64t;
typedef unsigned long long u64t;
typedef unsigned u32t;
typedef int i32t;
//
namespace IO_Helper {
template<typename Ty>
int read(Ty &var);  // Read a variable
//
#define SpecRead(type) template<> int Useful_Helpers::IO_Helper::read<type>(type &var)
#define ImplRead(type, fmt) \
    SpecRead(type) { return scanf(fmt, &var); }
ImplRead(int, "%" SCNd32);
ImplRead(char, " %c");
ImplRead(long long, "%" SCNd64);
SpecRead(char*) { return scanf("%s", var); }
template<int Size> int read(char (&i)[Size]) { return scanf("%s", i); }
#undef ImplRead
template<typename Ty>
void print(const Ty &var);
#define SpecPrint(type) template<> void Useful_Helpers::IO_Helper::print<type>(const type &var)
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(long long, "%" PRId64);
template<> void print<const char*>(const char *const &str) { fputs(str, stdout); }
template<int Size> void print(const char (&str)[Size]) { fputs(str, stdout); }
#undef ImplPrint
//
struct IO {
private:
    int read() { return 1; }
public:
    template<typename First, typename ...Rest>
    int read(First &car, Rest& ...cdr)
    { int cur = IO_Helper::read(car); return cur && cur != EOF ? this->read(cdr...) : cur; }
    template<typename Ty, int Size>
    int read(Ty (&arr)[Size], int beg, int end)
    { Rep(i, beg, end) this->read(arr[i]); return end - beg + 1; }
    void print(const char *fmt) { fputs(fmt, stdout); }
    template<typename First, typename ...Rest>
    void print(const char *fmt, const First &car, const Rest& ...cdr)
    {
        const char *fwd = fmt;
        int len = 0;
        while (*fwd && *fwd != '$')
        {
            ++fwd;
            ++len;
        }
        if (*fwd)
        {
            printf("%.*s", len, fmt);
            IO_Helper::print(car);
            this->print(fwd + 1, cdr...);
        }
    }
}io;
}  // End namespace IO_Helper
using IO_Helper::io;
} using namespace Useful_Helpers;

const int MaxN = int(1e5) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

struct Trash
{
    int x, y;
    int w;
};

int n, c;
Trash ai[MaxN];

void solve();

i32t main()
{
    int T;
    io.read(T);
    while (T--)
    {
        solve();
        if (T)
            io.print("\n");
    }
    return 0;
}

int dist[MaxN];
int sum[MaxN];
int dp[MaxN];

int getrest(int pos)
{
    return dp[pos] + ai[pos + 1].x + ai[pos + 1].y - dist[pos + 1];
}

int dpq[MaxN];
int qhead, qtail;
void push(int pos)
{
    while (qhead < qtail && getrest(dpq[qtail]) > getrest(pos))
        --qtail;
    dpq[++qtail] = pos;
}
int query(int pos)
{
    while (qhead < qtail && sum[pos] - sum[dpq[qhead]] > c)
        ++qhead;
    return dpq[qhead];
}

SpecRead(Trash) { return io.read(var.x, var.y, var.w); }
void solve()
{
    io.read(c, n);
    io.read(ai, 1, n);
    sum[1] = ai[1].w;
    Rep(i, 2, n)
    {
        dist[i] = dist[i - 1] + abs(ai[i].x - ai[i - 1].x) + abs(ai[i].y - ai[i - 1].y);
        sum[i] = sum[i - 1] + ai[i].w;
    }
    qhead = qtail = 0;
    push(0);
    //echo("rest 0 = %d\n", getrest(0));
    Rep(pos, 1, n)
    {
        int last = query(pos);
        dp[pos] = getrest(last) + ai[pos].x + ai[pos].y + dist[pos];
        //echo("ans at %d: %d(%d), rest %d\n", pos, dp[pos], last, getrest(pos));
        push(pos);
    }
    io.print("$\n", dp[n]);
}

