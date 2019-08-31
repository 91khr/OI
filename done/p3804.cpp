#include <cstdio>
#include <vector>
#include <algorithm>

typedef unsigned long long laolin;

const int MaxN = static_cast<int>(1e6 + 7);

laolin endset[MaxN * 2];
laolin ans;

namespace ParTree
{
int dst[MaxN * 2];
int next[MaxN * 2];
int via[MaxN * 2];
int size = 0;
void addedge(int par, int ch)
{
    dst[++size] = ch;
    next[size] = via[par];
    via[par] = size;
}
}

namespace SAM
{
int child[MaxN * 2][29];
int parent[MaxN * 2];
int len[MaxN  * 2];
int last = 1, size = 1;
void insert(int ch)
{
    int upper = last, now = (last = ++size);
    endset[now] = 1;
    len[now] = len[upper] + 1;
    for (; upper && child[upper][ch] == 0; upper = parent[upper])
        child[upper][ch] = now;
    if (upper == 0)
        parent[now] = 1;
    else
    {
        int base = child[upper][ch];
        if (len[upper] + 1 == len[base])
            parent[now] = base;
        else
        {
            int ins = ++size;
            // Copy
            for (int i = 0; i < 29; ++i)
                child[ins][i] = child[base][i];
            parent[ins] = parent[base];
            endset[ins] = 0;
            len[ins] = len[upper] + 1;
            parent[now] = parent[base] = ins;
            // Modify all parent
            for (; upper && child[upper][ch] == base; upper = parent[upper])
                child[upper][ch] = ins;
        }
    }
}
}

laolin dfs(int now)
{
    for (int i = ParTree::via[now]; i; i = ParTree::next[i])
        endset[now] += dfs(ParTree::dst[i]);
    if (endset[now] > 1)
        ans = std::max(ans, endset[now] * SAM::len[now]);
    return endset[now];
}

int main(void)
{
    // SAM
    {
        int peek;
        while (!feof(stdin) && (peek = getchar()) != '\n')
            SAM::insert(peek - 'a');
    }

    // Parent tree
    for (int i = 1; i <= SAM::size; ++i)
        if (SAM::parent[i] != 0)
            ParTree::addedge(SAM::parent[i], i);

    for (int i = 1; i <= SAM::size; ++i)
    {
        printf("%d: ", i);
        for (int j = ParTree::via[i]; j; j = ParTree::next[j])
            printf("%d ", ParTree::dst[j]);
        printf("\n");
    }

    // DFS for ans
    ans = 0;
    dfs(1);
    printf("%lld\n", ans);
    return 0;
}

