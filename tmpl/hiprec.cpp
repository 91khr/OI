#include <cstdio>
#include <cassert>
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

const int MaxN = 2500 + 7;

class BigInt
{
public:
    static const int Lim = int(1e9);
    static const int BitSize = 9;

public:
    int data[MaxN];
    int len;
    static BigInt res;

    BigInt(const BigInt &o) { *this = o; }
    BigInt &operator=(const BigInt &o)
    {
        len = o.len;
        Rep(i, 0, len - 1)
            data[i] = o.data[i];
        return *this;
    }

    static int unify(i64t val, int &res)
    {
        res = val % Lim;
        return val / Lim;
    }

public:
    BigInt(int v) : len(1) { *data = v; }
    BigInt() : len(0) {}

    BigInt &operator+=(const BigInt &other);
    BigInt &operator-=(const BigInt &other);
    BigInt &operator*=(const BigInt &other) { return *this = *this * other; }
    BigInt &operator/=(const BigInt &other) { return *this = *this / other; }

    friend BigInt operator+(const BigInt &a, const BigInt &b)
    {
        res = a;
        res += b;
        return res;
    }
    friend BigInt operator-(const BigInt &a, const BigInt &b)
    {
        res = a;
        res -= b;
        return res;
    }
    friend BigInt operator*(const BigInt &a, const BigInt &b);
    friend BigInt operator/(const BigInt &a, const BigInt &b);

    friend bool operator<(const BigInt &a, const BigInt &b)
    {
        if (a.len != b.len)
            return a.len < b.len;
        Repr(pos, a.len - 1, 0, -1)
            if (a.data[pos] != b.data[pos])
                return a.data[pos] < b.data[pos];
        return false;
    }
    friend bool operator==(const BigInt &a, const BigInt &b)
    {
        if (a.len != b.len)
            return false;
        Rep(pos, 0, a.len - 1)
            if (a.data[pos] != b.data[pos])
                return false;
        return true;
    }
    friend bool operator!=(const BigInt &a, const BigInt &b) { return !(a == b); }
    friend bool operator<=(const BigInt &a, const BigInt &b) { return a < b || a == b; }
    friend bool operator>=(const BigInt &a, const BigInt &b) { return !(a < b); }
    friend bool operator>(const BigInt &a, const BigInt &b) { return !(a <= b); }
    operator bool() const { return len; }

    void print() const
    {
        if (len >= 1)
            printf("%d", data[len - 1]);
        else
            putchar('0');
        Repr(i, len - 2, 0, -1)
            printf("%09d", data[i]);
    }
    void read()
    {
        int peek;
        while (!isdigit(peek = getchar()))
            ;
        // Scan reversly
        int upper = 1;
        res.data[res.len = 0] = 0;
        do
        {
            res.data[res.len] = res.data[res.len] * 10 + peek - '0';
            if ((upper *= 10) >= Lim)
            {
                upper = 1;
                res.data[++res.len] = 0;
            }
        } while (isdigit(peek = getchar()));
        /*
        ++res.len;
        io.print("res: $\n", res);
        --res.len;
        */
        // Copy into this
        int bit = 1;
        for (upper /= 10; res.len >= 0; --res.len, upper = Lim / 10)
        {
            int now = res.data[res.len];
            for (; upper; upper /= 10, now /= 10)
            {
                data[len] += bit * (now % 10);
                if ((bit *= 10) >= Lim)
                {
                    bit = 1;
                    data[++len] = 0;
                }
            }
        }
        if (bit != 1)
            ++len;
    }

    BigInt &lsh();
    BigInt &rsh();
};
BigInt BigInt::res;

SpecPrint(BigInt) { var.print(); }
SpecRead(BigInt) { var.read(); return 1; }

BigInt &BigInt::operator+=(const BigInt &other)
{
    int carry = 0;
    int i = 0;
    for (; i < len || i < other.len || carry; ++i)
    {
        if (i >= len)
            data[i] = 0;
        const int val = i < other.len ? other.data[i] : 0;
        carry = unify(i64t(data[i]) + val + carry, data[i]);
    }
    len = i;
    return *this;
}
BigInt &BigInt::operator-=(const BigInt &other)
{
    int carry = 0;
    for (int i = 0; i < other.len || carry; ++i)
    {
        const int val = i < other.len ? other.data[i] : 0;
        carry = !unify(i64t(data[i]) + Lim - val - carry, data[i]);
    }
    if (data[len - 1] == 0)
        --len;
    return *this;
}

BigInt operator*(const BigInt &a, const BigInt &b)
{
    static BigInt inter;
    BigInt &res = BigInt::res;
    res.len = 0;
    Rep(pos, 0, b.len - 1)
    {
        const int fac = b.data[pos];
        int carry = 0;
        Rep(i, 0, pos - 1)
            inter.data[i] = 0;
        int i;
        for (i = 0; i < a.len || carry; ++i)
        {
            const int val = i >= a.len ? 0 : a.data[i];
            carry = a.unify(i64t(fac) * val + carry, inter.data[i + pos]);
        }
        inter.len = i + pos;
        res += inter;
    }
    return res;
}

BigInt &BigInt::lsh()
{
    int carry = 0;
    int i;
    data[len] = 0;
    for (i = 0; i < len || carry; ++i)
        carry = unify(data[i] * 2ll + carry, data[i]);
    len = i;
    return *this;
}
BigInt &BigInt::rsh()
{
    data[0] /= 2;
    Rep(i, 1, len - 1)
    {
        data[i - 1] += Lim / 2 * (data[i] % 2);
        data[i] /= 2;
    }
    if (data[len - 1] == 0)
        --len;
    return *this;
}

BigInt operator/(const BigInt &a, const BigInt &b)
{
    BigInt &res = BigInt::res;
    static BigInt pivot, minus, fac;
    pivot = a;
    minus = b;
    res.len = 0;
    // Get the hightest bit of a / b
    for (fac = BigInt(1); minus < pivot; fac.lsh(), minus.lsh())
        ;
    // Attemp
    for (; fac; fac.rsh(), minus.rsh())
        if (minus <= pivot)
        {
            pivot -= minus;
            res += fac;
        }
    return res;
}

i32t main()
{
    BigInt i, j;
    io.read(i, j);
    //io.print("$ $\n", i, j);
    BigInt ans(i / j);
    //io.print("$ $\n", i, ans * j);
    io.print("$\n", ans);
    return 0;
}

