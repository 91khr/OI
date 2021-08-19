#include <cstdio>
#include <functional>
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <algorithm>
#include <vector>
#include <ctime>
//#define BUFFERED_IO
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
SpecRead(char) { while (isspace(var = getchar()) && var != EOF) {} return 1; }
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
{ return (io.read(p.first) > 0) && (io.read(p.second) > 0) ? 1 : -1; }
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

const int MaxN = int(1e5) + 7;
const int MaxM = int(1e6) + 7;

int n, m;
int initval[MaxN];

class Scapegoat
{
private:
    struct Node
    {
        int value;
        int count, size, capacity;
        Node *child[2];
    };
    static inline Node pool[MaxM + MaxN];
    static inline int maxid;
    Node *root;

    Node *alloc(int value)
    {
        Node *res = pool + ++maxid;
        res->value = value;
        res->count = res->size = res->capacity = 1;
        res->child[0] = res->child[1] = pool;
        return res;
    }

    static constexpr double alpha = 0.7;
    bool ismalformed(Node *now)
    {
        return (now->child[0]->capacity > now->capacity * alpha + 5) ||
            (now->child[1]->capacity > now->capacity * alpha + 5);
    }

    static inline Node *nodebuf[MaxM + MaxN];
    static inline int bufsiz;
    static void flatten(Node *now)
    {
        auto left = now->child[0], right = now->child[1];
        if (left != pool)
            flatten(left);
        if (now->count)
        {
            nodebuf[++bufsiz] = now;
            now->size = now->count;
            now->capacity = 1;
            now->child[0] = now->child[1] = pool;
        }
        if (right != pool)
            flatten(right);
    }
    static Node *collect(int l, int r)
    {
        if (l == r)
            return nodebuf[l];
        int mid = (l + r) / 2;
        Node *res = nodebuf[mid];
        if (mid > l)
            res->child[0] = collect(l, mid - 1);
        res->child[1] = collect(mid + 1, r);
        res->size = res->child[0]->size + res->child[1]->size + res->count;
        res->capacity = res->child[0]->capacity + res->child[1]->capacity + 1;
        return res;
    }
    static void rebuild(Node **now)
    {
        echo("Rebuild %ld\n", *now - pool);
        bufsiz = 0;
        flatten(*now);
        *now = bufsiz >= 1 ? collect(1, bufsiz) : pool;
    }

public:
    Scapegoat() : root(pool)
    {
        maxid = 0; bufsiz = 0;
        pool->capacity = pool->size = 0;
        pool->count = 0;
        pool->child[0] = pool->child[1] = pool;
        pool->value = -1;
    }
    Scapegoat(const Scapegoat &) = delete;
    Scapegoat &operator=(const Scapegoat &) = delete;

    void build()
    {
        std::sort(initval, initval + n);
        bufsiz = 0;
        nodebuf[0] = pool;
        Rep(i, 0, n - 1)
            if (initval[i] != nodebuf[bufsiz]->value)
                nodebuf[++bufsiz] = alloc(initval[i]);
            else
            {
                auto now = nodebuf[bufsiz];
                ++now->count;
                ++now->size;
            }
        root = collect(1, bufsiz);
    }

    void insert(int value)
    {
        Node *ins = alloc(value);
        Node **pos = &root;
        Node **bad = nullptr;
        while (*pos != pool && (*pos)->value != value)
        {
            ++(*pos)->capacity;
            ++(*pos)->size;
            if (!bad && ismalformed(*pos))
                bad = pos;
            pos = value <= (*pos)->value ? &(*pos)->child[0] : &(*pos)->child[1];
        }
        if (*pos == pool)
            *pos = ins;
        else
        {
            ++(*pos)->count;
            ++(*pos)->size;
        }
        if (bad)
            rebuild(bad);
    }

    void remove(int value)
    {
        Node **pos = &root;
        Node **bad = nullptr;
        while ((*pos)->value != value)
        {
            --(*pos)->size;
            if (!bad && ismalformed(*pos))
                bad = pos;
            pos = value <= (*pos)->value ? &(*pos)->child[0] : &(*pos)->child[1];
        }
        --(*pos)->count;
        --(*pos)->size;
        if (bad)
            rebuild(bad);
    }

