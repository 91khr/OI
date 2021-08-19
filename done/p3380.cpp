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
{ return io.read(p.first) < 0 ? -1 : io.read(p.second); }
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

[[maybe_unused]] const int MaxN = int(5e4) + 7;
[[maybe_unused]] const i64t Mod = int(1e9) + 7;
[[maybe_unused]] const int Inf = 0x7fffffff;
[[maybe_unused]] const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, m;
int initvalue[MaxN], sparsed[MaxN * 2];
std::pair<int, int> sorted[MaxN * 2];
struct {
    int code, l, r, val;
} oplist[MaxN];

class SubTree
{
private:
    struct Node
    {
        int weight;
        int size, value;
        Node *lchild, *rchild;
    };
    static inline Node pool[MaxN * 16];
    static inline int maxid = 0;
    Node *root;

    void update(Node *now)
    {
        now->size = now->lchild->size + now->rchild->size + 1;
    }

    template<int Ty> bool pred(Node *now, int &arg);
    template<int Ty>
    void split(Node *now, int arg, Node *&left, Node *&right)
    {
        if (now == pool)
            return void(left = right = pool);
        if (pred<Ty>(now, arg))
        {
            left = now;
            split<Ty>(now->rchild, arg, now->rchild, right);
        }
        else
        {
            right = now;
            split<Ty>(now->lchild, arg, left, now->lchild);
        }
        update(now);
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
    static void init()
    {
        pool->size = 0;
        pool->lchild = pool->rchild = pool;
    }

    void build(int lb, int rb);
    std::pair<int, int> rank(int value);
    void set(int from, int to);
    int prae(int value);
    int post(int value);

    void printTree();
};

void SubTree::printTree()
{
    auto print = [] (auto print, Node *now) -> void {
        echo("%ld [label=\"%d size %d\"]; ", now - pool, now->value, now->size);
        if (now->lchild != pool)
            echo("%ld -> %ld [label=\"l\"]; ", now - pool, now->lchild - pool);
        if (now->rchild != pool)
            echo("%ld -> %ld [label=\"r\"]; ", now - pool, now->rchild - pool);
        echo("\n");
        if (now->lchild != pool)
            print(print, now->lchild);
        if (now->rchild != pool)
            print(print, now->rchild);
    };
    static int id = 0;
    echo("digraph %d {\n", id++);
    print(print, root);
    echo("}\n");
}

void SubTree::build(int lb, int rb)
{
    static std::vector<Node *> leave;
    leave.reserve(n);
    leave.clear();
    auto alloc = [] (int value) {
        Node *res = pool + ++maxid;
        res->value = value;
        res->weight = rand();
        res->size = 1;
        res->lchild = res->rchild = pool;
        return res;
    };
    leave.push_back(root = alloc(initvalue[lb]));
    for (int *it = initvalue + lb + 1; it != initvalue + rb + 1; ++it)
    {
        Node *ins = alloc(*it);
        Node *pos = leave.back();
        for (; !leave.empty() && (pos = leave.back())->weight < ins->weight; leave.pop_back())
            update(pos);
        if (leave.empty())
        {
            root = ins;
            ins->lchild = pos;
        }
        else
        {
            ins->lchild = pos->rchild;
            pos->rchild = ins;
        }
        leave.push_back(ins);
    }
    for (auto it = leave.rbegin(); it != leave.rend(); ++it)
        update(*it);
}

#define NewSp(type) \
    constexpr int type = __LINE__; \
    template<> bool SubTree::pred<type>(Node *now, int &arg)

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

std::pair<int, int> SubTree::rank(int value)
{
    Node *l, *m, *r;
    split<ByVal>(root, value, l, r);
    split<ByVal>(r, value + 1, m, r);
    int front = l->size, count = m->size;
    root = merge(merge(l, m), r);
    return std::make_pair(front, count);
}

void SubTree::set(int from, int to)
{
    Node *l, *m, *it, *r;
    split<ByVal>(root, from, l, r);
    split<BySiz>(r, 1, it, r);
    it->value = to;
    if (to > from)
    {
        split<ByVal>(r, to, m, r);
        root = merge(merge(l, m), merge(it, r));
    }
    else
    {
        split<ByVal>(l, to, l, m);
        root = merge(merge(l, it), merge(m, r));
    }
}

int SubTree::prae(int value)
{
    Node *l, *r;
    split<ByVal>(root, value, l, r);
    int res = -Inf;
    for (Node *pos = l; pos != pool; pos = pos->rchild)
        res = pos->value;
    root = merge(l, r);
    return res;
}

int SubTree::post(int value)
{
    Node *l, *r;
    split<ByVal>(root, value + 1, l, r);
    int res = Inf;
    for (Node *pos = r; pos != pool; pos = pos->lchild)
        res = pos->value;
    root = merge(l, r);
    return res;
}

class ParTree
{
private:
     union Node
     {
         SubTree segment;
         int vertex;
     };
     Node pool[MaxN * 4];

     int lchild(int now) { return now * 2; }
     int rchild(int now) { return now * 2 + 1; }

