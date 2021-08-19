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
//
#define Rep(var, init, delim) for (int var = (init); var <= (delim); ++var)
//
using i64t = std::int64_t;
using u64t = std::uint64_t;
using i32t = std::int32_t;
using u32t = std::uint32_t;
using f64t = double;
//
namespace IO_Helper {
template<class T> inline int read(T &var) { return var.read(); }
template<class T> inline void print(const T &var) { var.print(); }
// Special cases ><
template<int N> int read(char (&var)[N]);  // Forwards to read<char*>
template<> void print<const char *>(const char *const &var) { fputs(var, stdout); }
//
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
    template<class T> friend int read(T &);
    template<class Ty, int Size>
    int read(Ty (&arr)[Size], int beg, int end)
    { Rep(i, beg, end) if (IO_Helper::read(arr[i]) < 0) return -1; return 1; }
    template<class First, class ...Rest>
    int read(First &car, Rest& ...cdr)
    { return IO_Helper::read(car) + read(cdr...); }
    template<class T> friend void print(const T &);
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
            IO_Helper::print(car);
            if constexpr (sizeof...(cdr))
                this->print(fwd + 1, cdr...);
            else
                IO_Helper::print(fwd + 1);
        }
    }
}io_impl;
//
#define SpecRead(type) template<> inline int read<type>(type &var)
#define READINT_COMMON_BEGIN \
    int peek; \
    while (isspace(peek = io_impl.getch()) && peek != '-') \
        ; \
    if (peek == EOF) return EOF
#define READINT_COMMON_END \
    var = 0; \
    do \
        var = var * 10 + peek - '0'; \
    while (isdigit(peek = io_impl.getch()))
SpecRead(i64t)
{
    READINT_COMMON_BEGIN;
    bool negi = peek == '-' ? (peek = io_impl.getch()) && true : false;
    READINT_COMMON_END;
    if (negi) var = -var;
    return 1;
}
SpecRead(u64t)
{
    READINT_COMMON_BEGIN;
    READINT_COMMON_END;
    return 1;
}
#undef READINT_COMMON_BEGIN
#undef READINT_COMMON_END
SpecRead(i32t) { i64t v; if (read<i64t>(v) > 0) return var = v, 1; else return -1; }
SpecRead(u32t) { u64t v; if (read<u64t>(v) > 0) return var = v, 1; else return -1; }
SpecRead(char) { while (isspace(var = io_impl.getch()) && var != EOF) {} return 1; }
SpecRead(char*)
{
    read(var[0]);
    char *it;
    for (it = var + 1; !isspace(*it = io_impl.getch()); ++it)
        ;
    *it = '\0';
    return 1;
}
template<int N> int read(char (&var)[N]) { char *ptr = var; return read(ptr); }
#if !defined(BUFFERED_IO)
SpecRead(f64t) { return scanf("%lf", &var); }
#endif  // !defined(BUFFERED_IO)
template<typename A, typename B> inline int read(std::pair<A, B> &p)
{ return io_impl.read(p.first) < 0 ? -1 : io_impl.read(p.second); }
#undef SpecRead
#define ImplPrint(type, fmt) template<> inline void print<type>(const type &var) { printf(fmt, var); }
ImplPrint(int, "%" PRId32);
ImplPrint(char, "%c");
ImplPrint(i64t, "%" PRId64);
ImplPrint(u64t, "%" PRIu64);
#undef ImplPrint
}  // End namespace IO_Helper
[[maybe_unused]] auto &io = IO_Helper::io_impl;
} using namespace Useful_Helpers;

[[maybe_unused]] const int MaxN = int(2e5) + 7;
[[maybe_unused]] const i64t Mod = int(1e9) + 7;
[[maybe_unused]] const int Inf = 0x3f3f3f3f;
[[maybe_unused]] const i64t Inf64 = 0x3f3f3f3f3f3f3f3f;

int n, m;
int orig[MaxN];
std::pair<int, int> sparsed[MaxN];

class SegTree
{
public:
    struct Node
    {
        int width;
        Node *lchild, *rchild;
    };
    Node pool[MaxN * 25];
    int maxid = 0, maxver = 0;
    Node *root[MaxN];

    Node *clone(Node *from) { return &(pool[++maxid] = *from); }

public:
    SegTree()
    {
        pool->width = 0;
        pool->lchild = pool->rchild = pool;
        root[0] = pool;
    }

    Node *build(int lb, int rb)
    {
        Node *now = pool + ++maxid;
        now->width = 0;
        if (lb != rb)
        {
            int mid = (lb + rb) / 2;
            now->lchild = build(lb, mid);
            now->rchild = build(mid + 1, rb);
        }
        else
            now->lchild = now->rchild = pool;
        return now;
    }

    void insert(Node *&now, int lb, int rb, int value)
    {
        now = clone(now);
        ++now->width;
        if (lb == rb)
            return;
        int mid = (lb + rb) / 2;
        if (value <= mid)
            insert(now->lchild, lb, mid, value);
        else
            insert(now->rchild, mid + 1, rb, value);
    }

    int query(Node *l, Node *r, int lb, int rb, int rank)
    {
        if (lb == rb)
            return lb;
        int mid = (lb + rb) / 2;
        int left = r->lchild->width - l->lchild->width;
        if (left < rank)
            return query(l->rchild, r->rchild, mid + 1, rb, rank - left);
        else
            return query(l->lchild, r->lchild, lb, mid, rank);
    }

    void printTree()
    {
        auto print = [this] (auto print, Node *now, int base) -> void {
            echo("%d.%ld [label=\"%ld w:%d\"]; ", base, now - pool, now - pool, now->width);
            if (now->lchild != pool)
                echo("%d.%ld -> %d.%ld; ", base, now - pool, base, now->lchild - pool);
            if (now->rchild != pool)
                echo("%d.%ld -> %d.%ld; ", base, now - pool, base, now->rchild - pool);
            echo("\n");
            if (now->lchild != pool)
                print(print, now->lchild, base);
            if (now->rchild != pool)
                print(print, now->rchild, base);
        };
        echo("digraph G {\n");
        Rep(i, 0, n)
            print(print, root[i], i);
        echo("}\n");
    }
};
SegTree tree;

int main()
{
    io.read(n, m);
    io.read(orig, 1, n);
    Rep(i, 1, n)
        sparsed[i] = std::make_pair(orig[i], i);
    std::sort(sparsed + 1, sparsed + n + 1);
    Rep(i, 1, n)
        orig[sparsed[i].second] = i;

    tree.root[0] = tree.build(1, n);
    Rep(i, 1, n)
        tree.insert(tree.root[i] = tree.root[i - 1], 1, n, orig[i]);
    //tree.printTree();

    Rep(i, 1, m)
    {
        int l, r, k;
        io.read(l, r, k);
        io.print("$\n", sparsed[tree.query(tree.root[l - 1], tree.root[r], 1, n, k)].first);
    }

    return 0;
}

