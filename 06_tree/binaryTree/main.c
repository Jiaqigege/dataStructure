#include <stdio.h>

#include "binaryTree.h"

void *printInteger(BinTreeNode *node)
{
    printf("%d ", *(int *)node->element);
    return NULL;
}

int binTreeCompareInteger(void *a, void *b)
{
    int left = *(int *)a;
    int right = *(int *)b;

    if (left > right)
        return 1;
    else if (left < right)
        return -1;
    else
        return 0;
}

int main()
{
    int rootValue = 10;

    BinTree *tree = createBinTreeWithRoot(&rootValue, binTreeCompareInteger, sizeof(int));
    if (!tree)
        return -1;

    /**
     *
     *        10
     *       /  \
     *      9    12
     *     /     / \
     *    1    11   18
     *   / \       /  \
     * -3   4     15   20
     *
     */

    int a = 12, b = 9, c = 18, d = 20, e = 1, f = 11, g = 4, h = 15, i = -3;
    insertBinTree(tree, &a);
    insertBinTree(tree, &b);
    insertBinTree(tree, &c);
    insertBinTree(tree, &d);
    insertBinTree(tree, &e);
    insertBinTree(tree, &f);
    insertBinTree(tree, &g);
    insertBinTree(tree, &h);
    insertBinTree(tree, &i);

    traverseBinTree(tree, PRE_ORDER, printInteger); // 10 9 1 -3 4 12 11 18 15 20
    printf("\n");

    traverseBinTree(tree, IN_ORDER, printInteger); // -3 1 4 9 10 11 12 15 18 20
    printf("\n");

    traverseBinTree(tree, POST_ORDER, printInteger); // -3 4 1 9 11 15 20 18 12 10
    printf("\n");

    return 0;
}
