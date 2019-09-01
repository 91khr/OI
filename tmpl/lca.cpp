#include <cstdio>
#include <cstring>
#include <algorithm>
namespace Useful_Helpers {
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif
#define Rep(var, init, delim) for (int var = init; var <= delim; ++var)
#define Reps(var, init, delim, step) for (int var = init; var <= delim; var += step)
#define Repr(var, init, delim) for (int var = init; var >= delim; --var)
//#define int long long
template<typename DstTy, typename SrcTy>
DstTy set(DstTy &dst, const SrcTy &src) { return dst = static_cast<DstTy>(src); }

typedef long long i64t;
typedef unsigned long long u64t;
typedef unsigned u32t;
typedef int i32t;

namespace IO_Helper {
template<typename Ty>
void read(Ty &var);  // Read a variable

#define ImplRead(type, fmt) \
    template<> void read<type>(type& i) { scanf(fmt, &i); }
ImplRead(int, "%d");
ImplRead(char, "%c");
ImplRead(long long, "%lld");
template<> void read<char*>(char *&i) { scanf("%s", i); }
template<int Size> void read(char (&i)[Size]) { scanf("%s", i); }
#undef ImplRead

struct IO {
private:
    void read() {}
public:
    template<typename Ty, typename ...Rest>
    void read(Ty &car, Rest& ...cdr) { IO_Helper::read(car); this->read(cdr...); }
    template<typename Ty, int Size>
    void read(Ty (&arr)[Size], int beg, int end) { Rep(i, beg, end) this->read(arr[i]); }
}io;
}
using IO_Helper::io;
} using namespace Useful_Helpers;

const int MaxN = int(5e5) + 7;
const int MaxT = 40;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

struct Edge
{
    int dst, next;
};
Edge eset[MaxN * 2];
int via[MaxN];
int emax;
void addedge(int src, int dst)
{
    eset[++emax].dst = dst;
    eset[emax].next = via[src];
    via[src] = emax;
}

int n, m, s;

int depth[MaxN];
int grand[MaxN][MaxT];

void dfs(int now, int dep)
{
    depth[now] = dep;
    // Preproc grand
    for (int tm = 1; ; ++tm)
    {
        grand[now][tm] = grand[grand[now][tm - 1]][tm - 1];
        if (grand[now][tm] == -1)
            break;
    }
    // Proc child
    for (int ed = via[now]; ed; ed = eset[ed].next)
    {
        const int next = eset[ed].dst;
        if (next != grand[now][0])
        {
            grand[next][0] = now;
            dfs(next, dep + 1);
        }
    }
}

int lca(int a, int b)
{
    if (depth[a] > depth[b])
        std::swap(a, b);
    // depth[b] >= depth[a]
    Repr(tm, MaxT - 3, 0)
    {
        if (grand[b][tm] != -1 && depth[grand[b][tm]] >= depth[a])
            b =  grand[b][tm];
    }
    // depth[b] == depth[a]
    Repr(tm, MaxT - 3, 0)
    {
        if (grand[a][tm] != grand[b][tm])
        {
            a = grand[a][tm];
            b = grand[b][tm];
        }
    }
    // if not equal, they must be brothers
    if (a != b)
        a = grand[a][0];
    return a;
}

i32t main()
{
    io.read(n, m, s);
    Rep(i, 1, n - 1)
    {
        int a, b;
        io.read(a, b);
        addedge(a, b);
        addedge(b, a);
    }
    // Preproc depth and grand
    memset(grand, -1, sizeof(grand));
    dfs(s, 0);

    Rep(count, 1, m)
    {
        int a, b;
        io.read(a, b);
        printf("%d\n", lca(a, b));
    }
    return 0;
}

