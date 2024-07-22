#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

/* * * * * Queue * * * * */
#define QUEUE_MAX_SIZE (20)
typedef struct CircularQueue {
    void *data[QUEUE_MAX_SIZE];
    int front;
    int rear;
} CircularQ;

static inline bool isEmpty(CircularQ *q)
{
    return q->front == q->rear;
}

static inline bool isFull(CircularQ *q)
{
    return (q->front + 1) % QUEUE_MAX_SIZE == q->rear;
}

int enqueue(CircularQ *q, void *value)
{
    if (isFull(q)) {
        printf("Queue is Full!\n");
        return -1;
    }

    q->data[q->front] = value;
    q->front = (q->front + 1) % QUEUE_MAX_SIZE;
    return 0;
}

int dequeue(CircularQ *q, void **value)
{
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return -1;
    }

    *value = q->data[q->rear];
    q->rear = (q->rear + 1) % QUEUE_MAX_SIZE;
    return 0;
}
/* * * * * * * * * * * * */

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

BSTNode *insert(BSTNode *mid, void *payload, bool (*left_before)(void *l, void *r))
{
    if (!mid) {
        return createNode(payload);
    }

    if (left_before(payload, mid->payload)) {
        mid->l = insert(mid->l, payload, left_before);
    } else {
        mid->r = insert(mid->r, payload, left_before);
    }

    return mid; // 最外层返回rootNode
}

#define BFS_QUEUE_SIZE
BSTNode *bfs_queue[BFS_QUEUE_SIZE] = {NULL};

// bfs基于queue
void bfs(BSTNode *root, void (*opera)(void *payload))
{
    CircularQ q;
    q.front = q.rear = 0;

    BSTNode *curr = NULL;

    enqueue(&q, (void *)root);
    while (!isEmpty(&q)) {
        if (dequeue(&q, (void **)&curr) != 0)
            break;
        opera(curr->payload);
        if (curr->l != NULL)
            enqueue(&q, (void *)curr->l);
        if (curr->r != NULL)
            enqueue(&q, (void *)curr->r);
    }
}

bool left_before(void *l, void *r)
{
    return *(int *)l < *(int *)r;
}

void operation(void *payload)
{
    printf("val: %d\n", *(int *)payload);
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
int main(int argc, char *argv[])
{
    BSTNode *root = NULL;
    int arr[] = {3, 5, 1, 4, 7, 10, 12, 2};
    for (int i = 0; i < ARRAY_SIZE(arr); ++i)
        root = insert(root, (void *)&arr[i], left_before);

    bfs(root, operation);
    return 0;
}