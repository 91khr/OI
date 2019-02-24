#include <cstdio>
#include <queue>
#include <algorithm>

const int MaxN = 5000 + 7;

struct Signal
{
    int pos, id;
    int endpos;
    enum {
        Begin,
        End,
    } type;
    friend bool operator < (const Signal &a, const Signal &b)
    {
        return a.pos < b.pos;
    }
    friend bool operator > (const Signal &a, const Signal &b)
    {
        return a.endpos > b.endpos;
    }
};

int n;
Signal sig[2][MaxN * 2];
int pos[2][MaxN];
bool used[MaxN];
std::priority_queue<Signal, std::vector<Signal>, std::greater<Signal>> on;

bool iteraxis(int ax)
{
    int now = 0;
    for (int i = 1; i <= n; ++i)
    {
        // Push signal
        while (sig[ax][now].pos <= i)
        {
            if (sig[ax][now].type == Signal::Begin)
            {
                used[sig[ax][now].id] = true;
                on.push(sig[ax][now]);
            }
            else if (sig[ax][now].type == Signal::End && used[sig[ax][now].id])
            {
                printf("IMPOSSIBLE\n");
                return false;
            }
        }
        // Find an available
        while (!used[on.top().id])
        {
            on.pop();
            // On every row there must be sth
            if (on.empty())
            {
                printf("IMPOSSIBLE\n");
                return false;
            }
        }
        // Push into answer
        pos[ax][on.top().id] = i;
        used[on.top().id] = false;
        on.pop();
    }
    return true;
}

int main(void)
{
    while (scanf("%d", &n), n != 0)
    {
        for (int i = 0; i < n; ++i)
        {
            const int endpos =  i * 2 + 1;
            scanf("%d %d %d %d", &sig[0][i].pos, &sig[1][i].pos,
                &sig[0][endpos].pos, &sig[1][endpos].pos);
            sig[0][i].id = sig[1][i].id = i;
            sig[0][endpos].id = sig[1][endpos].id = i;
            sig[0][i].type = sig[1][i].type = Signal::Begin;
            sig[0][endpos].type = sig[1][i].type = Signal::End;
            sig[0][i].endpos = sig[1][i].endpos = endpos;
        }
        if (iteraxis(0) && iteraxis(1))
            for (int i = 0; i < n; ++i)
                printf("%d %d\n", pos[0][i], pos[1][i]);
    }
    return 0;
}
