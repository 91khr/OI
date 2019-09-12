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
#define ImplRead(type, fmt) \
    SpecRead(type) { return scanf(fmt, &var); }
ImplRead(int, "%" SCNd32);
ImplRead(char, " %c");
ImplRead(i64t, "%" SCNd64);
ImplRead(u64t, "%" SCNu64);
ImplRead(f64t, "%lf");
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

const int MaxN = 17;
const int Sigma = 29;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

void solve();

i32t main()
{
    int T;
    io.read(T);
    for (int count = 1; count <= T; ++count)
    {
        io.print("Case $:\n", count);
        solve();
        if (count < T)
            io.print("\n");
    }
    return 0;
}

int n, sl;
char si[MaxN];
int fail[MaxN], jmp[MaxN][Sigma];

i64t gcd(i64t a, i64t b)
{
    if (a < b)
        std::swap(a, b);
    return b == 0 ? a : gcd(b, a % b);
}

struct Frac
{
    i64t up, down;
    Frac(const i64t &u = 0, const i64t &d = 0) : up(u), down(d) {}

    friend void operator+=(Frac &a, const Frac &b)
    {
        i64t div = gcd(a.down, b.down);
        a.up = b.down / div * a.up + a.down / div * b.up;
        a.down *= b.down / div;
    }
    void operator/=(const Frac &o) { *this *= Frac(o.down, o.up); }
    void operator*=(Frac o)
    {
        i64t ud = gcd(up, o.down), dd = gcd(down, o.up);
        up = (up / ud) * (o.up / dd);
        down = (down / dd) * (o.down / ud);
    }
    void operator/=(i64t o) { *this /= Frac(o, 1); }
    void operator*=(i64t o) { *this *= Frac(o, 1); }
};
Frac esp[MaxN];

const char base = 'A';
void solve()
{
    io.read(n, si);
    setq(sl, strlen(si));

    fail[0] = 0;
    memset(jmp, 0, sizeof(jmp));
    jmp[0][si[0] - base] = 1;
    Rep(now, 1, sl - 1)
    {
        int last = fail[now - 1];
        //echo("on %d: ", now);
        while (last && si[last] != si[now])
        {
            int &jpt = jmp[now][si[last] - base];
            if (!jpt)
                jpt = last + 1;
            //echo("%d ", last);
            last = fail[last - 1];
        }
        fail[now] = last + (si[last] == si[now]);
        for (; last; last = fail[last - 1])
        {
            int &jpt = jmp[now][si[last] - base];
            if (!jpt)
                jpt = last + 1;
            //echo("%d ", last);
        }
        //echo("%d ", last);
        int &jpt = jmp[now][si[last] - base];
        if (!jpt)
            jpt = last + 1;
        jmp[now][si[now] - base] = now + 1;
        //echo("\n");
    }
    /*
    Rep(len, 0, sl - 1)
    {
        echo("%d(%d): ", len, fail[len]);
        Rep(ch, 0, n - 1)
            echo("%d ", jmp[len][ch]);
        echo("\n");
    }
    */

    const int CPos = sl + 1;
    Rep(i, 0, sl)
    {
        esp[i].up = 0;
        esp[i].down = 1;
    }
    esp[CPos].up = 0;
    esp[CPos].down = 1;
    Repr(match, sl - 1, 0, -1)
    {
        // Unify: /n
        Rep(i, 0, match)
            if (esp[i].up)
                esp[i] /= n;
        esp[CPos] /= n;
        // Add jmp up
        Rep(ch, 0, n - 1)
            if (jmp[match][ch] != match + 1)
                esp[jmp[match][ch]] += Frac(1, n);
        esp[CPos].up += esp[CPos].down;
        // Reduce this
        Frac fac = Frac(esp[match].down, esp[match].down - esp[match].up);
        // fac is already reduced
        Rep(i, 0, match - 1)
            esp[i] *= fac;
        esp[CPos] *= fac;
    }
    io.print("$\n", esp[CPos].up / esp[CPos].down);
}

