#include <cstdio>
#include <algorithm>
#include <cmath>

const int MaxN = 1e5 + 7;

class FHQTreap
{
public:

    struct TreeNode
    {
        TreeNode *lchild, *rchild;
        int width;
        bool reverse;
        int value;
        int weight;
    };
    TreeNode node[MaxN];
    int max;
    TreeNode *root;

    FHQTreap(void) : max(0), root(node)
    {
        node->lchild = node->rchild = node;
        node->width = 0;
        node->value = -1;
        node->reverse = false;
    }

    void push(TreeNode *now)
    {
        if (now->reverse && now != node)
        {
            now->reverse = false;
            std::swap(now->lchild, now->rchild);
            now->lchild->reverse ^= 1;
            now->rchild->reverse ^= 1;
        }
    }
    void update(TreeNode *now)
    {
        now->width = now->lchild->width + now->rchild->width + 1;
    }

    void split(TreeNode *now, int size, TreeNode *&left, TreeNode *&right)
    {
        if (now == node)
        {
            left = right = node;
            return;
        }
        push(now);
        if (now->lchild->width < size)
        {
            left = now;
            split(now->rchild, size - now->lchild->width - 1, now->rchild, right);
        }
        else
        {
            right = now;
            split(now->lchild, size, left, now->lchild);
        }
        update(now);
    }

    TreeNode *merge(TreeNode *left, TreeNode *right)
    {
        push(left);
        push(right);
        if (left == node)
            return right;
        else if (right == node)
            return left;
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

    TreeNode *newnode(int id)
    {
        TreeNode *now = node + ++max;
        now->lchild = now->rchild = node;
        now->width = 1;
        now->reverse = false;
        now->value = id;
        now->weight = rand();
        return now;
    }
    void insert(int id)
    {
        TreeNode *left, *right;
        split(root, id, left, right);
        root = merge(left, merge(newnode(id), right));
    }

    void reverse(int invl, int invr)
    {
        TreeNode *left, *mid, *right;
        split(root, invr, left, right);
        split(left, invl - 1, left, mid);
        mid->reverse ^= 1;
        push(mid);
        root = merge(left, merge(mid, right));
    }

    int inrank(int rank)
    {
        TreeNode *left, *mid, *right;
        split(root, rank, left, right);
        split(left, rank - 1, left, mid);
        root = merge(left, merge(mid, right));
        return mid->value;
    }
};

int n, m;
FHQTreap tree;

void printTree(void)
{
    for (int i = 1; i <= n; ++i)
        printf("%d ", tree.inrank(i));
    printf("\n");
}

int main(void)
{
    srand(169980034);
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i)
        tree.insert(i);
    //printTree();
    while (m--)
    {
        int l, r;
        scanf("%d %d", &l, &r);
        tree.reverse(l, r);
        //printTree();
    }
    printTree();
    return 0;
}

