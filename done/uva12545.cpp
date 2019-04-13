#include <cstdio>
#include <algorithm>

const int MaxN = 100 + 7;

int C, T;
int n;
int src[MaxN], dst[MaxN];

void outsln(int i)
{
    fprintf(stdout, "Case %d: %d\n", C - T, i);
}

void scan(void)
{
    int peek;
    n = 0;
    while ((peek = getchar()) != '\n')
        src[n++] = peek == '?' ? -1 : peek - '0';
    n = 0;
    while ((peek = getchar()) != '\n')
        dst[n++] = peek == '?' ? -1 : peek - '0';
}

int main(void)
{
    scanf("%d", &C);
    T = C;
    getchar();  // Ignore \n
    while (T--)
    {
        scan();
        // Accumulate one and wc
        // upone: 1 0
        // downone: 0 1
        // wcfree: -1 0
        // wcone: -1 1
        int upone = 0, downone = 0, wcfree = 0, wcone = 0;
        for (int i = 0; i < n; ++i)
            if (src[i] == 1 && dst[i] == 0)
                ++upone;
            else if (src[i] == -1)
                if (dst[i] == 0)
                    ++wcfree;
                else
                    ++wcone;
            else if (dst[i] == 1 && src[i] == 0)
                ++downone;
        // More 1 in up than down: no solution
        if (upone > downone + wcone)
            outsln(-1);
        // Some must be hided or changed
        else
            outsln(std::max(upone, downone) + wcone + wcfree);
    }
    return 0;
}

