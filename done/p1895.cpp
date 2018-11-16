#include <cstdio>
#include <cmath>
#ifdef DEBUG
#include "../debug.hpp"
#else
template<typename First, typename ...Rest>
void VarLog(First, Rest...) {}
template<typename First, typename ...Rest>
void FormatLog(First, Rest...) {}
#endif

typedef long long laolin;

int t;
laolin n;

void solve(void);

int main(void)
{
    scanf("%d", &t);
    while (t--)
    {
        solve();
    }

    return 0;
}

laolin replen(laolin rep)
{
    laolin res = 0;
    laolin base = 1, upper = 10;
    while (upper <= rep)
    {
        // max = upper - 1, min = upper / 10
        // len = max - min + 1
        laolin len = upper - upper / 10;
        // cycle len times, from 1 number to len numbers, each with base digit
        res += len * (len + 1) / 2 * base;
        // len numbers, cycle rep - upper + 1 times, each with base digit
        res += len * (rep - upper + 1) * base;
        // proceed to next bit
        ++base;
        upper *= 10;
    }
    // return to last bit, but preserve base of this bit
    upper /= 10;
    // max = rep, min = upper
    // len = max - min + 1
    laolin len = rep - upper + 1;
    // cycle len times, from 1 number to len numbers, each with base digit
    res += len * (len + 1) / 2 * base;
    return res;
}

void solve(void)
{
    scanf("%lld", &n);
    // search for the last number
    {
        laolin l = 1, r = n;
        while (l < r)
        {
            laolin mid = (l + r) / 2 + 1;
            if (replen(mid) < n)
            {
                l = mid;
            }
            else
            {
                r = mid - 1;
            }
        }
        VarLog(n, n - replen(l), l);
        n -= replen(l);
    }
    // now only 1 cyclation left, from 1 to last(may be incomplete)
    // find nth number in this cyclation
    {
        laolin base = 1, upper = 10;
        laolin total = 0;
        while ((total + (upper - upper / 10) * base) < n)
        {
            // max = upper - 1, min = upper / 10
            // len = max - min + 1
            laolin len = upper - upper / 10;
            // len numbers, each with base digits
            total += len * base;
            // proceed to next bit
            ++base;
            upper *= 10;
        }
        // turn upper back to last bit
        upper /= 10;
        // now there're n - total digits left
        n -= total + 1;
        // upper is the first, each number have base digits
        // but (k*base)th digit belong to upper + k - 1
        VarLog(total, upper, n, base);
        laolin num = upper + n / base;
        // n - (num - upper + 1) * base digits left to right
        // ergo base - n % base digits right to left
        laolin shift = base - 1 - n % base;
        VarLog(num, shift);
        while (shift--)
        {
            num /= 10;
        }
        // output the last number
        printf("%lld\n", num % 10);
    }
}

