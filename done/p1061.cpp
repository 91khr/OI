#include <cstdio>

int s, t, w;
char num[30];

bool inc()
{
    int i;
    for (i = 1; i <= w; ++i)
    {
        //printf("try %c to %c by %d(%c)\n", num[w - i], num[w - i] + i, i, 'a' + t);
        if (num[w - i] + i < 'a' + t)
        {
            char base = ++num[w - i] + 1;
            for (int j = w - i + 1; j < w; ++j, ++base)
                num[j] = base;
            return true;
        }
    }
    return false;
}

int main()
{
    scanf("%d %d %d", &s, &t, &w);
    scanf("%s", num);
    for (int i = 0; i < 5; ++i)
        if (inc())
            printf("%s\n", num);
        else
            break;
    return 0;
}

