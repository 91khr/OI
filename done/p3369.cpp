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
//
using i64t = std::int64_t;
using u64t = std::uint64_t;
using i32t = std::int32_t;
using u32t = std::uint32_t;
using f64t = double;
//
namespace IO_Helper {
template<typename Ty>
int read(Ty &var);  // Read a variable
#define SpecRead(type) template<> int IO_Helper::read<type>(type &var)
template<typename Ty>
void print(const Ty &var);
#define SpecPrint(type) template<> void IO_Helper::print<type>(const type &var)
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wextra-qualification"
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
SpecRead(char) { while (isspace(var = getchar()) && var != EOF) {} return var == EOF ? EOF : 1; }
SpecRead(f64t) { return scanf("%lf", &var); }
SpecRead(char*) { return scanf("%s", var); }
template<int Size> int read(char (&i)[Size]) { return scanf("%s", i); }
template<typename A, typename B> int read(std::pair<A, B> &p)
{ return read(p.first), read(p.second); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
template<> void print<const char*>(const char *const &str) { fputs(str, stdout); }
template<int Size> void print(const char (&str)[Size]) { fputs(str, stdout); }
#undef ImplPrint
#pragma GCC diagnostic pop
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

struct FHQTreap
{
public:
    struct Node
    {
        int value;
        int width, weight;
        Node *lchild, *rchild;
    };
    Node *pool, *root;
    int maxid;
    Node *alloc(int value)
    {
        Node *res = pool + ++maxid;
        res->value = value;
        res->width = 1;
        res->weight = rand();
        res->lchild = res->rchild = pool;
        return res;
    }

    void update(Node *now)
    {
        now->width = now->lchild->width + now->rchild->width + 1;
    }

    template<int ty> bool pred(Node *now, int &arg);
    template<int ty>
    void split(Node *now, int arg, Node *&left, Node *&right)
    {
        if (now == pool)
            return void(left = right = pool);
        if (pred<ty>(now, arg))
        {
            left = now;
            split<ty>(now->rchild, arg, now->rchild, right);
            update(left);
        }
        else
        {
            right = now;
            split<ty>(now->lchild, arg, left, now->lchild);
            update(right);
        }
    }

    Node *merge(Node *left, Node *right)
    {
        if (left == pool)
            return right;
        if (right == pool)
            return left;
        Node *res;
        if (left->weight > right->weight)
        {
            res = left;
            res->rchild = merge(res->rchild, right);
        }
        else
        {
            res = right;
            res->lchild = merge(left, res->lchild);
        }
        update(res);
        return res;
    }

public:
    FHQTreap() : pool(new Node[10000]), root(pool), maxid(0)
    {
        pool->width = 0;
        pool->lchild = pool->rchild = pool;
    }
    void refresh(std::vector<int> vals);

    void insert(int value);
    void remove(int value);
    int rank(int value);
    int totsize() { return root->width; }
    int atrank(int rank);
    int prae(int value);
    int post(int value);
} tree;

void FHQTreap::refresh(std::vector<int> vals)
{
    root = pool;
    maxid = 0;
    std::sort(vals.begin(), vals.end());
    static std::vector<Node *> leave;
    leave.reserve(vals.size());
    leave.push_back(root = alloc(vals.front()));
    for (auto it = vals.begin() + 1; it != vals.end(); ++it)
    {
        Node *ins = alloc(*it), *now = leave.back();
        for (; !leave.empty() && ins->weight > (now = leave.back())->weight; leave.pop_back())
            update(now);
        if (!leave.empty())
        {
            ins->lchild = now->rchild;
            now->rchild = ins;
        }
        else
        {
            ins->lchild = root;
            root = ins;
        }
        leave.push_back(ins);
    }
    for (auto now = leave.rbegin(); now != leave.rend(); ++now)
        update(*now);
}

#define NewSp(type) \
    inline constexpr int type = __LINE__; \
    template<> bool FHQTreap::pred<type>(Node *now, int &arg)

NewSp(ByVal) { return now->value < arg; }

void FHQTreap::insert(int value)
{
    Node *l, *r;
    split<ByVal>(root, value, l, r);
    root = merge(merge(l, alloc(value)), r);
}

NewSp(BySiz)
{
    if (int len = now->lchild->width + 1; len <= arg)
    {
        arg -= len;
        return true;
    }
    else
        return false;
}

void FHQTreap::remove(int value)
{
    Node *l, *r, *m;
    split<ByVal>(root, value, l, r);
    split<BySiz>(r, 1, m, r);
    root = merge(l, r);
}

int FHQTreap::rank(int value)
{
    Node *l, *r;
    split<ByVal>(root, value, l, r);
    int res = l->width + 1;
    root = merge(l, r);
    return res;
}

int FHQTreap::atrank(int rank)
{
    Node *l, *r, *m;
    split<BySiz>(root, rank - 1, l, r);
    split<BySiz>(r, 1, m, r);
    int res = m->value;
    root = merge(merge(l, m), r);
    return res;
}

int FHQTreap::prae(int value)
{
    Node *l, *r, *m;
    split<ByVal>(root, value, l, r);
    split<BySiz>(l, l->width - 1, l, m);
    int res = m->value;
    root = merge(merge(l, m), r);
    return res;
}

int FHQTreap::post(int value)
{
    Node *l, *r, *m;
    split<ByVal>(root, value + 1, l, r);
    split<BySiz>(r, 1, m, r);
    int res = m->value;
    root = merge(merge(l, m), r);
    return res;
}

#undef NewSp

void (FHQTreap::*const modify[])(int) = { &FHQTreap::insert, &FHQTreap::remove };
int (FHQTreap::*const query[])(int) = { &FHQTreap::rank, &FHQTreap::atrank, &FHQTreap::prae, &FHQTreap::post };

int n, m;
int initval[10000];

int main()
{
    io.read(n, m);
    io.read(initval, 0, n - 1);
    tree.refresh({initval, initval + n});
    int last = 0;
    while (m--)
    {
        int op, val;
        io.read(op, val);
        if (op <= 2)
            (tree.*modify[op - 1])(val);
        else
        {
            last = (tree.*query[op - 3])(val);
            io.print("$\n", last);
        }
    }
    return 0;
}

