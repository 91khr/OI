#include <cstdio>
#include <cstring>
#include <algorithm>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif

typedef long long laolin;

const int MaxN = 1e6 + 7;

int n;
int hei[2][MaxN];

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%d", hei[0] + i);
    for (int i = 1; i <= n; ++i)
        scanf("%d", hei[1] + i);
    return 0;
}

