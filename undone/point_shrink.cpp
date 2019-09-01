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

const int MaxN = int(1e4) + 7;
const int MaxM = int(1e5) + 7;
const int Inf = 0x3f3f3f3f;

struct Edge
{
    int dst, next;
};
namespace Origin
{
Edge eset[MaxM * 2];
int via[MaxN];
int emax;
void addedge(int src, int dst)
{
    eset[++emax].dst = dst;
    eset[emax].next = via[src];
    via[src] = emax;
}
}  // End namespace Origin

int n, m;
int weight[MaxN];

namespace Shrink
{
int stamp, dfn[MaxN], low[MaxN];
int path[MaxN], npath;
int id[MaxN], maxid;
void dfs(int now)
{
    using namespace Origin;
    dfn[now] = low[now] = ++stamp;
    path[++npath] = now;
    // Iterate all child
    for (int ed = via[now]; ed; ed = eset[ed].next)
    {
        const int next = eset[ed].dst;
        if (dfn[next] == 0)
        {
            dfs(next);
            low[now] = std::min(low[now], low[next]);
        }
        else if (id[next] == 0)
        {
            low[now] = std::min(low[now], dfn[next]);
        }
    }
    // Collect component
    if (dfn[now] == low[now])
    {
        ++maxid;
        while (true)
        {
            const int last = path[npath--];
            id[last] = maxid;
            if (last == now)
                break;
        }
    }
}
}  // End namespace Shrink

i32t main()
{
    // Build graph
    io.read(n, m);
    io.read(weight, 1, n);
    Rep(i, 1, m)
    {
        int u, v;
        io.read(u, v);
        Origin::addedge(u, v);
    }

    // Find link component
    Rep(now, 1, n)
    {
        if (Shrink::dfn[now] == 0)
            Shrink::dfs(now);
    }

    Rep(i, 1, n)
        echo("%d ", Shrink::id[i]);
    echo("\n");

    return 0;
}

