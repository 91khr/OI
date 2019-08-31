//#define Codeforces
#include <cstdio>
#include <cassert>
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
//
typedef long long i64t;
typedef unsigned long long u64t;
typedef unsigned u32t;
typedef int i32t;
//
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
//
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

const int MaxN = int(1e5) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

char str[3][MaxN];
int len[3];
// isin[i][j] -> i is inside j
bool isin[3][3];

int fail[MaxN];
int getcom(int fr, int bk)
{
    const char *const prev = str[fr], *const post = str[bk];
    fail[0] = 0;
    for (int pos = 1; post[pos]; ++pos)
    {
        int last = fail[pos - 1];
        while (last && post[last] != post[pos])
            last = fail[last - 1];
        fail[pos] = last + (post[last] == post[pos]);
    }
    int match = 0;
    for (int pos = 1; prev[pos]; ++pos)
    {
        while (match && post[match] != prev[pos])
            match = fail[match - 1];
        if (post[match] == prev[pos])
            ++match;
        if (post[match] == '\0')
        {
            isin[bk][fr] = true;
            if (prev[pos + 1])
                match = fail[match];
        }
    }
    return match;
}

int dist[3][3];

i32t main()
{
    io.read(str, 0, 2);
    Rep(i, 0, 2)
        set(len[i], strlen(str[i]));
    // Preproc containing and common component
    Rep(i, 0, 2)
    {
        Rep(j, 0, 2)
            if (i != j)
                dist[i][j] = getcom(i, j);
    }
    // Iterate to calc
    int ans = Inf;
    Rep(i, 0, 2)
        Rep(j, 0, 2)
        {
            if (i == j)
                continue;
        }
    printf("%d\n", ans);
    return 0;
}

/*
 * Cases:
 *   - Independent: ab cd ef
 *   - Partial intersect: ab bc cd
 *   - Partial intersect 2: abbc bbccd bccda
 *   - Include: ab abbc bcab
 *   - Include 2: ab abbccd bccd
 */