     std::pair<int, int> rank_impl(int now, int lb, int rb, int value, int ql, int qr);
public:
     void build(int now, int lb, int rb);
     int rank(int now, int lb, int rb, int value, int ql, int qr)
     { return rank_impl(now, lb, rb, value, ql, qr).first + 1; }
     int atrank(int now, int lb, int rb, int rank, int ql, int qr);
     int set(int now, int lb, int rb, int pos, int value);
     int prae(int now, int lb, int rb, int value, int ql, int qr);
     int post(int now, int lb, int rb, int value, int ql, int qr);
};

void ParTree::build(int now, int lb, int rb)
{
    if (lb == rb)
        return void(pool[now].vertex = initvalue[lb]);
    int mid = (lb + rb) / 2;
    // Build child
    build(lchild(now), lb, mid);
    build(rchild(now), mid + 1, rb);
    // Merge the intervals
    static int mergebuf[MaxN];
    int *const midl = initvalue + mid + 1, *const rl = initvalue + rb + 1;
    int *bufp = mergebuf;
    for (int *lp = initvalue + lb, *rp = midl; lp != midl; )
    {
        while (rp != rl && *rp <= *lp)
            *bufp++ = *rp++;
        *bufp++ = *lp++;
    }
    for (int *src = mergebuf, *dst = initvalue + lb; src != bufp; ++src, ++dst)
        *dst = *src;
    // Build subtree
    pool[now].segment.build(lb, rb);
}

std::pair<int, int> ParTree::rank_impl(int now, int lb, int rb, int value, int ql, int qr)
{
    if (lb == rb)
        return std::make_pair(pool[now].vertex < value, pool[now].vertex == value);
    if (ql <= lb && rb <= qr)
        return pool[now].segment.rank(value);
    int mid = (lb + rb) / 2;
    std::pair<int, int> res{0, 0};
    auto add = [] (auto &a, const auto &b) { a.first += b.first; a.second += b.second; };
    if (ql <= mid)
        add(res, rank_impl(lchild(now), lb, mid, value, ql, qr));
    if (mid < qr)
        add(res, rank_impl(rchild(now), mid + 1, rb, value, ql, qr));
    return res;
}

// To ensure same prototype as others
int ParTree::atrank(int, int, int, int pos, int ql, int qr)
{
    int lb = post(1, 1, n, -Inf, ql, qr), rb = prae(1, 1, n, Inf, ql, qr);
    int val = (lb + rb) / 2;
    while (true)
    {
        auto [currank, rankspan] = rank_impl(1, 1, n, val, ql, qr);
        currank += 1;
        if (currank <= pos && pos < currank + rankspan)
            return val;
        else if (currank <= pos)
            lb = val + 1;
        else
            rb = val;
        val = (lb + rb) / 2;
    }
}

int ParTree::set(int now, int lb, int rb, int pos, int value)
{
    if (lb == rb)
    {
        int res = pool[now].vertex;
        pool[now].vertex = value;
        return res;
    }
    int mid = (lb + rb) / 2;
    int orig;
    if (pos <= mid)
        orig = set(lchild(now), lb, mid, pos, value);
    else
        orig = set(rchild(now), mid + 1, rb, pos, value);
    pool[now].segment.set(orig, value);
    return orig;
}

int ParTree::prae(int now, int lb, int rb, int value, int ql, int qr)
{
    if (lb == rb)
        return ([&] (int res) { return res < value ? res : -Inf; })(pool[now].vertex);
    if (ql <= lb && rb <= qr)
        return pool[now].segment.prae(value);
    int mid = (lb + rb) / 2;
    int res = -Inf;
    if (ql <= mid)
        res = std::max(res, prae(lchild(now), lb, mid, value, ql, qr));
    if (mid < qr)
        res = std::max(res, prae(rchild(now), mid + 1, rb, value, ql, qr));
    return res;
}

int ParTree::post(int now, int lb, int rb, int value, int ql, int qr)
{
    if (lb == rb)
        return ([&] (int res) { return res > value ? res : Inf; })(pool[now].vertex);
    if (ql <= lb && rb <= qr)
        return pool[now].segment.post(value);
    int mid = (lb + rb) / 2;
    int res = Inf;
    if (ql <= mid)
        res = std::min(res, post(lchild(now), lb, mid, value, ql, qr));
    if (mid < qr)
        res = std::min(res, post(rchild(now), mid + 1, rb, value, ql, qr));
    return res;
}

ParTree tree;
int optime[6];

int main()
{
    srand(74563109);
    io.read(n, m);
    io.read(initvalue, 1, n);
    Rep(i, 1, n)
        sorted[i] = std::make_pair(initvalue[i], i);
    int spdelim = n;
    Rep(i, 1, m)
    {
        auto &op = oplist[i];
        io.read(op.code, op.l);
        if (op.code == 3)
            io.read(op.val);
        else
            io.read(op.r, op.val);
        if (op.code != 2)
            sorted[++spdelim] = std::make_pair(op.val, i + n);
    }
    std::sort(sorted + 1, sorted + spdelim + 1);
    sparsed[0] = -1;
    int lastelem = 0;
    Rep(i, 1, spdelim)
    {
        if (sorted[i].first != sparsed[lastelem])
            sparsed[++lastelem] = sorted[i].first;
        if (int index = sorted[i].second; index <= n)
            initvalue[index] = lastelem;
        else
            oplist[index - n].val = lastelem;
    }

    SubTree::init();
    tree.build(1, 1, n);
    int (ParTree::*query[])(int now, int lb, int rb, int arg, int ql, int qr) = {
        &ParTree::rank, &ParTree::atrank, nullptr, &ParTree::prae, &ParTree::post,
    };
    Rep(i, 1, m)
    {
#ifdef DEBUG
        int start = clock();
#endif  // DEBUG
        const auto &op = oplist[i];
        if (op.code == 3)
            tree.set(1, 1, n, op.l, op.val);
        else
        {
            int res = (tree.*query[op.code - 1])(1, 1, n, op.val, op.l, op.r);
            io.print("$\n", op.code == 1 || res == Inf || res == -Inf ? res : sparsed[res]);
        }
#ifdef DEBUG
        optime[op.code] += clock() - start;
#endif  // DEBUG
    }

#ifdef DEBUG
    Rep(i, 1, 5)
        echo("%d ", optime[i]);
    echo("\n");
    echo("%ld\n", CLOCKS_PER_SEC);
#endif  // DEBUG
    return 0;
}

