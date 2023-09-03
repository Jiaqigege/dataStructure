#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binaryTree.h"

static BinTreeNode *createBinTreeNode(void *element, size_t elementSize)
{
    BinTreeNode *node = malloc(sizeof(BinTreeNode));
    if (!node)
        return NULL;

    node->element = malloc(elementSize);
    if (!node->element) {
        free(node);
        return NULL;
    }
    memcpy(node->element, element, elementSize);

    node->lChildNode = NULL;
    node->rChildNode = NULL;

    return node;
}

BinTree *createEmptyBinTree(binTreeCompareFunc compareFunc, size_t elementSize)
{
    BinTree *tree = malloc(sizeof(BinTree));
    if (!tree)
        return NULL;

    tree->rootNode = NULL;
    tree->compareFunc = compareFunc;
    tree->elementSize = elementSize;
    return tree;
}

BinTree *createBinTreeWithRoot(void *rootNodeValue, binTreeCompareFunc compareFunc,
                               size_t elementSize)
{
    BinTree *tree = createEmptyBinTree(compareFunc, elementSize);
    if (!tree)
        return NULL;

    tree->rootNode = createBinTreeNode(rootNodeValue, elementSize);
    if (!tree->rootNode) {
        free(tree);
        return NULL;
    }

    return tree;
}

static void recursiveInsert(BinTree *tree, BinTreeNode *node, BinTreeNode *insertNode)
{
    int ret = tree->compareFunc(node->element, insertNode->element);
    if (ret >= 0) { // left
        if (node->lChildNode) {
            recursiveInsert(tree, node->lChildNode, insertNode);
        } else {
            node->lChildNode = insertNode;
        }
    } else { // right
        if (node->rChildNode) {
            recursiveInsert(tree, node->rChildNode, insertNode);
        } else {
            node->rChildNode = insertNode;
        }
    }
}

int insertBinTree(BinTree *tree, void *element)
{
    if (!tree)
        return -1;

    BinTreeNode *node = createBinTreeNode(element, tree->elementSize);
    if (!node)
        return -1;

    if (!tree->rootNode) {
        tree->rootNode = node;
    } else {
        recursiveInsert(tree, tree->rootNode, node);
    }
    return 0;
}

static void recursivePreOrderIterTraverse(BinTree *tree, BinTreeNode *node, traverseCb func)
{
    func(node);
    if (node->lChildNode)
        recursivePreOrderIterTraverse(tree, node->lChildNode, func);
    if (node->rChildNode)
        recursivePreOrderIterTraverse(tree, node->rChildNode, func);
}

static void recursiveInOrderIterTraverse(BinTree *tree, BinTreeNode *node, traverseCb func)
{
    if (node->lChildNode)
        recursiveInOrderIterTraverse(tree, node->lChildNode, func);
    func(node);
    if (node->rChildNode)
        recursiveInOrderIterTraverse(tree, node->rChildNode, func);
}

static void recursivePostOrderIterTraverse(BinTree *tree, BinTreeNode *node, traverseCb func)
{
    if (node->lChildNode)
        recursivePostOrderIterTraverse(tree, node->lChildNode, func);
    if (node->rChildNode)
        recursivePostOrderIterTraverse(tree, node->rChildNode, func);
    func(node);
}

void traverseBinTree(BinTree *tree, enum TraverseMethod method, traverseCb cb)
{
    if (!tree || !tree->rootNode)
        return;

    switch (method) {
    case PRE_ORDER:
        recursivePreOrderIterTraverse(tree, tree->rootNode, cb);
        break;
    case IN_ORDER:
        recursiveInOrderIterTraverse(tree, tree->rootNode, cb);
        break;
    case POST_ORDER:
        recursivePostOrderIterTraverse(tree, tree->rootNode, cb);
        break;
    }
}

static void *freeBinTreeNode(BinTreeNode *node)
{
    if (node->element)
        free(node->element);

    free(node);
    return NULL;
}

void freeBinTree(BinTree *tree)
{
    traverseBinTree(tree, POST_ORDER, freeBinTreeNode);
    free(tree);
}