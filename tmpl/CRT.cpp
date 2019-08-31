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

typedef long long laolin;

const int MaxN = int(1e5) + 7;

int n;
laolin a[MaxN], b[MaxN];

laolin exgcd(laolin x, laolin y, laolin &s, laolin &t)
{
    if (y == 0)
    {
        s = 1;
        t = 0;
        return x;
    }
    laolin res = exgcd(y, x % y, t, s);
    t -= x / y * s;
    return res;
}


laolin mul(laolin base, laolin time, laolin mod)
{
    assert(time >= 0);
    laolin res = 0;
    for (; time; time >>= 1, base = base * 2 % mod)
        if (time & 1)
            res = (res + base) % mod;
    return res;
}

int main()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
        scanf("%lld %lld", a + i, b + i);
    // Apply solution to equation
    laolin ans = b[1], mod = a[1];
    for (int equ = 2; equ <= n; ++equ)
    {
        // s * mod + t * a_i = gcd(mod, a_i)
        laolin s, t;
        laolin lcm = exgcd(mod, a[equ], s, t);
        // na, s.t. na = u * mod + ans = v * a_i + b_i, i.e. u * mod - v * a_i = b_i - ans
        // By the way, ensure esr > 0
        laolin esr = (b[equ] - ans % a[equ] + a[equ]) % a[equ];
        assert(esr % lcm == 0);
        esr /= lcm;
        // na = ans + mod * s * esr
        ans += mod * mul(s, esr, a[equ] / lcm);
        mod *= a[equ] / lcm;
        // Restrict ans in mod
        ans = (ans % mod + mod) % mod;
    }
    printf("%lld\n", ans);
    return 0;
}

