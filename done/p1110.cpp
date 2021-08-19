#include <cstdio>
#include <functional>
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <queue>
#define BUFFERED_IO
namespace Useful_Helpers {
void Unused(...) {}
#if defined(DEBUG) || defined(DEBUG_ECHO)
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#else
  #define echo(...) Unused(__VA_ARGS__)
#endif
#define HELPER_COMBINE_IMPL(a, b) a##b
#define HELPER_COMBINE(a, b) HELPER_COMBINE_IMPL(a, b)
#define HELPER_TMPVARNAME(index) HELPER_COMBINE(HELPER_COMBINE(helper_tmpvar_, __LINE__), HELPER_COMBINE(_, index))
//
#define Reps(var, init, delim, step) for (int var = (init); var <= (delim); var += (step))
#define Repr(var, init, delim, step) for (int var = (init); var >= (delim); var += (step))
#define Rep(var, init, delim) Reps(var, init, delim, 1)
//
using i64t = std::int64_t;
using u64t = std::uint64_t;
using i32t = std::int32_t;
using u32t = std::uint32_t;
using f64t = double;
//
namespace IO_Helper {
template<class T> int read(T &var) { return var.read(); }
template<class T> void print(const T &var) { var.print(); }
// Special cases ><
template<int N> int read(char (&var)[N]);
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
#define SpecRead(type) template<> int read<type>(type &var)
#define SpecPrint(type) template<> void print<type>(const type &var)
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wextra-qualification"
SpecRead(i64t)
{
    int peek;
    while (isspace(peek = io_impl.getch()) && peek != '-')
        ;
    if (peek == EOF) return EOF;
    bool negi;
    if (peek == '-')
    {
        negi = true;
        peek = io_impl.getch();
    }
    else
        negi = false;
    var = 0;
    do
        var = var * 10 + peek - '0';
    while (isdigit(peek = io_impl.getch()));
    if (negi) var = -var;
    return 1;
}
SpecRead(u64t)
{
    int peek;
    while (isspace(peek = io_impl.getch()) && peek != '-')
        ;
    if (peek == EOF) return EOF;
    var = 0;
    do
        var = var * 10 + peek - '0';
    while (isdigit(peek = io_impl.getch()));
    return 1;
}
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
template<typename A, typename B> int read(std::pair<A, B> &p)
{ return io_impl.read(p.first) < 0 ? -1 : io_impl.read(p.second); }
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
#undef ImplPrint
#pragma GCC diagnostic pop
}  // End namespace IO_Helper
[[maybe_unused]] auto &io = IO_Helper::io_impl;
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

[[maybe_unused]] const int MaxN = int(5e5) + 7;
[[maybe_unused]] const i64t Mod = int(1e9) + 7;
[[maybe_unused]] const int Inf = 0x3f3f3f3f;
[[maybe_unused]] const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

std::vector<std::vector<int>> neighbor;
std::set<int> total;
int n, m;

struct MinInfo {
    int val, pos, shift;
    friend bool operator<(const MinInfo &a, const MinInfo &b)
    {
        return a.val > b.val;
    }
    bool vaild() const
    {
        return val == std::abs(shift == 0 ?
                neighbor[pos - 1].back() - neighbor[pos].front() :
                neighbor[pos][shift - 1] - neighbor[pos][shift]);
    }
};
std::priority_queue<MinInfo> sidemin;

int totmin;
void updtot(int val)
{
    if (total.find(val) != total.end())
        return void(totmin = 0);
    total.insert(val);
    auto it = total.find(val);
    int praeval = Inf, postval = Inf;
    if (auto praeptr = it; praeptr != total.begin())
        praeval = std::abs(*--praeptr - val);
    if (auto postptr = it; ++postptr != total.end())
        postval = std::abs(*postptr - val);
    totmin = std::min({ totmin, praeval, postval });
}

int main()
{
    io.read(n, m);
    neighbor.resize(n + 1, std::vector<int>(1));
    totmin = Inf;
    io.read(neighbor[1].front());
    total.insert(neighbor[1].front());
    Rep(i, 2, n)
    {
        int &val = neighbor[i].front();
        // Update side
        io.read(val);
        sidemin.push({ std::abs(val - neighbor[i - 1].back()), i, 0 });
        updtot(val);
    }

    char op[20];
    Rep(i, 1, m)
    {
        io.read<char[20]>(op);
        if (op[0] == 'I')
        {
            int pos, val;
            io.read(pos, val);
            // Update side
            sidemin.push({ std::abs(val - neighbor[pos].back()), pos, static_cast<int>(neighbor[pos].size()) });
            if (pos != n)
                sidemin.push({ std::abs(val - neighbor[pos + 1].front()), pos + 1, 0 });
            neighbor[pos].push_back(val);
            updtot(val);
        }
        else if (op[4] == 'S')  // Query total
            io.print("$\n", totmin);
        else
        {
            while (!sidemin.top().vaild())
                sidemin.pop();
            io.print("$\n", sidemin.top().val);
        }
    }

    return 0;
}

