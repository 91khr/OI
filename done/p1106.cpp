#include <cstdio>
#include <vector>
#include <cctype>

const int MaxN = 300;

std::vector<int> nums;
std::vector<int> result;
int k;

int main(void)
{
    // scan
    do
    {
        nums.push_back(getchar() - '0');
    }
    while (!feof(stdin) && isdigit(nums.back() + '0'));
    nums.pop_back();
    result.reserve(nums.size());
    scanf("%d", &k);

    for (int i = 0; i < nums.size(); ++i)
    {
        // Push nums[i] into result
        while (!result.empty() && nums[i] < result.back() && k > 0)
        {
            //printf("pop %d\n", result.back());
            result.pop_back();
            --k;
        }
        //printf("push %d\n", nums[i]);
        result.push_back(nums[i]);
    }

    bool printed = false;
    for (int i = 0; i < result.size() - k; ++i)
    {
        if (printed || result[i] != 0)
        {
            printf("%d", result[i]);
            printed = true;
        }
    }
    if (!printed)
    {
        printf("0");
    }
    printf("\n");

    return 0;
}
