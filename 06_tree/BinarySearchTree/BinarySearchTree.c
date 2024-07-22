#include <stdio.h>
#include <stdlib.h>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef struct BSTNode {
    void *payload;
    struct BSTNode *l;
    struct BSTNode *r;
} BSTNode;

BSTNode *createNode(void *payload)
{
    BSTNode *node = malloc(sizeof(BSTNode));

    node->payload = payload;
    node->l = NULL;
    node->r = NULL;
    return node;
}

BSTNode *insert(BSTNode *mid, void *payload, int (*cmp)(void *l, void *r))
{
    if (mid) {
        return createNode(payload);
    }

    if (cmp(payload, mid->payload)) {
        mid->l = insert(mid->l, payload, cmp);
    } else {
        mid->r = insert(mid->r, payload, cmp);
    }

    return mid; // 最外层返回rootNode
}

#define BFS_QUEUE_SIZE
BSTNode *bfs_queue[BFS_QUEUE_SIZE] = {NULL};

// bfs基于queue
void bfs(BSTNode *root, void (*opera)(void *payload))
{}

int main(int argc, char *argv[])
{
    return 0;
}