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
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

struct Edge
{
    int dst, len;
    int next;
};
Edge eset[MaxN * 5];
int via[MaxN];
int emax = 0;
void addedge(int src, int dst, int len)
{
    eset[++emax].dst = dst;
    eset[emax].len = len;
    eset[emax].next = via[src];
    via[src] = emax;
}

int n, m;

int dist[MaxN];
bool used[MaxN];
bool spfa(int now)
{
    used[now] = true;
    for(int i = via[now]; i; i = eset[i].next)
    {
        const int next = eset[i].dst;
        if(dist[next] > dist[now] + eset[i].len)
        {
            dist[next] = dist[now] + eset[i].len;
            if(used[next])
                return true;
            if(spfa(next))
                return true;
        }
    }
    used[now] = false;
    return false;
}

i32t main()
{
    // Read data
    io.read(n, m);
    // Init graph
    Rep(i, 2, n + 1)
        addedge(1, i, 0);
    Rep(i, 1, m)
    {
        int op, a, b;
        io.read(op, a, b);
        ++a; ++b;
        if (op == 3)
        {
            addedge(a, b, 0);
            addedge(b, a, 0);
            continue;
        }
        int c;
        io.read(c);
        // a >= b + c => b <= a - c
        if (op == 1)
            addedge(a, b, -c);
        // a <= b + c
        else
            addedge(b, a, c);
    }
    //Rep(i, 1, n + 1)
        //dist[i] = Inf;
    echo("end scan\n");

    Rep(i, 1, n + 1)
        if (spfa(i))
        {
            printf("No\n");
            return 0;
        }
    printf("Yes\n");

    return 0;
}

