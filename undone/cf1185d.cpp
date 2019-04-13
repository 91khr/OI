#include <cstdio>
#include <cmath>
#include <algorithm>

const int MaxN = int(2e5) + 7;

int n;

struct Num
{
    int val;
    int index;
    friend bool operator<(const Num &a, const Num &b)
    {
        return a.val == b.val ? a.index < b.index : a.val < b.val;
    }
};
Num nums[MaxN];
Num diff[MaxN];
Num comp[MaxN];
int count;

void scan()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i)
    {
        scanf("%d", &nums[i].val);
        nums[i].index = i;
    }
    std::sort(nums + 1, nums + n + 1);
    for (int i = 1; i < n; ++i)
    {
        diff[i].val = nums[i + 1].val - nums[i].val;
        diff[i].index = i;
    }
    std::sort(diff + 1, diff + n);
    comp[count = 1].val = diff[1].val;
    for (int i = 1; i < n; ++i)
    {
        if (diff[i].val == comp[count].val)
            ++comp[count].index;
        else
        {
            comp[++count].val = diff[i].val;
            comp[count].index = 1;
        }
    }
    /*
    for (int i = 1; i < n; ++i)
        printf("(%d,%d) ", diff[i].index, diff[i].val);
    printf("\n");
    */
}

int main()
{
    scan();
    switch (count)
    {
    case 1:
        printf("%d\n", n);
        break;
    case 2:
        {
            if (diff[1].val + diff[2].val == diff[3].val && comp[1].index == 2
                    && diff[1].index + 1 == diff[2].index)
            {
                printf("%d\n", nums[diff[2].index].index);
                break;
            }
            else if (diff[1].val == 0 && comp[1].index == 1)
            {
                printf("%d\n", nums[diff[1].index].index);
                break;
            }
            int index = comp[1].index == 1 ? diff[1].index :
                comp[2].index == 1 ? diff[n - 1].index : -1;
            if (index == n - 1)
                printf("%d\n", nums[n].index);
            else if (index == 1)
                printf("%d\n", nums[1].index);
            else
                printf("-1\n");
        }
        break;
    case 3:
        if (diff[1].val + diff[2].val == diff[3].val && diff[1].val != 0
                && abs(diff[1].index - diff[2].index) == 1)
            printf("%d\n", nums[std::max(diff[2].index, diff[1].index)].index);
        else
            printf("-1\n");
        break;
    default:
        printf("-1\n");
        break;
    }
    return 0;
}

