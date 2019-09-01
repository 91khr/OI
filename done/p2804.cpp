#include <cstdio>
#include <cmath>

const int MaxN = 2e5 + 7;

int n, m;
int nums[MaxN];

class FHQTreap
{
private:
    struct TreeNode
    {
        int weight;
        int width;
        TreeNode *lchild, *rchild;
        int value;
        int modi;
    };
    TreeNode node[MaxN];
    TreeNode *root;
    int maxid;

    void push(TreeNode *now)
    {
        if (now->modi != 0)
        {
            now->lchild->value += now->modi;
            now->rchild->value += now->modi;
            now->lchild->modi += now->modi;
            now->rchild->modi += now->modi;
            now->modi = 0;
        }
    }
    void update(TreeNode *now)
    {
        if (now == node)
            return;
        now->width = now->lchild->width + now->rchild->width + 1;
    }

    void split(TreeNode *now, int pivot, TreeNode *&left, TreeNode *&right)
    {
        if (now == node)
        {
            left = right = node;
            return;
        }
        push(now);
        if (now->value <= pivot)
        {
            left = now;
            split(now->rchild, pivot, now->rchild, right);
        }
        else
        {
            right = now;
            split(now->lchild, pivot, left, now->lchild);
        }
        update(left);
        update(right);
    }
    TreeNode *merge(TreeNode *left, TreeNode *right)
    {
        if (left == node)
            return right;
        else if (right == node)
            return left;
        push(left);
        push(right);
        TreeNode *res;
        if (left->weight > right->weight)
        {
            res = left;
            res->rchild = merge(res->rchild, right);
        }
        else
        {
            res = right;
            res->lchild = merge(left, res->lchild);
        }
        update(res);
        return res;
    }

    void init(TreeNode *now, int value)
    {
        now->weight = rand();
        now->width = 1;
        now->lchild = now->rchild = node;
        now->value = value;
        now->modi = 0;
    }

public:
    FHQTreap()
        : root(node), maxid(0)
    {
        node->weight = 0;
        node->width = 0;
        node->lchild = node->rchild = node;
        node->value = 0;
        node->modi = 0;
    }

    void insert(int value)
    {
        TreeNode *left, *right;
        split(root, value, left, right);
        init(node + ++maxid, value);
        root = merge(merge(left, node + maxid), right);
    }

    void add(int value)
    {
        root->modi = value;
        root->value += value;
    }

    int query(int value)
    {
        TreeNode *left, *right;
        split(root, value, left, right);
        int ans = right->width;
        root = merge(left, right);
        return ans;
    }

    /*
    void printTree(TreeNode *n = nullptr)
    {
        if (n == nullptr)
        {
            printf(" N%d [label=\"%d\"]\n", 0, node->value);
            n = root;
        }
        else if (n == node)
            return;
        printf(" N%d [label=\"%d %d %d\"]\n", n - node, n - node, n->value, n->modi);
        printf("  N%d -> N%d [label=\"l\"]\n", n - node, n->lchild - node);
        printf("  N%d -> N%d [label=\"r\"]\n", n - node, n->rchild - node);
        printTree(n->lchild);
        printTree(n->rchild);
    }
    */
};
FHQTreap tree;

int main()
{
    srand(20030125);
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%d", nums + i);

    int ans = 0;
    for (int i = n; i >= 1; --i)
    {
        tree.add(nums[i] - m);
        tree.insert(nums[i] - m);
        /*
        printf("```dot\n");
        printf("digraph G {\n");
        tree.printTree();
        printf("}\n""```\n");
        */
        int now = tree.query(0);
        //printf("%d after %d(%d)\n", now, i, ans);
        ans = (ans + now) % 92084931;
    }
    printf("%d\n", ans);
    return 0;
}

