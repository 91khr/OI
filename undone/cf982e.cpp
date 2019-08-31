#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>
void Unused(...) {}
#ifndef DEBUG
  #define echo(...) Unused(__VA_ARGS__)
#else
  #define echo(...) fprintf(stderr, __VA_ARGS__)
#endif

int n, m, x, y, vx, vy;

void printans(int xs, int ys) { printf("%d %d", xs > 0 ? n : 0, ys > 0 ? m : 0); }

int exgcd(int x, int y, int &a, int &b)
{
    if (y == 0)
    {
        a = 1;
        b = 0;
        return x;
    }
    int res = exgcd(y, x % y, b, a);
    b -= x / y * a;
    return res;
}

// x * r === 1, %p
// x * r + sp = 1
int inv(int x, int p)
{
    int s, t;
    assert(exgcd(x, p, s, t) == 1);
    return s;
}

int main()
{
    scanf("%d %d %d %d %d %d", &n, &m, &x, &y, &vx, &vy);
    // Ensure vx = vy = 1
    if (vx == 0)
    {
        if (x == n)
            printf("%d %d\n", n, vy == 1 ? m : 0);
        else
            printf("-1\n");
        return 0;
    }
    else if (vy == 0)
    {
        if (y == m)
            printf("%d %d\n", vx == 1 ? n : 0, m);
        else
            printf("-1\n");
        return 0;
    }
    x *= vx;
    y *= vy;
    // Now we can assume vx = vy = 1
    return 0;
}

/*
 * np = x + tv_x => v_x * t %=n x
 * mq = y + tv_y => v_y * t %=m y
 */

