#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) printf(__VA_ARGS__)
#endif

const int MaxN = int(1e5) + 7;
const int Inf = 0x3f3f3f3f;

int n, l;
struct Book
{
    int height, width;
};
Book lib[MaxN];
int sum[MaxN];

namespace ProgSln
{
int dp[MaxN];
class SlnTree
{
private:
    int weight[MaxN];
    int child[MaxN][2];
    int index[MaxN];
    int lmost[MaxN];
    int height[MaxN], heicov[MaxN];
    int minans[MaxN];

    int root, id;
    int alloc(int pos);

    void update(int now);
    void push(int now);

    template<int type>
    bool spprec(int now, int &arg);
    template<int type>
    void split(int now, int arg, int &left, int &right)
    {
        if (now == 0)
        {
            left = right = 0;
            return;
        }
        push(now);
        if (spprec<type>(now, arg))
        {
            right = now;
            split<type>(child[now][0], arg, left, child[now][0]);
        }
        else
        {
            left = now;
            split<type>(child[now][1], arg, child[now][1], right);
        }
        update(now);
    }

    int merge(int left, int right)
    {
        if (left == 0 || right == 0)
            return left + right;
        push(left); push(right);
        int res;
        if (weight[left] > weight[right])
        {
            res = left;
            child[res][1] = merge(child[res][1], right);
        }
        else
        {
            res = right;
            child[res][0] = merge(left, child[res][0]);
        }
        update(res);
        return res;
    }

public:
    SlnTree() { clear(); }
    void clear()
    {
        root = id = 0;
        (*child)[0] = (*child)[1] = 0;
        *index = -1;
        *height = 0;
        *minans = Inf;
    }

    void insert(int pos);
    int query(int pos);

    void ptree(int node = -1, bool stal = true)
    {
        struct Echoer
        {
            Echoer() { echo("```dot\ndigraph G {\n"); }
            ~Echoer() { echo("}\n```\n"); }
        };
        if (node == -1)
            node = root;
        else if (node == 0)
            return;
        Echoer *ec = stal ? new Echoer : nullptr;
        const int pos = index[node];
        echo(" N%d [label=\"%d: hei %d, ans %d\"]\n", pos, pos, height[node], minans[node]);
        auto pn = [this, node](int next, char st) { if (next) echo("  N%d -> N%d [label=\"%c\"]\n", index[node], index[next], st); };
        const int lch = child[node][0], rch = child[node][1];
        pn(lch, 'l'); pn(rch, 'r');
        ptree(lch, false);
        ptree(rch, false);
        delete ec;
    }
};
SlnTree ansq;
}  // End namespace ProgSln

class Test
{
private:
    int randint(int lo, int hi) { return rand() % (hi - lo + 1) + lo; }
public:
    void scan()
    {
        scanf("%d%d", &n, &l);
        lib[0].width = lib[0].height = 0;
        for (int i = 1; i <= n; ++i)
        {
            scanf("%d%d", &lib[i].height, &lib[i].width);
            sum[i] = sum[i - 1] + lib[i].width;
        }
    }
    void generate()
    {
        n = randint(7, 20);
        l = randint(11, 23);
        lib[0].width = lib[0].height = 0;
        for (int i = 1; i <= n; ++i)
        {
            lib[i].width = randint(1, std::min(l, 13));
            lib[i].height = randint(1, 12);
            sum[i] = sum[i - 1] + lib[i].width;
        }
    }
    void print()
    {
        fprintf(stderr, "%d %d\n", n, l);
        for (int i = 1; i <= n; ++i)
            fprintf(stderr, "%d %d\n", lib[i].height, lib[i].width);
    }

    int runstd()
    {
        static int dp[MaxN];
        dp[0] = 0;
        for (int now = 1; now <= n; ++now)
        {
            dp[now] = Inf;
            int maxhei = lib[now].height;
            for (int last = now - 1;
                    last >= 0 && sum[now] - sum[last] <= l;
                    maxhei = std::max(maxhei, lib[last].height), --last)
                dp[now] = std::min(dp[now], maxhei + dp[last]);
        }
        return dp[n];
    }
    int runprog()
    {
        using namespace ProgSln;
        ansq.clear();
        dp[0] = 0;
        ansq.insert(0);
        for (int now = 1; now <= n; ++now)
        {
            dp[now] = ansq.query(now);
            echo("query %d to %d\n", now, dp[now]);
            ansq.insert(now);
        }
        return dp[n];
    }
};

Test t;
int main()
{
    for (int data = 1; data; ++data)
    {
        echo("\rdata %d", data);
        t.generate();
        int stdans = t.runstd();
        int progans = t.runprog();
        if (stdans != progans)
        {
            echo("\nstd %d, prog %d\n", stdans, progans);
            break;
        }
    }
    t.print();
    return 0;
}

namespace ProgSln
{
int SlnTree::alloc(int pos)
{
    const int now = ++id;
    //weight[now] = rand();
    weight[now] = ((id * 1103515245) + 12345) & 0x7fffffff;
    index[now] = pos;
    lmost[now] = pos;
    child[now][0] = child[now][1] = 0;
    heicov[now] = height[now] = 0;
    minans[now] = dp[pos];
    return now;
}

#define newsp(type, expr) \
    const int type = __LINE__; \
    template<> bool SlnTree::spprec<type>(int now, int &arg) { return expr; }
newsp(ByDP, dp[index[now]] >= arg);
void SlnTree::insert(int pos)
{
    // Ensure dp inc
    int rest;
    split<ByDP>(root, dp[pos], root, rest);
    // Merge
    int ins = alloc(pos);
    //echo("insert(%d):", dp[pos]);
    //ptree(ins, false);
    //echo("insert merge:%d(%d) %d(%d)\n", index[root], root, index[ins], ins);
    root = merge(root, ins);
}

newsp(BySum, arg - sum[index[now]] <= l);
newsp(ByHei, height[now] < arg);
int SlnTree::query(int pos)
{
    // Retire outdated
    int rest;
    split<BySum>(root, sum[pos], rest, root);
    // Update height
    split<ByHei>(root, lib[pos].height, rest, root);
    if (root)
    {
        heicov[root] = height[root] = lib[pos].height;
        minans[root] = dp[lmost[root]] + height[root];
        echo("set %d(%d:%d) to hei %d, ans %d\n", root, pos, lib[pos].height, height[root], minans[root]);
    }
    root = merge(rest, root);
    echo("query on:\n");
    ptree();
    return minans[root];
}
#undef newsp

void SlnTree::update(int now)
{
    const int lch = child[now][0], rch = child[now][1];
    if (lch)
    {
        lmost[now] = lmost[lch];
        height[now] = std::max(std::max(lib[index[now]].height, lib[lmost[rch]].height),
                std::max(height[lch], height[rch]));
    }
    else
    {
        lmost[now] = index[now];
        height[now] = height[rch];
    }
    minans[now] = std::min(dp[lmost[now]] + height[now], std::min(minans[lch], minans[rch]));
}
void SlnTree::push(int now)
{
    auto touch = [this, now] (int next) {
        if (next && heicov[now] > height[next])
        {
            height[next] = heicov[next] = heicov[now];
            minans[next] = dp[lmost[next]] + height[next];
        }
    };
    touch(child[now][0]);
    touch(child[now][1]);
    heicov[now] = 0;
}
}  // End namespace ProgSln

