#include <cstdio>
#include <algorithm>

const int MaxL = 11;
const int MaxN = 1000 + 7;

struct Name
{
    int data[MaxL];
    int get(int index) const
    {
        int at = index / 6, shift = 25 - (index % 6) * 5;
        return (data[at] & (0x1f << shift)) >> shift;
    }
    void set(int index, int value)
    {
        int at = index / 6, shift = 25 - (index % 6) * 5;
        data[at] &= ~(0x1f << shift);  // Clear to 0
        data[at] |= value << shift;  // Set to value
    }
    // When stdin is on the first char, read the \n
    void scan()
    {
        int index = 0;
        int peek;
        while ((peek = getchar()) != '\n')
        {
            if (index % 6 == 0)
                data[index / 6] = 0;
            set(index++, peek - 'A' + 1);
        }
        data[index % 6 == 0 ? index / 6 : index / 6 + 1] = 0;
    }
    void print(void)
    {
        for (int index = 0, ch; (ch = get(index)); ++index)
        {
            fflush(stdout);
            fputc(ch + 'A' - 1, stdout);
        }
        printf("\n");
    }
    friend bool operator < (const Name &a, const Name &b)
    {
        int i = 0;
        for (; a.data[i] && b.data[i]; ++i)
            if (a.data[i] != b.data[i])
                return a.data[i] < b.data[i];
        return a.data[i] == 0;
    }
};

int n;
Name names[MaxN];

int main(void)
{
    while (scanf("%d", &n) && n)
    {
        getchar();  // ignore \n
        for (int i = 0; i < n; ++i)
            names[i].scan();
        std::sort(names, names + n);
        // Find middle
        Name &l = names[n / 2 - 1], &r = names[n / 2];
        Name output;
        int index = 0;
        // For each bit
        for (; l.get(index) && r.get(index); ++index)
            if (l.get(index) != r.get(index))
            {
                if (l.get(index + 1) == 0)
                    output.set(index, l.get(index));
                else if (r.get(index) == l.get(index) + 1 && r.get(index + 1) == 0)
                {
                    // First copy this
                    output.set(index, l.get(index));
                    ++index;
                    // Then copy until can +1
                    for (; l.get(index) != 0; ++index)
                        // Not at end but can +1
                        // If at end, just copy
                        if (l.get(index) < 26 && l.get(index + 1) != 0)
                        {
                            output.set(index, l.get(index) + 1);
                            ++index;
                            break;
                        }
                        else
                            output.set(index, l.get(index));
                    --index;  // Quit to last copied
                }
                else
                    output.set(index, l.get(index) + 1);
                ++index;
                break;
            }
            else  // Copy since cant modify
                output.set(index, l.get(index));
        output.set(index, 0);
        output.print();
    }
    return 0;
}

