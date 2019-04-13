#include <cstdio>
#include <cctype>
#include <cmath>
#include <algorithm>

const int MaxN = 5e5 + 7;
const int MinInf = -5e5 * 1000;

int n, m;

class FHQTreap
{
public:

    struct TreeNode
    {
        bool reverse;
        bool fill;
        int lmax, rmax, max, sum;
        int value;
        TreeNode *lchild, *rchild;
        int weight;
        int width;
    };
    TreeNode node[MaxN];
    TreeNode *root;
    TreeNode *free;
    int max;

    FHQTreap(void) : root(node), free(node + 1), max(1)
    {
        node->lchild = node->rchild = node;
        node->width = 0;
        node->lmax = node->rmax = node->max = node->sum = 0;
        free->lchild = free->rchild = node;
    }

    void push(TreeNode *now)
    {
        if (now->reverse)
        {
            // Swap left and right
            std::swap(now->lchild, now->rchild);
            now->lchild->reverse ^= 1;
            now->rchild->reverse ^= 1;
            now->reverse = false;
            // Fill data
            std::swap(now->lmax, now->rmax);
        }
        if (now->fill)
        {
            // Fill child
            now->lchild->value = now->value;
            now->rchild->value = now->value;
            now->lchild->fill = now->rchild->fill = true;
            now->fill = false;
            // Set now
            now->sum = now->width * now->value;
            if (now->sum > 0)
                now->lmax = now->rmax = now->sum;
            else
                now->lmax = now->rmax = now->value;
        }
    }
    void update(TreeNode *now)
    {
        now->width = now->lchild->width + now->rchild->width + 1;
        now->sum = now->lchild->sum + now->rchild->sum + now->value;
        now->lmax = std::max(now->lchild == node ? MinInf : now->lchild->lmax,
                now->lchild->sum + now->value + now->rchild->lmax);
        now->rmax = std::max(now->rchild == node ? MinInf : now->rchild->rmax,
                now->rchild->sum + now->value + now->lchild->rmax);
        now->max = std::max(now->lchild->rmax + now->value + now->rchild->lmax,
                std::max(now->lchild == node ? MinInf : now->lchild->max,
                    now->rchild == node ? MinInf : now->rchild->max));
    }

    void split(TreeNode *now, int bound, TreeNode *&left, TreeNode *&right)
    {
        if (now == node)
        {
            left = right = node;
            return;
        }
        push(now);
        if (now->lchild->width < bound)
        {
            left = now;
            split(now->rchild, bound - now->lchild->width - 1, now->rchild, right);
        }
        else
        {
            right = now;
            split(now->lchild, bound, left, now->lchild);
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

    TreeNode *newnode(int value)
    {
        // Alloc
        TreeNode *res = free;
        free = merge(free->lchild, free->rchild);
        if (free == node)
        {
            free = node + ++max;
            free->lchild = free->rchild = node;
        }
        // Fill data
        res->reverse = res->fill = false;
        res->value = value;
        res->lmax = res->rmax = res->max = res->sum = value;
        res->lchild = res->rchild = node;
        res->weight = rand();
        res->width = 1;
        return res;
    }
    void insert(int pos, int value)
    {
        TreeNode *ins = newnode(value);
        TreeNode *left, *right;
        split(root, pos, left, right);
        root = merge(merge(left, ins), right);
    }

    void remove(int pos, int len)
    {
        TreeNode *left, *right, *mid;
        split(root, pos - 1, left, right);
        split(right, len, mid, right);
        free = merge(free, mid);
        root = merge(left, right);
    }

    void fill(int pos, int len, int value)
    {
        TreeNode *left, *mid, *right;
        split(root, pos - 1, left, right);
        split(right, len, mid, right);
        mid->fill = true;
        mid->value = value;
        push(mid);
        root = merge(merge(left, mid), right);
    }

    void reverse(int pos, int len)
    {
        TreeNode *left, *mid, *right;
        split(root, pos - 1, left, right);
        split(right, len, mid, right);
        mid->reverse ^= 1;
        push(mid);
        root = merge(merge(left, mid), right);
    }

    int sum(int pos, int len)
    {
        TreeNode *left, *mid, *right;
        split(root, pos - 1, left, right);
        split(right, len, mid, right);
        int res = mid->sum;
        root = merge(merge(left, mid), right);
        return res;
    }
};

FHQTreap tree;

int parse()
{
    int res;
    while (isspace(res = getchar()))
        ;
    switch (res)
    {
    case 'I':
        res = 1;
        break;
    case 'D':
        res = 2;
        break;
    case 'R':
        res = 3;
        break;
    case 'G':
        res = 4;
        break;
    default:
        getchar();
        if (getchar() == 'K')
            res = 5;
        else
            res = 6;
        break;
    }
    while (!isspace(getchar()))
        ;
    return res;
}

void printTree(void)
{
    printf("digraph G {\n");
    auto print = [](FHQTreap::TreeNode *now, auto fn) -> void {
        printf(" N%ld [label=\"%d %d %d %d %d %d\"]\n", now - tree.node,
                now->value, now->lmax, now->rmax, now->max, now->sum,
                now->width);
        printf("  N%ld -> N%ld [label=\"l\"]\n", now - tree.node, now->lchild - tree.node);
        printf("  N%ld -> N%ld [label=\"r\"]\n", now - tree.node, now->rchild - tree.node);
        if (now->lchild != tree.node)
            fn(now->lchild, fn);
        if (now->rchild != tree.node)
            fn(now->rchild, fn);
    };
    print(tree.root, print);
    printf("}\n");
}

int main(void)
{
    scanf("%d %d", &n, &m);
    while (n--)
    {
        int val;
        scanf("%d", &val);
        tree.insert(tree.root->width, val);
    }
    //printTree();
    while (m--)
    {
        switch(parse())
        {
        case 1:  // INSERT
            {
                int pos, len;
                scanf("%d %d", &pos, &len);
                for (; len; ++pos ,--len)
                {
                    int val;
                    scanf("%d", &val);
                    tree.insert(pos, val);
                }
                //printTree();
            }
            break;
        case 2:  // DELETE
            {
                int pos, len;
                scanf("%d %d", &pos, &len);
                tree.remove(pos, len);
                //printTree();
            }
            break;
        case 3:  // REVERSE
            {
                int pos, len;
                scanf("%d %d", &pos, &len);
                tree.reverse(pos, len);
                //printTree();
            }
            break;
        case 4:  // GET-SUM
            {
                int pos, len;
                scanf("%d %d", &pos, &len);
                printf("%d\n", tree.sum(pos, len));
                //printTree();
            }
            break;
        case 5:  // MAKE-SAME
            {
                int pos, len, val;
                scanf("%d %d %d", &pos, &len, &val);
                tree.fill(pos, len, val);
                //printTree();
            }
            break;
        case 6:  // MAX-SUM
            {
                tree.push(tree.root);
                tree.update(tree.root);
                printf("%d\n", tree.root->max);
            }
            break;
        }
    }
    return 0;
}

