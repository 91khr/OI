#include <cstdio>
#include <cstring>
#include <algorithm>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif

const int MaxN = 2000 + 7;

int n;
char s[MaxN];

int main()
{
    scanf("%s", s + 1);
    n = int(strlen(s + 1));
    return 0;
}

