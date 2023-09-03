#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stddef.h>

typedef int (*binTreeCompareFunc)(void *a, void *b); // Returns <0 if a<b, 0 if a==b, >0 if a>b

typedef struct BinTreeNode {
    void *element;
    struct BinTreeNode *lChildNode;
    struct BinTreeNode *rChildNode;
} BinTreeNode;

typedef struct BinTree {
    BinTreeNode *rootNode;
    binTreeCompareFunc compareFunc;
    size_t elementSize;
} BinTree;

BinTree *createEmptyBinTree(binTreeCompareFunc compareFunc, size_t elementSize);
BinTree *createBinTreeWithRoot(void *rootNodeValue, binTreeCompareFunc compareFunc,
                               size_t elementSize);
int insertBinTree(BinTree *tree, void *element);

enum TraverseMethod
{
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER
};
typedef void *traverseCb(BinTreeNode *node);
void traverseBinTree(BinTree *tree, enum TraverseMethod method, traverseCb cb);

#endif // BINARY_TREE_H
