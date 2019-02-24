#include <cstdio>
#include <cstring>
#include <vector>

const int MaxN = 100 + 7;
const int MaxM = 150 + 7;

int n, m;
std::pair<int, int> fr[MaxM];
int ans[MaxN];
bool color[5];
std::vector<int> adj[MaxN];

int main(void)
{
#ifndef DEBUG
    freopen("revegetate.in", "r", stdin);
    freopen("revegetate.out", "w", stdout);
#endif
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i)
    {
        scanf("%d %d", &fr[i].first, &fr[i].second);
        adj[fr[i].first].push_back(fr[i].second);
        adj[fr[i].second].push_back(fr[i].first);
    }
    memset(ans, -1, sizeof(ans));
    ans[1] = 1;
    for (int i = 1; i <= n; ++i)
    {
        //printf("point %d\n", i);
        color[1] = color[2] = color[3] = color[4] = true;
        // Iterate all neighbor to find unused color
        for (int j = 0; j < adj[i].size(); ++j)
        {
            //printf("  search %d with color %d\n", adj[i][j], ans[adj[i][j]]);
            if (ans[adj[i][j]] != -1)
                color[ans[adj[i][j]]] = false;
        }
        // Find first unused color
        for (int j = 1; j <= 4; ++j)
            if (color[j])
            {
                ans[i] = j;
                break;
            }
    }
    // output answer
    for (int i = 1; i <= n; ++i)
        printf("%d", ans[i]);
    printf("\n");
    return 0;
}