    int rank(int value)
    {
        int res = 0;
        Node *pos = root;
        while (pos != pool && pos->value != value)
        {
            int go = value > pos->value;
            if (go == 1)
                res += pos->child[0]->size + pos->count;
            pos = pos->child[go];
        }
        res += pos->child[0]->size;
        return res + 1;
    }

    int atrank(int rank)
    {
        int res;
        Node *pos = root;
        while (true)
            if (int left = pos->child[0]->size + pos->count; rank <= left)
            {
                if (rank > pos->child[0]->size)
                    return pos->value;
                else
                    pos = pos->child[0];
            }
            else
            {
                rank -= left;
                pos = pos->child[1];
            }
        return res;
    }

private:
    int findside(int value, int side)
    {
        bufsiz = 0;
        Node *pos = root;
        for (; pos != pool && pos->value != value;
                pos = value <= pos->value ? pos->child[0] : pos->child[1])
            nodebuf[++bufsiz] = pos;
        if (pos != pool)
            nodebuf[++bufsiz] = pos;
        for (Node **nowit = nodebuf + bufsiz; nowit != nodebuf; --nowit)
        {
            Node *now = *nowit;
            if (now->value != value && (now->value <= value) == side)
                continue;
            if (now->value != value && now->count)
                return now->value;
            if (now->child[side]->size == 0)
                continue;
            now = now->child[side];
            while (now != pool)
                if (now->child[!side]->size > 0)
                    now = now->child[!side];
                else if (now->count)
                    return now->value;
                else
                    now = now->child[side];
        }
        return -1;
    }
public:
    int prae(int value)
    {
        return findside(value, 0);
    }
    int post(int value)
    {
        return findside(value, 1);
    }

    void printTree()
    {
#ifdef DEBUG
        auto print = [] (auto print, Node *now) -> void {
            echo("%ld [label=\"%d(%d) %d %d\"]; ", now - pool, now->value, now->count,
                    now->size, now->capacity);
            if (now->child[0] != pool)
                echo("%ld -> %ld [label=\"l\"]; ",  now - pool, now->child[0] - pool);
            if (now->child[1] != pool)
                echo("%ld -> %ld [label=\"r\"]; ",  now - pool, now->child[1] - pool);
            echo("\n");
            if (now->child[0] != pool)
                print(print, now->child[0]);
            if (now->child[1] != pool)
                print(print, now->child[1]);
        };
        echo("digraph G {\n");
        print(print, root);
        echo("}\n");
#endif
    }
};
Scapegoat tree;

void (Scapegoat::*const modify[])(int) = { &Scapegoat::insert, &Scapegoat::remove };
int (Scapegoat::*const query[])(int) = { &Scapegoat::rank, &Scapegoat::atrank, &Scapegoat::prae, &Scapegoat::post };

int timecount[10];

int main()
{
    io.read(n, m);
    io.read(initval, 0, n - 1);
    tree.build();
    int last = 0, ans = 0;
    //tree.printTree();
    while (m--)
    {
        int op, val;
        io.read(op, val);
#ifdef DEBUG
        int beg = clock();
#else  // DEBUG not defined
        val ^= last;
#endif  // DEBUG
        echo("%d %d", op, val);
        if (op <= 2)
        {
            (tree.*modify[op - 1])(val);
            //tree.printTree();
        }
        else
        {
            last = (tree.*query[op - 3])(val);
            ans ^= last;
#ifdef DEBUG
            io.print("$\n", last);
#endif  // DEBUG
        }
        echo(" -> %d\n", last);
#ifdef DEBUG
        timecount[op] += clock() - beg;
#endif // DEBUG
    }
#ifdef DEBUG
    echo("tot ");
    int tottm = 0;
    Rep(i, 1, 6)
    {
        tottm += timecount[i];
        echo("%d ", timecount[i]);
    }
    echo(" -> %d\n", tottm);
#else  // DEBUG not defined
    io.print("$\n", ans);
#endif  // DEBUG
    return 0;
}

