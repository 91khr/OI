#include <cstdio>
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
#define Repr(var, init, delim, step) for (int var = (init); var >= (delim); var += (step))
#define Rep(var, init, delim) Reps(var, init, delim, 1)
template<typename DstTy, typename SrcTy>
DstTy setq(const DstTy &dst, const SrcTy &src) { return const_cast<DstTy&>(dst) = static_cast<DstTy>(src); }
template<typename DstTy, typename SrcTy, typename ...Rest>
DstTy setq(const DstTy &dst, const SrcTy &src, const Rest &...cdr) { setq(cdr...); return setq(dst, src); }
//
typedef std::int64_t i64t;
typedef std::uint64_t u64t;
typedef std::uint32_t u32t;
typedef std::int32_t i32t;
typedef double f64t;
//
namespace IO_Helper {
template<typename Ty>
int read(Ty &var);  // Read a variable
//
#define SpecRead(type) template<> int Useful_Helpers::IO_Helper::read<type>(type &var)
template<typename IType>
int read_int(IType &var)
{
    int peek;
    while (isspace(peek = getchar()) && peek != '-')
        ;
    if (peek == EOF) return EOF;
    bool negi;
    if (peek == '-')
    {
        negi = true;
        peek = getchar();
    }
    else
        negi = false;
    var = 0;
    do
        var = var * 10 + peek - '0';
    while (isdigit(peek = getchar()));
    if (negi) var = -var;
    return 1;
}
SpecRead(i32t) { return read_int(var); }
SpecRead(u32t) { return read_int(var); }
SpecRead(i64t) { return read_int(var); }
SpecRead(u64t) { return read_int(var); }
SpecRead(char) { while (isspace(var = getchar()) && var != EOF); return var == EOF ? EOF : 1; }
SpecRead(f64t) { return scanf("%lf", &var); }
SpecRead(char*) { return scanf("%s", var); }
template<int Size> int read(char (&i)[Size]) { return scanf("%s", i); }
template<typename A, typename B> int read(std::pair<A, B> &p)
{ return read(p.first), read(p.second); }
#undef ImplRead
template<typename Ty>
void print(const Ty &var);
#define SpecPrint(type) template<> void Useful_Helpers::IO_Helper::print<type>(const type &var)
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
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

const int MaxN = int(1e4) + 7;
const int MaxM = int(1e5) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, m;
int weight[MaxN];

struct Edge
{
    int dst;
    int next;
};
Edge eset[MaxM * 2];
int via[MaxN];
int emax;
void addedge(int src, int dst)
{
    eset[++emax].dst = dst;
    eset[emax].next = via[src];
    via[src] = emax;
}

class UnionSet
{
private:
    int parent[MaxN];
public:
    void init() { Rep(i, 1, n) parent[i] = i; }
    int getpar(int now)
    {
        if (parent[now] == now)
            return now;
        else
            return parent[now] = getpar(parent[now]);
    }
    void merge(int dst, int src)
    {
        parent[getpar(src)] = getpar(dst);
    }
};
UnionSet set;

namespace Shrink
{
int clock;
int dfn[MaxN], low[MaxN];
int stack[MaxN], tail;
bool instk[MaxN];
void dfs(int now)
{
    dfn[now] = low[now] = ++clock;
    stack[++tail] = now;
    instk[now] = true;
    for (int ed = via[now]; ed; ed = eset[ed].next)
    {
        const int dst = eset[ed].dst;
        if (!dfn[dst])
        {
            dfs(dst);
            low[now] = std::min(low[now], low[dst]);
        }
        else if (instk[dst])
            low[now] = std::min(low[now], dfn[dst]);
    }
    if (low[now] == dfn[now])
    {
        for (int next = stack[tail]; tail && next != now; next = stack[--tail])
        {
            instk[next] = false;
            set.merge(now, next);
            weight[now] += weight[next];
            for (int ed = via[next]; ed; ed = eset[ed].next)
                addedge(now, eset[ed].next);
        }
    }
}
}

int ans;
int dp[MaxN];

int dfs(int now)
{
    if (dp[now] != -1)
        return dp[now];
    int res = weight[now];
    for (int ed = via[now]; ed; ed = eset[ed].next)
    {
        const int dst = set.getpar(eset[ed].dst);
        if (dst != now)
            res = std::max(res, weight[now] + dfs(dst));
    }
    ans = std::max(ans, dp[now] = res);
    return res;
}

i32t main()
{
    io.read(n, m);
    io.read(weight, 1, n);
    Rep(i, 1, m)
    {
        int s, t;
        io.read(s, t);
        addedge(s, t);
    }
    set.init();

    Rep(i, 1, n)
        if (Shrink::dfn[i] == 0)
            Shrink::dfs(i);
    //echo("end shrink\n");

    memset(dp, -1, sizeof(dp));
    Rep(i, 1, n)
        if (dp[set.getpar(i)] == -1)
            //echo("%d(%d)\n", dfs(set.getpar(i)), set.getpar(i));
            dfs(set.getpar(i));

    io.print("$\n", ans);

    return 0;
}

