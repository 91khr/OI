#include <cstdio>
#include <cmath>
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

const int MaxN = int(2e5) + 7;
const numt Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, m, k, q;
int range[MaxN][2];
int up[MaxN];
i64t dp[MaxN][2];

int reach(int tar, int p)
{
    int *ch = std::lower_bound(up + 1, up + q + 1, p);
    int res = Inf;
    if (ch != up + q + 1)
        res = std::min(res, abs(tar - *ch) + abs(*ch - p));
    if (ch != up + 1)
        res = std::min(res, abs(tar - ch[-1]) + abs(ch[-1] - p));
    echo("reach from %d to %d is %d\n", p, tar, res);
    return res;
}

i32t main()
{
    io.read(n, m, k, q);
    Rep(i, 1, n)
    {
        range[i][0] = Inf;
        range[i][1] = 0;
    }
    Rep(i, 1, k)
    {
        int x, y;
        io.read(x, y);
        range[x][0] = std::min(range[x][0], y);
        range[x][1] = std::max(range[x][1], y);
    }
    Rep(i, 1, q)
        io.read(up[i]);
    std::sort(up + 1, up + q + 1);

    range[1][0] = std::min(range[1][0], 1);
    range[1][1] = std::max(range[1][1], 1);
    dp[1][0] = abs(range[1][1] - 1) + abs(range[1][1] - range[1][0]);
    dp[1][1] = abs(range[1][1] - 1);
    int prev = 1;
    Rep(line, 2, n)
    {
        if (range[line][0] == Inf)
            continue;
        i64t ranlen = abs(range[line][1] - range[line][0]);
        Rep(s, 0, 1)
        {
            int tar = range[line][s ^ 1];
            dp[line][s] = ranlen + line - prev +
                std::min(reach(tar, range[prev][0]) + dp[prev][0],
                        reach(tar, range[prev][1]) + dp[prev][1]);
            echo("dp %d %d to %I64d\n", line, s, dp[line][s]);
        }
        prev = line;
    }
    printf("%I64d\n", std::min(dp[prev][0], dp[prev][1]));
    return 0;
}

