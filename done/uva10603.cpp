#include <cstdio>
#include <queue>
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
DstTy setq(const DstTy &dst, const SrcTy &src) { return const_cast<DstTy&>(dst) = static_cast<DstTy>(src); }
template<typename DstTy, typename SrcTy, typename ...Rest>
DstTy setq(const DstTy &dst, const SrcTy &src, const Rest &...cdr) { setq(cdr...); return setq(dst, src); }
//
typedef std::int64_t i64t;
typedef std::uint64_t u64t;
typedef std::uint32_t u32t;
typedef std::int32_t i32t;
typedef double f64t;
//
namespace IO_Helper {
template<typename Ty>
int read(Ty &var);  // Read a variable
//
#define SpecRead(type) template<> int Useful_Helpers::IO_Helper::read<type>(type &var)
template<typename IType>
int read_int(IType &var)
{
    int peek;
    while (isspace(peek = getchar()) && peek != '-')
        ;
    if (peek == EOF) return EOF;
    bool negi;
    if (peek == '-')
    {
        negi = true;
        peek = getchar();
    }
    else
        negi = false;
    var = 0;
    do
        var = var * 10 + peek - '0';
    while (isdigit(peek = getchar()));
    if (negi) var = -var;
    return 1;
}
SpecRead(i32t) { return read_int(var); }
SpecRead(u32t) { return read_int(var); }
SpecRead(i64t) { return read_int(var); }
SpecRead(u64t) { return read_int(var); }
SpecRead(char) { while (isspace(var = getchar()) && var != EOF); return var == EOF ? EOF : 1; }
SpecRead(f64t) { return scanf("%lf", &var); }
SpecRead(char*) { return scanf("%s", var); }
template<int Size> int read(char (&i)[Size]) { return scanf("%s", i); }
template<typename A, typename B> int read(std::pair<A, B> &p)
{ return read(p.first), read(p.second); }
#undef ImplRead
template<typename Ty>
void print(const Ty &var);
#define SpecPrint(type) template<> void Useful_Helpers::IO_Helper::print<type>(const type &var)
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
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
const int Inf = 0x3f3f3f3f;

void solve();

i32t main()
{
    int T;
    io.read(T);
    while (T--)
        solve();
    return 0;
}

int cup[3], water, target;
bool used[MaxN][MaxN];

typedef int waterTy;
typedef int ansTy;
typedef std::pair<waterTy, waterTy> handle;
std::priority_queue<std::pair<ansTy, handle>,
    std::vector<std::pair<ansTy, handle>>,
    std::greater<std::pair<ansTy, handle>>> bfsq;
#define mp(a, b) std::make_pair(a, b)
waterTy ansval;
ansTy ans;

void update(waterTy nowval, ansTy nowans)
{
    if ((nowval > ansval && nowval <= target) ||
            (nowval == ansval && nowans < ans))
    {
        ansval = nowval;
        ans = nowans;
    }
}

// first: current water
// second: max amount
std::pair<ansTy, handle> pour(ansTy pre, handle src, handle dst)
{
    waterTy rest = dst.second - dst.first;
    ansTy res;
    if (src.first < rest)
    {
        dst.first += src.first;
        res = pre + src.first;
        update(dst.first, res);
        src.first = 0;
    }
    else
    {
        dst.first += rest;
        res = pre + rest;
        update(dst.first, res);
        src.first -= rest;
        update(src.first, res);
    }
    return mp(res, mp(src.first, dst.first));
}

// first: id
// second: val
int getone(int id, handle src, handle dst)
{
    if (src.first == id)
        return src.second;
    else if (dst.first == id)
        return dst.second;
    else
        return water - src.second - dst.second;
}
handle getab(handle src, handle dst)
{
    return mp(getone(0, src, dst), getone(1, src, dst));
}

void solve()
{
    io.read(cup, 0, 2);
    io.read(target);
    water = cup[2];
    ansval = 0;

    memset(used, false, sizeof(used));
    bfsq.push(mp(0, mp(0, 0)));
    while (!bfsq.empty())
    {
        const auto now = bfsq.top();
        bfsq.pop();
        const int vals[] = {
            now.second.first,
            now.second.second,
            water - now.second.first - now.second.second,
        };
        if (used[vals[0]][vals[1]])
            continue;
        used[vals[0]][vals[1]] = true;
        Rep(i, 0, 2) Rep(j, 0, 2)
            if (i != j)
            {
                std::pair<waterTy, handle> res = pour(now.first, mp(vals[i], cup[i]), mp(vals[j], cup[j]));
                res.second = getab(mp(i, res.second.first), mp(j, res.second.second));
                bfsq.push(res);
            }
    }
    io.print("$ $\n", ans, ansval);
}

