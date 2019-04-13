#include <cstdio>

const int MaxN = 3000 + 7;
const int MaxL = 30000 + 7;

int n, m;
int lens[MaxN];

int main()
{
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%d", lens + i);
    return 0;
}

