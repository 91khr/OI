#include <cstdio>

const int MaxN = 100 + 7;

int C, T;
int n;
int src[MaxN], dst[MaxN];

void outsln(int i)
{
    fprintf(stdout, "Case %d: %d\n", C - T, i);
}

int main(void)
{
    scanf("%d", &C);
    T = C;
    getchar();  // Ignore \n
    while (T--)
    {
        {
            int peek;
            n = 0;
            int one = 0;
            while ((peek = getchar()) != '\n')
                switch (peek)
                {
                case '0':
                    src[n++] = 1;
                    break;
                case '1':
                    src[n++] = 2;
                    ++one;
                    break;
                case '?':
                    src[n++] = 0;
                    break;
                }
            n = 0;
            while ((peek = getchar()) != '\n')
            {
                dst[n] = peek - '0' + 1;
                if (dst[n++] == 2)
                    --one;
            }
            if (one)
            {
                outsln(-1);
                continue;
            }
        }
    }
    return 0;
}

