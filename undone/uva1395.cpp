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

const int MaxN = int(100) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

struct Edge
{
    int src, dst, len;
    friend bool operator<(const Edge &a, const Edge &b)
    {
        return a.len < b.len;
    }
};
Edge eset[MaxN * MaxN];

int n, m;

int par[MaxN], sn;
int getpar(int a)
{
    if (par[a] == a)
        return a;
    else
        return par[a] = getpar(par[a]);
}
void merge(int a, int b)
{
    if (getpar(a) == getpar(b))
        return;
    --sn;
    par[getpar(a)] = getpar(b);
}

i32t main()
{
    while (io.read(n, m) && n != 0)
    {
        for (int i = 0; i < m; ++i)
        {
            int s, t, d;
            io.read(s, t, d);
            eset[i].src = s;
            eset[i].dst = t;
            eset[i].len = d;
        }
        std::sort(eset, eset + m);

        // For beg kruskal
        int ans = Inf;
        Rep(beg, 0, m - 1)
        {
            Rep(i, 1, n)
                par[i] = i;
            sn = n;
            int last = m;
            Rep(ed, beg, m - 1)
            {
                if (sn <= 1)
                    break;
                if (getpar(eset[ed].src) != getpar(eset[ed].dst))
                {
                    last = ed;
                    merge(eset[ed].src, eset[ed].dst);
                }
            }
            if (sn <= 1)
                ans = std::min(ans, eset[last].len - eset[beg].len);
        }
        io.print("$\n", ans == Inf ? -1 : ans);
    }
    return 0;
}

