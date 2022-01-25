#include <cstdio>
#include <functional>
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <algorithm>
#if !defined(DEBUG)
#    define BUFFERED_IO
#endif  // DEBUG
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

[[maybe_unused]] const int MaxN = int(1e5) + 7;
[[maybe_unused]] const i64t Mod = int(1e9) + 7;
[[maybe_unused]] const int Inf = 0x3f3f3f3f;
[[maybe_unused]] const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

i64t exgcd(i64t x, i64t y, i64t &a, i64t &b)
{
    if (y == 0)
    {
        a = 1;
        b = 0;
        return x;
    }
    int r = exgcd(y, x % y, b, a);
    b -= x / y * a;
    return r;
}

int main()
{
    i64t n, m, x, y, vx, vy;
    io.read(n, m, x, y, vx, vy);
    if (vx == 0)
    {
        if (x == 0 || x == n)
            io.print("$ $\n", x, y == 0 || y == m ? y : vy > 0 ? m : 0);
        else
            io.print("-1\n");
        return 0;
    }
    if (vy == 0)
    {
        if (y == 0 || y == m)
            io.print("$ $\n", x == 0 || x == n ? x : vx > 0 ? n : 0, y);
        else
            io.print("-1\n");
        return 0;
    }

    i64t p, q, r, d = x * vx - y * vy;
    r = exgcd(n * vx, -m * vy, p, q);
    if (d % r)
    {
        io.print("-1\n");
        return 0;
    }
    p *= d / r;
    q *= d / r;
    echo("%ld %ld\n", p, q);
    r = std::abs(r);
    // vx * (p * n - x) < 0
    if (p * vx < 0 || (p == 0 && x * vx > 0))
    {
        i64t diff = -vx * p / (m / r) + !(p % (m / r) == 0 && p != 0);
        echo("%d 1\n", vx * (p * n - x) < 0);
        p += diff * (m / r) * vx;
        q += diff * (n / r) * vy;
    }
    else
    {
        // (p * n - x) / lcm
        i64t diff = vx * p / (m / r) - (p % (m / r) == 0 && p != 0);
        echo("%d 0 %ld\n", vx * (p * n - x) < 0, diff);
        p -= diff * (m / r) * vx;
        q -= diff * (n / r) * vy;
    }
    echo("%ld %ld\n", p, q);
    io.print("$ $\n", p % 2 ? n : 0, q % 2 ? m : 0);
}

