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
typedef size_t idxt;
typedef i64t numt;

namespace IO_Helper {
template<typename Ty>
void read(Ty &var);  // Read a variable

#define ImplRead(type, fmt) \
    template<> void read<type>(type& i) { scanf(fmt, &i); }
ImplRead(int, "%d");
ImplRead(long long, "%I64d");
template<> void read<char*>(char *&i) { scanf("%s", i); }
template<int Size> void read(char (&i)[Size]) { scanf("%s", i); }
#undef ImplRead

struct IO {
private:
    void read() {}
public:
    template<typename Ty, typename ...Rest>
    void read(Ty &car, Rest& ...cdr) { IO_Helper::read(car); this->read(cdr...); }
}io;
}
using IO_Helper::io;
} using namespace Useful_Helpers;

const int MaxN = int(1000) + 7;
const numt Mod = int(1e9) + 7;
const numt Inf = 0x3f3f3f3f * (sizeof(numt) == sizeof(i64t) ? 0x0000000100000001 : 1);

int n, m;
char si[MaxN][MaxN];
int ai[MaxN];
int chose[MaxN][5];

i32t main()
{
    io.read(n, m);
    Rep(i, 1, n)
        io.read(si[i]);
    Rep(i, 1, m)
        io.read(ai[i]);
    Rep(man, 1, n)
    {
        Rep(prob, 1, m)
            ++chose[prob][si[man][prob - 1] - 'A'];
    }
    int ans = 0;
    Rep(prob, 1, m)
    {
        int maxs = 0;
        Rep(sel, 0, 4)
            maxs = std::max(maxs, chose[prob][sel]);
        echo("prob %d, maxs %d, sc %d\n", prob, maxs, ai[prob]);
        ans += maxs * ai[prob];
    }
    printf("%d\n", ans);
    return 0;
}

