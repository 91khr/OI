#include <cstdio>
#include <algorithm>

const int MaxN = 37;

int n;
int pies[MaxN];

void scan(void)
{
    n = 1;
    do
    {
        scanf("%d", pies + n);
        printf("%d ", pies[n]);
        ++n;
    } while (getchar() != '\n' && !feof(stdin));
    printf("\n");
}

void flip(int pos)
{
    int beg = 1, end = pos;
    while (beg < end)
    {
        std::swap(pies[beg], pies[end]);
        ++beg; --end;
    }
    printf("%d ", n - pos);
}

int main(void)
{
    while (!feof(stdin))
    {
        scan();
        // n is 1 greater than real amount
        int rest = n;
        while (--rest > 1)
        {
            int maxnum = 0, maxpos = 0;
            // Find max
            for (int i = 1; i <= rest; ++i)
                if (pies[i] > maxnum)
                {
                    maxnum = pies[i];
                    maxpos = i;
                }
            // If already bottom, ignore
            if (maxpos == rest)
                continue;
            // If not top, flip to top
            if (maxpos != 1)
                flip(maxpos);
            // Flip to bottom
            flip(rest);
        }
        printf("0\n");
    }
    return 0;
}
