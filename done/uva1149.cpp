#include <cstdio>
#include <algorithm>

const int MaxN = 1e5 + 7;

int T;
int n, m;
int weight[MaxN];

int main(void)
{
    scanf("%d", &T);
    while (T--)
    {
        scanf("%d %d", &n, &m);
        for (int i = 1; i <= n; ++i)
            scanf("%d", weight + i);
        std::sort(weight + 1, weight + n + 1);
        int ans = 0;
        // Iterate every in front
        int front = 1, back = n;  // The last one
        while (back > front)
        {
            // Find one can be put together with front
            // (cant equal to front)
            while (weight[front] + weight[back] > m && back > front + 1)
            {
                //fprintf(stderr, "pack %d alone\n", weight[back]);
                --back;
                // Must be packed alone: accmulate this
                ++ans;
            }
            // Can be put together
            if (weight[front] + weight[back] <= m)
            {
                //fprintf(stderr, "combine %d and %d\n", weight[front], weight[back]);
                // Accmulate
                ++ans;
                // Pass packed
                --back;
                ++front;
            }
            else  // weight[front] + weight[back] > m && back == front + 1
            {
                // Accmulate
                ans += 2;
                // Pack them seperately
                --back;
                ++front;
            }
        }
        // accumlate those between them
        if (front == back)
            ++ans;
        printf("%d\n", ans);
        if (T != 0)
            printf("\n");
    }
    return 0;
}

