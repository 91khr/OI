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
ImplRead(i64t, "%" SCNdFAST64);
ImplRead(u64t, "%" SCNuFAST64);
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
ImplPrint(i64t, "%" PRIdFAST64);
ImplPrint(u64t, "%" PRIuFAST64);
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

const int MaxN = int(200) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

void solve();

i32t main()
{
    int T;
    io.read(T);
    while (T--)
        solve();
    return 0;
}

int a, b, c, d;
int dist[MaxN * MaxN];
bool used[MaxN * MaxN];

void solve()
{
    io.read(a, b, c, d);
    memset(dist, Inf, sizeof(dist));
    memset(used, false, sizeof(used));
    int tot = c;
    // Problem answer
    int ans = 0, ansv = 0;
    auto update = [&ans, &ansv] (int val, int nans) {
        if (val <= d && (val > ansv || nans < ans))
        {
            ansv = val;
            ans = nans;
        }
    };
    // State packing
    const int fac[] = { MaxN, 1, 0 };
    // BFS
    typedef std::pair<int, int> handle;
    std::priority_queue<handle, std::vector<handle>, std::greater<handle>>
        bfsq;
#define mp(a, b) std::make_pair(a, b)
    bfsq.push(mp(0, 0));
    dist[0] = 0;
    while (!bfsq.empty())
    {
        int now = bfsq.top().second;
        bfsq.pop();
        if (used[now])
            continue;
        used[now] = true;
    }
    io.print("$ $\n", ans, ansv);
}

