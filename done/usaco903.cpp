#include <cstdio>
#include <algorithm>

int cow[3];

int main(void)
{
#ifndef DEBUG
    freopen("herding.in", "r", stdin);
    freopen("herding.out", "w", stdout);
#endif  // DEBUG
    scanf("%d %d %d", cow, cow + 1, cow + 2);
    std::sort(cow, cow + 3);
    int sep[] = { cow[2] - cow[1], cow[1] - cow[0] };
    if (sep[0] > sep[1])
        std::swap(sep[0], sep[1]);
    if (sep[1] == 1)
        printf("0\n0\n");
    else if (sep[0] == 1)
    {
        if (sep[1] == 2)
            printf("1\n");
        else
            printf("2\n");
        printf("%d\n", sep[1] - 1);
    }
    else
    {
        if (sep[0] == 2)
            printf("1\n");
        else
            printf("2\n");
        printf("%d\n", sep[1] - 1);
    }
    return 0;
}

