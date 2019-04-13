#include <cstdio>

const int MaxN = 1e5 + 7;

int n, d;
int nums[MaxN];
int ans[MaxN];
int anslen;

int main(void)
{
    while (scanf("%d %d", &n, &d), n != 0)
    {
        getchar();
        // Scan
        for (int i = 0; i < n; ++i)
            nums[i] = getchar() - '0';
        anslen = 0;
        // For each try to push
        for (int now = 0; now < n; ++now)
        {
            while (anslen > 0 && nums[ans[anslen - 1]] < nums[now] && d > 0)
            {
                --anslen;
                --d;
            }
            ans[anslen++] = now;
        }
        // Output
        for (int i = 0; i < anslen - d; ++i)
            printf("%d", nums[ans[i]]);
        printf("\n");
    }
    return 0;
}

