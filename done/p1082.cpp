#include <cstdio>
#include <cstring>
#include <algorithm>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif

int a, b;

// ax+by=gcd(x, y)
int exgcd(int x, int y, int &a, int &b)
{
    if (y == 0)
    {
        a = 1;
        b = 0;
        return x;
    }
    int res = exgcd(y, x % y, b, a);
    // Knowing py + q(x-ty) = gcd(x, y)
    // i.e. qx + (p-qt)y = gcd(x, y)
    // so a = q, b = p - qt
    b -= x / y * a;
    return res;
}
int main()
{
    scanf("%d %d", &a, &b);
    // xa + yb = 1
    int x, y;
    exgcd(a, b, x, y);
    if (x < 0)
        x += b;
    printf("%d\n", x);
    return 0;
}

