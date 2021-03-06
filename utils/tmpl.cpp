#include <cstdio>
#include <functional>
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
#define HELPER_COMBINE_IMPL(a, b) a##b
#define HELPER_COMBINE(a, b) HELPER_COMBINE_IMPL(a, b)
#define HELPER_TMPVARNAME(index) HELPER_COMBINE(HELPER_COMBINE(helper_tmpvar_, __LINE__), HELPER_COMBINE(_, index))
//
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
//
namespace RAIIOper
{
class RAIIOper
{
private:
    std::function<void(void)> defer;
    bool cond;
public:
    RAIIOper(std::function<void(void)> cdr, bool wh = true) : defer(cdr), cond(wh) {}
    ~RAIIOper() { if (cond) defer(); }
};
#define PostOper(oper) Useful_Helpers::RAIIOper::RAIIOper HELPER_TMPVARNAME(0)(oper)
#define PostOperIf(cond, oper) \
    Useful_Helpers::RAIIOper::RAIIOper HELPER_TMPVARNAME(0)(oper, cond)
}  // End namespace RAIIOper
} using namespace Useful_Helpers;

const int MaxN = int(1e5) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x3f3f3f3f;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

i32t main()
{
    return 0;
}

