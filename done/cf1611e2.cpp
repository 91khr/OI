#include <cstdio>
#include <functional>
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <algorithm>
#include <queue>
#define BUFFERED_IO
namespace Useful_Helpers {
void Unused(...) {}
#if defined(DEBUG) || defined(DEBUG_ECHO)
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#else
  #define echo(...) Unused(__VA_ARGS__)
#endif
//
#define Rep(var, init, delim) for (int var = (init); var <= (delim); ++var)
//
using i64t = std::int64_t;
using u64t = std::uint64_t;
using i32t = std::int32_t;
using u32t = std::uint32_t;
using f64t = double;
//
namespace IO_Helper {
template<class T> inline int read(T &var) { return var.read(); }
template<class T> inline void print(const T &var) { var.print(); }
// Special cases ><
template<int N> int read(char (&var)[N]);  // Forwards to read<char*>
template<typename A, typename B> inline int read(std::pair<A, B> &p)
{ return read(p.first) < 0 ? -1 : read(p.second); }
template<> void print<const char *>(const char *const &var) { fputs(var, stdout); }
//
struct IO {
private:
#ifdef BUFFERED_IO
    char buf[BUFSIZ], *endbuf = buf, *it = buf;
    int getch()
    {
        if (++it >= endbuf)
            return (endbuf = buf + fread(buf, 1, BUFSIZ, stdin)) == buf ? EOF : *(it = buf);
        return *it;
    }
#else  // !defined(BUFFERED_IO)
    int getch() { return getchar(); }
#endif  // defined(BUFFERED_IO)
    int read() { return 0; }
public:
    template<class T> friend int read(T &);
    template<class Ty, int Size>
    int read(Ty (&arr)[Size], int beg, int end)
    { Rep(i, beg, end) if (IO_Helper::read(arr[i]) < 0) return -1; return 1; }
    template<class First, class ...Rest>
    int read(First &car, Rest& ...cdr)
    { return IO_Helper::read(car) + read(cdr...); }
    template<class T> friend void print(const T &);
    void print(const char *fmt) { fputs(fmt, stdout); }
    template<class First, class ...Rest>
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
            if constexpr (sizeof...(cdr))
                this->print(fwd + 1, cdr...);
            else
                IO_Helper::print(fwd + 1);
        }
    }
}io_impl;
//
#define SpecRead(type) template<> inline int read<type>(type &var)
#define READINT_COMMON_BEGIN \
    int peek; \
    while (isspace(peek = io_impl.getch()) && peek != '-') \
        ; \
    if (peek == EOF) return EOF
#define READINT_COMMON_END \
    var = 0; \
    do \
        var = var * 10 + peek - '0'; \
    while (isdigit(peek = io_impl.getch()))
SpecRead(i64t)
{
    READINT_COMMON_BEGIN;
    bool negi = peek == '-' ? (peek = io_impl.getch()) && true : false;
    READINT_COMMON_END;
    if (negi) var = -var;
    return 1;
}
SpecRead(u64t)
{
    READINT_COMMON_BEGIN;
    READINT_COMMON_END;
    return 1;
}
#undef READINT_COMMON_BEGIN
#undef READINT_COMMON_END
SpecRead(i32t) { i64t v; if (read<i64t>(v) > 0) return var = v, 1; else return -1; }
SpecRead(u32t) { u64t v; if (read<u64t>(v) > 0) return var = v, 1; else return -1; }
SpecRead(char) { while (isspace(var = io_impl.getch()) && var != EOF) {} return 1; }
SpecRead(char*)
{
    read(var[0]);
    char *it;
    for (it = var + 1; !isspace(*it = io_impl.getch()); ++it)
        ;
    *it = '\0';
    return 1;
}
template<int N> int read(char (&var)[N]) { char *ptr = var; return read(ptr); }
#if !defined(BUFFERED_IO)
SpecRead(f64t) { return scanf("%lf", &var); }
#endif  // !defined(BUFFERED_IO)
#undef SpecRead
#define ImplPrint(type, fmt) template<> inline void print<type>(const type &var) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
#undef ImplPrint
}  // End namespace IO_Helper
[[maybe_unused]] auto &io = IO_Helper::io_impl;
} using namespace Useful_Helpers;

[[maybe_unused]] const int MaxN = int(2e5) + 7;
[[maybe_unused]] const i64t Mod = int(1e9) + 7;
[[maybe_unused]] const int Inf = 0x3f3f3f3f;
[[maybe_unused]] const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int T;
int n, k;
bool fri[MaxN];

struct Edge
{
    int dst, next;
};
Edge eset[MaxN * 2];
int emax, via[MaxN];
void eadd(int s, int d)
{
    eset[++emax].dst = d;
    eset[emax].next = via[s];
    via[s] = emax;
}

int depth[MaxN], parent[MaxN], out[MaxN];
void dfs(int now)
{
    out[now] = 0;
    for (Edge *ed = eset + via[now]; ed != eset; ed = eset + ed->next)
        if (ed->dst != parent[now])
        {
            ++out[now];
            depth[ed->dst] = depth[now] + 1;
            parent[ed->dst] = now;
            dfs(ed->dst);
        }
}

int ans[MaxN], mintm[MaxN], rest[MaxN];
std::queue<int> bfsq;

void solve()
{
    io.read(n, k);
    memset(fri, false, sizeof(fri));
    echo("graph G%d {\n", T);
    while (k--)
    {
        int i;
        io.read(i);
        fri[i] = true;
        echo("%d [label=\"%d:o\"]\n", i, i);
    }
    memset(via, 0, sizeof(via));
    emax = 0;
    Rep(i, 2, n)
    {
        int s, d;
        io.read(s, d);
        eadd(s, d);
        eadd(d, s);
        echo("%d -- %d\n", s, d);
    }
    echo("}\n");

    depth[1] = 0;
    dfs(1);
    memset(ans, 0, sizeof(ans));
    memset(mintm, Inf, sizeof(mintm));
    Rep(i, 1, n)
    {
        rest[i] = out[i];
        if (out[i] == 0)
        {
            bfsq.push(i);
            rest[i] = 1;
        }
    }
    ++out[1];
    while (!bfsq.empty())
    {
        int now = bfsq.front(), par = parent[now];
        bfsq.pop();
        if (fri[now])
        {
            ans[now] = 1;
            mintm[now] = 0;
            rest[now] = 0;
        }
        echo("%d: %d %d %d\n", now, mintm[now], rest[now], ans[now]);
        if (mintm[now] + 1 <= std::min(depth[par], mintm[par]))
        {
            ans[par] = 1;
            mintm[par] = mintm[now] + 1;
            rest[par] = 0;
        }
        else if (rest[now] == 0 && rest[par] != 0)
        {
            ans[par] += ans[now];
            --rest[par];
        }
        if (--out[par] == 0)
            bfsq.push(par);
    }
    echo("%d %d\n", rest[1], ans[1]);
    io.print("$\n", rest[1] == 0 ? ans[1] : -1);
}

int main()
{
    io.read(T);
    while (T--)
        solve();
    return 0;
}

