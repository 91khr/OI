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

const int MaxN = int(2e5) + 7;
const numt Mod = int(1e9) + 7;
const numt Inf = 0x3f3f3f3f3f3f3f3f;

int n, k;
i64t ai[MaxN];

i32t main()
{
    io.read(n, k);
    Rep(i, 1, n)
        io.read(ai[i]);
    std::sort(ai + 1, ai + n + 1);
    ai[n + 1] = ai[n];
    i64t tot = 1, sum = 0;
    Rep(pos, n / 2 + 1, n)
    {
        if ((ai[pos + 1] - ai[pos]) * tot + sum > k)
        {
            printf("%I64d\n", ai[pos] + (k - sum) / tot);
            return 0;
        }
        sum += tot * (ai[pos + 1] - ai[pos]);
        ++tot;
    }
    --tot;
    printf("%I64d\n", ai[n] + (k - sum) / tot);
    return 0;
}

