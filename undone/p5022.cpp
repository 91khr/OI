#include <cstdio>
#include <vector>
#include <functional>
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
#define HELPER_COMBINE_IMPL(a, b) a##b
#define HELPER_COMBINE(a, b) HELPER_COMBINE_IMPL(a, b)
#define HELPER_TMPVARNAME(index) HELPER_COMBINE(HELPER_COMBINE(helper_tmpvar_, __LINE__), HELPER_COMBINE(_, index))
//
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
//
namespace RAIIOper
{
class RAIIOper
{
private:
    std::function<void(void)> defer;
    bool cond;
public:
    RAIIOper(std::function<void(void)> cdr, bool wh = true) : defer(cdr), cond(wh) {}
    ~RAIIOper() { if (cond) defer(); }
};
#define PostOper(oper) Useful_Helpers::RAIIOper::RAIIOper HELPER_TMPVARNAME(0)(oper)
#define PostOperIf(cond, oper) \
    Useful_Helpers::RAIIOper::RAIIOper HELPER_TMPVARNAME(0)(oper, cond)
}  // End namespace RAIIOper
} using namespace Useful_Helpers;

const int MaxN = int(5000) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, m;
bool graph[MaxN][MaxN];

struct Edge
{
    int dst;
    Edge *next;
};
Edge eset[MaxN * 2];
Edge *via[MaxN];
int emax;
void addedge(int src, int dst)
{
    eset[++emax].next = via[src];
    via[src] = eset + emax;
    eset[emax].dst = dst;
}

struct
{
private:
    bool isin[MaxN];
    int stack[MaxN], tail;
    bool instack[MaxN];
public:
    bool dfs(int now, int par = 0)
    {
        if (instack[now])
        {
            for (int dst = stack[tail--]; instack[now]; dst = stack[tail--])
            {
                instack[dst] = false;
                isin[dst] = true;
            }
            return true;
        }
        instack[now] = true;
        stack[++tail] = now;
        PostOper(([this, now] {
            --tail;
            instack[now] = false;
        }));
        for (Edge *ed = via[now]; ed; ed = ed->next)
        {
            if (ed->dst == par)
                continue;
            if (dfs(ed->dst, now))
                return true;
        }
        return false;
    }
    bool operator()(int n) { return isin[n]; }
}ring;

bool used[MaxN];
int parent[MaxN];
void findpar(int now)
{
    used[now] = true;
    PostOper([now] { used[now] = false; });
    for (Edge *ed = via[now]; ed; ed = ed->next)
        if (ed->dst != parent[now] && !used[ed->dst])
        {
            parent[ed->dst] = now;
            findpar(ed->dst);
        }
}

void dfs(int now);
void procring(int now);

i32t main()
{
    io.read(n, m);
    Rep(i, 1, m)
    {
        int s, t;
        io.read(s, t);
        graph[s][t] = graph[t][s] = true;
    }
    Rep(i, 1, n) Repr(j, n, 1, -1)
        if (graph[i][j])
            addedge(i, j);
    ring.dfs(1);
    findpar(1);

    if (ring(1))
        procring(1);
    else
        dfs(1);

    return 0;
}

void dfs(int now)
{
    io.print("$ ", now);
    for (Edge *ed = via[now]; ed; ed = ed->next)
        if (ed->dst != parent[now])
        {
            if (ring(ed->dst))
                procring(ed->dst);
            else
                dfs(ed->dst);
        }
}

int iterUntil(int now, int prev, std::function<bool(int, int)> quit)
{
    std::vector<std::pair<int, Edge*>> trace;
    trace.push_back(std::make_pair(prev, nullptr));
    for (int next = now;
            !quit(next, trace.back().second ? trace.back().second->dst : 0) &&
            (now = next); )
    {
        io.print("$ ", now);
        next = 0;
        Edge *ed = via[now];
        for (; ed; ed = ed->next)
            if (ed->dst != trace.back().first)
            {
                if (ring(ed->dst))
                    next = ed->dst;
                else if (next == 0)
                    dfs(ed->dst);
                else
                    break;
            }
        trace.push_back(std::make_pair(now, ed));
    }
    for (; !trace.empty(); trace.pop_back())
        for (Edge *ed = trace.back().second; ed; ed = ed->next)
            if (ed->dst != trace.back().first)
                dfs(ed->dst);
    return now;
}

int procring(int now)
{
    io.print("$ ", now);
    int left = 0, right = 0;
    Edge *rest = nullptr;
    for (Edge *ed = via[now]; ed; ed = ed->next)
        if (ed->dst != parent[now])
        {
            if (ring(ed->dst))
                if (left == 0)
                    left = ed->dst;
                else
                {
                    right = ed->dst;
                    break;
                }
            else if (left == 0)
                dfs(ed->dst);
            else if (rest == nullptr)
                rest = ed;
        }
    int delim = iterUntil(left, now, [right] (int now, int prev) {
        static int back = right;
        if (prev)
            back = prev;
        return now > back;
    });
}

