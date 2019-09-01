#include <cstdio>
#include <ctime>
#include <queue>
#include <vector>
#include <algorithm>

const int MaxN = 1e6 + 7;

int n;

class Heap
{
public:
    int datum[MaxN];
    int bound;

    int parent(int now) { return now / 2; }
    int lchild(int now) { return now * 2; }
    int rchild(int now) { return now * 2 + 1; }

    int touch(int now, int next)
    {
        return next <= bound && datum[now] < datum[next] ? next : now;
    }

public:
    Heap() : bound(0) {}

    void push(int now)
    {
        datum[++bound] = now;
        for (int pos = bound; pos > 1 && datum[parent(pos)] < datum[pos]; pos = parent(pos))
            std::swap(datum[pos], datum[parent(pos)]);
    }

    void pop()
    {
        datum[1] = datum[bound--];
        for (int last = 1, pos = 1; (pos = touch(touch(pos, lchild(pos)), rchild(pos))) != last; last = pos)
            std::swap(datum[last], datum[pos]);
    }

    int top()
    {
        return datum[1];
    }
};

namespace Test
{
int randint(int lo, int hi)
{
    return rand() % (hi - lo + 1) + lo;
}
struct Data
{
    int op, num;
};
std::vector<Data> gendata()
{
    std::vector<Data> res(randint(5, 15));
    int count = 0;
    for (auto &i : res)
    {
        if (count == 0)
        {
            i.op = 1;
            ++count;
        }
        else
        {
            i.op = randint(1, 3);
            count += 2 - i.op;
        }
        if (i.op == 1)
            i.num = randint(5, 10);
    }
    return res;
}

void printAns(std::vector<Data> in)
{
    printf("%d\n", in.size());
    for (auto it : in)
    {
        printf("%d", it.op);
        if (it.op == 1)
            printf(" %d", it.num);
        printf("\n");
    }
}

void test()
{
    srand(20030125);
    for (int idx = 0; true; ++idx)
    {
        if (idx % 10000 == 0)
        {
            printf("\rdata %d                             ", idx);
            fflush(stdout);
        }
        auto data = gendata();
        //printAns(data);
        // Run and record ans of std
        //std::priority_queue<int, std::vector<int>, std::greater<int>> std;
        std::priority_queue<int> std;
        std::vector<int> stdans;
        for (auto it : data)
            switch (it.op)
            {
            case 1:
                std.push(it.num);
                break;
            case 2:
                stdans.push_back(std.top());
                break;
            case 3:
                std.pop();
                break;
            }
        // Run and record ans of tree
        if (idx % 10000 == 0)
            printf("run tree...");
        Heap tree;
        std::vector<int> treeans;
        for (auto it : data)
            switch (it.op)
            {
            case 1:
                tree.push(it.num);
                break;
            case 2:
                treeans.push_back(tree.top());
                break;
            case 3:
                tree.pop();
                break;
            }
        // Compare both anses
        if (stdans != treeans)
        {
            printf("failed:\n");
            printAns(data);
            break;
        }
    }
}
}

Heap tree;

int main()
{
    Test::test();
    return 0;
    scanf("%d", &n);
    while (n--)
    {
        int op, num;
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            scanf("%d", &num);
            tree.push(num);
            break;
        case 2:
            /*
            printf("ans: %d(%d: ", tree.top(), tree.bound);
            for (int i = 1; i <= tree.bound; ++i)
                printf("%d ", tree.datum[i]);
            printf(")\n");
            */
            printf("%d\n", tree.top());
            break;
        case 3:
            tree.pop();
            break;
        }
    }
    return 0;
}

