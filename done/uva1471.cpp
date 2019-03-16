#include <cstdio>
#include <map>

const int MaxN = 2e5 + 7;

int T;
int n;
int nums[MaxN];
int prae[MaxN], post[MaxN];
std::map<int, int> value;

int main(void)
{
    scanf("%d", &T);
    while (T--)
    {
        scanf("%d", &n);
        for (int i = 1; i <= n; ++i)
            scanf("%d", nums + i);
        value.clear();
        // Accumlate prae and post
        prae[0] = 0;
        for (int i = 1; i <= n; ++i)
            if (nums[i] > nums[i - 1])
                prae[i] = prae[i - 1] + 1;
            else
                prae[i] = 1;
        nums[n + 1] = 0;
        post[n + 1] = 0;
        for (int i = n; i >= 1; --i)
            if (nums[i] < nums[i + 1])
                post[i] = post[i + 1] + 1;
            else
                post[i] = 1;
        // Accumlate max
        int maxlen = 1;
        for (int i = 1; i <= n; ++i)
        {
            //fprintf(stderr, "exam pos %d(%d)\n", i, nums[i]);
            // Calc max (and this is the insert pos)
            std::map<int, int>::iterator inspos = value.lower_bound(nums[i]);
            // If not at beginning and not unfindable
            if (inspos != value.begin())
            {
                maxlen = std::max(maxlen, (--inspos)->second + post[i]);
                //fprintf(stderr, "  find %d, refresh with %d\n", inspos->first, inspos->second + post[i]);
                // If last > now: not worth to reserve
                if (inspos->second > prae[i])
                    continue;
            }
            // Worth to stay: insert
            inspos = value.find(nums[i]);
            if (inspos == value.end() || inspos->second < prae[i])
            {
                //fprintf(stderr, "  refresh %d with %d\n", nums[i], prae[i]);
                value[nums[i]] = prae[i];
            }
            // Iterate to remove
            inspos = value.upper_bound(nums[i]);
            while (inspos != value.end() && inspos->second < prae[i])
            {
                //fprintf(stderr, "    remove %d %d\n", inspos->first, inspos->second);
                value.erase(inspos++);
            }
        }
        printf("%d\n", maxlen);
    }
    return 0;
}

