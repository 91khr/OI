#include <cstdio>

const int MaxN = 50 + 7;

int n;

void decode(int num)
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    putchar(charset[num]);
}

int main(void)
{
    while (scanf("%d", &n) == 1)
    {
        printf("%d %d %d\n", 1, n, 2 * n);
        for (int i = 0; i < n * 2; ++i)
        {
            // Decode current nation
            if (i % 2 == 0)
                for (int j = 0; j < n; ++j)
                    decode(i / 2);
            // Decode all other nations
            else
                for (int j = 0; j < n; ++j)
                    decode(j);
            printf("\n");
        }
    }
    return 0;
}
