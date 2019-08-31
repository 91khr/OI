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

const int Mod = int(1e9) + 7;

laolin mat[][2] = {
    0, 1,
    1, 1,
};
laolin fac[2][2], res[2][2];
laolin init[] = { 0, 1, };
laolin n;

void copy(laolin (*dst)[2], laolin (*src)[2])
{
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            dst[i][j] = src[i][j];
}
void mul(laolin (*lhs)[2], laolin (*rhs)[2])
{
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
        {
            res[i][j] = 0;
            for (int k = 0; k < 2; ++k)
                res[i][j] = (res[i][j] + lhs[i][k] * rhs[k][j] % Mod) % Mod;
        }
}

int main()
{
    scanf("%lld", &n);
    copy(fac, mat);

    for (; n; n >>= 1, mul(fac, fac), copy(fac, res))
        if (n & 1)
            mul(mat, fac), copy(mat, res);
    
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
            echo("%lld ", mat[i][j]);
        echo("\n");
    }

    printf("%lld\n", mat[0][0]);

    return 0;
}

