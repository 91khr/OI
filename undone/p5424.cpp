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
#define Rep(var, init, delim) Reps(var, init, delim, 1)
#define Repr(var, init, delim) for (int var = (init); var >= (delim); --var)
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

const int MaxN = int(400) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, k;
int ai[MaxN];
int maxv[MaxN][MaxN];
int sum[MaxN];
int dp[MaxN][MaxN];

i32t main()
{
    io.read(n, k);
    ++k;
    io.read(ai, 1, n);
    Rep(i, 1, n)
    {
        sum[i] = sum[i - 1] + ai[i];
        maxv[i][i] = ai[i];
        Rep(j, i, n)
            maxv[i][j] = std::max(maxv[i][j - 1], ai[j]);
    }

    Rep(pos, 2, n)
        dp[pos][1] = maxv[1][pos] * pos - sum[pos];
    Rep(rest, 1, k)
        dp[1][rest] = 0;
    Rep(pos, 2, n)
    {
        Rep(rest, 2, k)
        {
            int &ans = dp[pos][rest];
            ans = Inf;
            Repr(left, pos - 1, 1)
                ans = std::min(ans, dp[left][rest - 1] + maxv[left + 1][pos] * (pos - left) - (sum[pos] - sum[left]));
            echo("ans of %d %d: %d\n", pos, rest, ans);
        }
    }

    io.print("$\n", dp[n][k]);
    return 0;
}

