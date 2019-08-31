// luogu p3375
#include <cstdio>
#include <cstring>
#include <algorithm>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif

const int MaxN = int(1e6) + 7;

int n, m;
char s[MaxN], t[MaxN];
int pre[MaxN];

int main()
{
    scanf("%s%s", s + 1, t + 1);
    n = int(strlen(s + 1));
    m = int(strlen(t + 1));

    pre[1] = 0;
    for (int pos = 2; pos <= m; ++pos)
    {
        int last = pre[pos - 1];
        while (last && t[last + 1] != t[pos])
            last = pre[last];
        pre[pos] = last + (t[last + 1] == t[pos]);
    }

    int match = 0;
    for (int pos = 1; pos <= n; ++pos)
    {
        while (match && s[pos] != t[match + 1])
            match = pre[match];
        if (s[pos] == t[match + 1])
            ++match;
        if (match == m)
        {
            printf("%d\n", pos - m + 1);
            match = pre[match];
        }
    }

    for (int i = 1; i <= m; ++i)
        printf("%d ", pre[i]);
    printf("\n");

    return 0;
}

