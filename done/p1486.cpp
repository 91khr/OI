#include <cstdio>
#include <functional>
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <algorithm>
#define BUFFERED_IO
namespace Useful_Helpers {
void Unused(...) {}
#if defined(DEBUG) || defined(DEBUG_ECHO)
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#else
  #define echo(...) Unused(__VA_ARGS__)
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
struct IO {
private:
#ifdef BUFFERED_IO
    char buf[BUFSIZ], *endbuf = buf, *it = buf;
    int getch()
    {
        if (++it >= endbuf)
            return (endbuf = buf + fread(buf, 1, BUFSIZ, stdin)) == buf ? EOF : *(it = buf);
        return *it;
    }
#else  // !defined(BUFFERED_IO)
    int getch() { return getchar(); }
#endif  // defined(BUFFERED_IO)
    int read() { return 0; }
public:
    template<class T>
    int read(T &val);
    template<class Ty, int Size>
    int read(Ty (&arr)[Size], int beg, int end)
    { Rep(i, beg, end) if (read(arr[i]) < 0) return -1; return 1; }
    template<class First, class ...Rest>
    int read(First &car, Rest& ...cdr)
    { return read(car) + read(cdr...); }
    template<class T>
    void print(const T &val);
    void print(const char *fmt) { fputs(fmt, stdout); }
    template<class First, class ...Rest>
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
            print(car);
            print(fwd + 1, cdr...);
        }
    }
}io;
//
#define SpecRead(type) template<> int IO_Helper::IO::read<type>(type &var)
#define SpecPrint(type) template<> void IO_Helper::IO::print<type>(const type &var)
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wextra-qualification"
SpecRead(i64t)
{
    int peek;
    while (isspace(peek = getch()) && peek != '-')
        ;
    if (peek == EOF) return EOF;
    bool negi;
    if (peek == '-')
    {
        negi = true;
        peek = getch();
    }
    else
        negi = false;
    var = 0;
    do
        var = var * 10 + peek - '0';
    while (isdigit(peek = getch()));
    if (negi) var = -var;
    return 1;
}
SpecRead(u64t)
{
    int peek;
    while (isspace(peek = getch()) && peek != '-')
        ;
    if (peek == EOF) return EOF;
    var = 0;
    do
        var = var * 10 + peek - '0';
    while (isdigit(peek = getch()));
    return 1;
}
SpecRead(i32t) { i64t v; if (read<i64t>(v) > 0) return var = v, 1; else return -1; }
SpecRead(u32t) { u64t v; if (read<u64t>(v) > 0) return var = v, 1; else return -1; }
SpecRead(char) { while (isspace(var = getch()) && var != EOF) {} return 1; }
SpecRead(char*)
{
    read(var[0]);
    char *it;
    for (it = var + 1; !isspace(*it = getch()); ++it)
        ;
    *it = '\0';
    return 1;
}
#if !defined(BUFFERED_IO)
SpecRead(f64t) { return scanf("%lf", &var); }
#endif  // !defined(BUFFERED_IO)
template<typename A, typename B> int read(std::pair<A, B> &p)
{ io.read(p.first); io.read(p.second); }
#define ImplPrint(type, fmt) \
    SpecPrint(type) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
#undef ImplPrint
#pragma GCC diagnostic pop
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

[[maybe_unused]] const int MaxN = int(1e5) + 7;
[[maybe_unused]] const i64t Mod = int(1e9) + 7;
[[maybe_unused]] const int Inf = 0x3f3f3f3f;
[[maybe_unused]] const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, min;

class FHQTreap
{
private:
    struct Node
    {
        int value, addtag;
        int size, weight;
        Node *lchild, *rchild;
    };
    Node pool[MaxN];
    Node *root;
    int maxid;

    void update(Node *now)
    {
        now->size = now->lchild->size + now->rchild->size + 1;
    }
    void push(Node *now)
    {
        if (now->addtag)
        {
            now->lchild->value += now->addtag;
            now->rchild->value += now->addtag;
            now->lchild->addtag += now->addtag;
            now->rchild->addtag += now->addtag;
            now->addtag = 0;
        }
    }

    template<int type> bool pred(Node *now, int &arg);
    template<int type>
    void split(Node *now, int arg, Node *&left, Node *&right)
    {
        if (now == pool)
            return void(left = right = pool);
        push(now);
        if (pred<type>(now, arg))
        {
            left = now;
            split<type>(now->rchild, arg, now->rchild, right);
            update(left);
        }
        else
        {
            right = now;
            split<type>(now->lchild, arg, left, now->lchild);
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
        push(left);
        push(right);
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
    FHQTreap() : root(pool), maxid(0)
    {
        pool->size = 0;
        pool->lchild = pool->rchild = pool;
    }

    void insert(int value);
    void addall(int value);
    int suball(int value);
    int query(int rank);

    void printTree()
    {
#ifdef DEBUG
        auto print = [this] (auto f, Node *now) -> void {
            echo("%ld [label=\"v:%d t:%d w:%d\"]; ", now - pool, now->value, now->addtag, now->size);
            if (now->lchild != pool)
                echo("%ld -> %ld; ", now - pool, now->lchild - pool);
            if (now->rchild != pool)
                echo("%ld -> %ld; ", now - pool, now->rchild - pool);
            echo("\n");
            if (now->lchild != pool)
                f(f, now->lchild);
            if (now->rchild != pool)
                f(f, now->rchild);
        };
        echo("digraph G {\n");
        print(print, root);
        echo("}\n");
#endif
    }
};

#define NewSp(T) \
    constexpr int T = __LINE__; \
    template<> bool FHQTreap::pred<T>(Node *now, int &arg)

NewSp(ByVal) { return now->value < arg; }
NewSp(BySiz)
{
    if (int left = now->lchild->size + 1; left <= arg)
    {
        arg -= left;
        return true;
    }
    else
        return false;
}

#undef NewSp

void FHQTreap::insert(int value)
{
    Node *ins = pool + ++maxid;
    ins->value = value;
    ins->addtag = 0;
    ins->size = 1;
    ins->weight = rand();
    ins->lchild = ins->rchild = pool;
    Node *l, *r;
    split<ByVal>(root, value, l, r);
    root = merge(merge(l, ins), r);
}

void FHQTreap::addall(int value)
{
    root->addtag += value;
    root->value += value;
}

int FHQTreap::suball(int value)
{
    root->addtag -= value;
    root->value -= value;
    Node *l, *r;
    split<ByVal>(root, min, l, r);
    int res = l->size;
    root = r;
    return res;
}

int FHQTreap::query(int rank)
{
    if (rank > root->size)
        return -1;
    Node *l, *r;
    split<BySiz>(root, root->size - rank, l, r);
    int res;
    for (Node *pos = r; pos != pool; pos = pos->lchild)
    {
        push(pos);
        res = pos->value;
    }
    root = merge(l, r);
    return res;
}

FHQTreap tree;

int main()
{
    io.read(n, min);
    int ans = 0;
    while (n--)
    {
        char op;
        int arg;
        io.read(op, arg);
        switch (op)
        {
        case 'I':
            if (arg < min)
                break;
            tree.insert(arg);
            break;
        case 'A':
            tree.addall(arg);
            break;
        case 'S':
            ans += tree.suball(arg);
            break;
        case 'F':
            io.print("$\n", tree.query(arg));
            break;
        }
    }
    io.print("$\n", ans);
    return 0;
}

