#include <cstdio>

typedef long long laolin;

const int MaxN = 1e5 + 7;

int n, m;

class FHQTreap
{
private:
    struct TreeNode
    {
        int weight;
        int width;
        TreeNode *lchild, *rchild;
        laolin value;
        laolin inc;
    };
    TreeNode *root;
    TreeNode node[MaxN];
    int id;
};

int main()
{
    return 0;
}

