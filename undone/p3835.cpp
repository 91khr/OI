#include <cstdio>
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
DstTy set(DstTy &dst, const SrcTy &src) { return dst = static_cast<DstTy>(src); }
//
typedef long long i64t;
typedef unsigned long long u64t;
typedef unsigned u32t;
typedef int i32t;
//
namespace IO_Helper {
template<typename Ty>
int read(Ty &var);  // Read a variable
//
#define SpecRead(type) template<> int Useful_Helpers::IO_Helper::read<type>(type &var)
#define ImplRead(type, fmt) \
    SpecRead(type) { return scanf(fmt, &var); }
ImplRead(int, "%" SCNd32);
ImplRead(char, " %c");
ImplRead(i64t, "%" SCNdFAST64);
ImplRead(u64t, "%" SCNuFAST64);
SpecRead(char*) { return scanf("%s", var); }
template<int Size> int read(char (&i)[Size]) { return scanf("%s", i); }
#undef ImplRead
template<typename Ty>
void print(const Ty &var);
#define SpecPrint(type) template<> void Useful_Helpers::IO_Helper::print<type>(const type &var)
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRIdFAST64);
ImplPrint(u64t, "%" PRIuFAST64);
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

const int MaxN = int(5e5) + 7;
const i64t Mod = int(1e9) + 7;
const int Inf = 0x7fffffff;
const int MinInf = -Inf;
const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

class FHQTreap
{
private:
    struct TreeNode
    {
        int weight, width;
        TreeNode *lchild, *rchild;
        int value;
    };
    TreeNode node[2 * MaxN];
    int id, maxver;
    int root[MaxN];

    TreeNode *alloc(int val)
    {
        TreeNode *res = node + ++id;
        res->weight = rand();
        res->width = 1;
        res->lchild = res->rchild = nullptr;
        res->value = val;
        return res;
    }
    TreeNode *clone(TreeNode *o)
    {
        TreeNode *res = node + ++id;
        res->weight = o->weight;
        res->width = o->width;
        res->lchild = o->lchild;
        res->rchild = o->rchild;
        res->value = o->value;
        return res;
    }

    void update(TreeNode *now)
    {
        TreeNode *lch = now->lchild ? now->lchild : node, rch = now->rchild ? now->rchild : node;
        now->width = lch->width + rch->width + 1;
    }

    template<int type> bool spprec(TreeNode *now, int &arg);
    template<int type>
    void split(TreeNode *now, int arg, TreeNode *&left, TreeNode *&right)
    {
        if (!now)
        {
            left = right = nullptr;
            return;
        }
        now = clone(now);
        if (spprec(now, arg))
        {
            left = now;
            split(now->rchild, arg, now->rchild, right);
        }
        else
        {
            right = now;
            split(now->lchild, arg, left, now->lchild);
        }
        update(now);
    }

    TreeNode *merge(TreeNode *left, TreeNode *right)
    {
        if (left == nullptr)
            return right;
        else if (right == nullptr)
            return left;
        TreeNode *res;
        if (left->weight > right->weight)
        {
            left = clone(left);
            res = left;
            res->rchild = merge(res->rchild, right);
        }
        else
        {
            right = clone(right);
            res = right;
            res->lchild = merge(left, res->lchild);
        }
        update(res);
        return res;
    }

public:
    FHQTreap() : id(0)
    {
        node->width = 0;
    }
};

i32t main()
{
    return 0;
}

