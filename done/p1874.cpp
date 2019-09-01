#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 40 + 7;
const int MaxM = 1e5 + 7;
const int Inf = 0x3f3f3f3f;

char str[MaxN];
int n, m;

int info[MaxN][MaxM];
int dp(int pos, int sum)
{
    if (pos == 0)
        return sum == str[0] ? 0 : Inf;
    int &ans = info[pos][sum];
    if (ans < Inf)
        return ans;
    //printf("calc %d %d\n", pos, sum);
    int val = str[pos], exp = 10;
    int last = pos - 1;
    for (; last >= 0 && val <= sum;
            val += str[last] * exp, exp *= 10, --last)
    {
        //printf("update %d by %d(%d)\n", last, dp(last, sum - val), val);
        ans = std::min(ans, dp(last, sum - val) + 1);
    }
    if (last <= -1 && val == sum)
        ans = 0;
    //printf("%d %d into %d\n", pos, sum, ans);
    return ans;
}

int main()
{
    // Init
    {
        scanf("%s%d", str, &m);
        int i = 0;
        for (; str[i] != '\0'; ++i)
            str[i] -= '0';
        n = i;
    }
    memset(info, Inf, sizeof(info));
    int ans = dp(n - 1, m);
    if (ans >= Inf)
        printf("-1\n");
    else
        printf("%d\n", ans);
    return 0;
}

