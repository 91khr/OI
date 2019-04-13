#include <cstdio>

const int MaxN = 5e5 + 7;
const int Sigma = 29;

char origin[MaxN];
int t, k;
int endset[MaxN * 2];
int substr[MaxN * 2];
int ans;

namespace SAM
{
int parent[MaxN * 2], child[MaxN * 2][Sigma];
int len[MaxN * 2];
int last = 1, max = 1;
void insert(int ch)
{
    int upper = last, now = (last = ++max);
    endset[now] = 1;
    len[now] = len[upper] + 1;
    for (; upper && child[upper][ch] == 0; upper = parent[upper])
        child[upper][ch] = now;
    if (!upper)
        parent[now] = 1;
    else
    {
        int base = child[upper][ch];
        if (len[upper] + 1 == len[base])
            parent[now] = base;
        else
        {
            int ins = ++max;
            parent[ins] = parent[base];
            for (int i = 0; i < Sigma; ++i)
                child[ins][i] = child[base][i];
            endset[ins] = 0;
            len[ins] = len[upper] + 1;
            parent[now] = parent[base] = ins;
            for (; upper && child[upper][ch] == base; upper = parent[upper])
                child[upper][ch] = ins;
        }
    }
}
}

namespace ParTree
{
int dst[MaxN * 2], next[MaxN * 2];
int via[MaxN * 2];
int max = 0;
void addedge(int par, int ch)
{
    dst[++max] = ch;
    next[max] = via[par];
    via[par] = max;
}
}

void calen(int now)
{
    if (substr[now])
        return;
    //printf("calc %d\n", now);
    // Calc endset for appear times
    if (t)
        for (int edge = ParTree::via[now]; edge; edge = ParTree::next[edge])
        {
            int next = ParTree::dst[edge];
            calen(next);
            endset[now] += endset[next];
        }
    else
        endset[now] = 1;
    // Calc substr
    substr[now] = t ? endset[now] : 1;
    for (int ch = 0; ch < Sigma; ++ch)
    {
        int next = SAM::child[now][ch];
        if (next)
        {
            calen(next);
            substr[now] += substr[next];
            //printf("refresh %d by %d at %d\n", now, substr[next], next);
        }
    }
}

void printTree()
{
    printf("```dot\ndigraph G {\n");
    for (int i = 1; i <= SAM::max; ++i)
    {
        printf(" N%d [label=\"%d %d\"]\n", i, endset[i], substr[i]);
        if (SAM::parent[i])
            printf("  N%d -> N%d [style=\"dotted\"]\n", SAM::parent[i], i);
        for (int ch = 0; ch < Sigma; ++ch)
            if (SAM::child[i][ch])
                printf("  N%d -> N%d [label=\"%c\"]\n", i, SAM::child[i][ch], ch + 'a');
    }
    printf("}\n```\n");
}

int main(void)
{
    scanf("%s\n%d%d", origin, &t, &k);
    // Build SAM and tree
    for (int i = 0; origin[i] != '\0'; ++i)
        SAM::insert(origin[i] - 'a');
    for (int i = 1; i <= SAM::max; ++i)
        if (SAM::parent[i])
            ParTree::addedge(SAM::parent[i], i);
    calen(1);
    //printTree();
    if (k > substr[1])
    {
        printf("-1\n");
        return 0;
    }
    int now = 1;
    endset[1] = 0;
    while (k > 0)
    {
        int ch = 0;
        k -= endset[now];
        while (k > 0 && substr[SAM::child[now][ch]] < k && ch < Sigma)
            k -= substr[SAM::child[now][ch++]];
        if (k > 0)
            putchar(ch + 'a');
        now = SAM::child[now][ch];
    }
    printf("\n");
    return 0;
}

