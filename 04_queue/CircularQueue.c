
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct CircularQueue {
    int *data;
    int size;
    int front;
    int rear;
} CircularQ;

CircularQ *newCircularQueue(int size)
{
    CircularQ *q = malloc(sizeof(CircularQ));
    q->data = malloc(sizeof(int) * size);

    q->size = size;
    q->front = q->rear = 0;
    return q;
}

void destroyCircularQueue(CircularQ *q)
{
    free(q->data);
    q->data = NULL;
    free(q);
}

static bool isEmpty(CircularQ *q)
{
    return q->front == q->rear;
}

static bool isFull(CircularQ *q)
{
    return (q->front + 1) % q->size == q->rear;
}

int enqueue(CircularQ *q, int value)
{
    if (isFull(q)) {
        printf("Queue is Full!\n");
        return -1;
    }

    q->data[q->front] = value;

    q->front = (q->front + 1) % q->size;
    return 0;
}

int dequeue(CircularQ *q, int *value)
{
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return -1;
    }

    *value = q->data[q->rear];
    q->rear = (q->rear + 1) % q->size;
    return 0;
}

void printQueue(CircularQ *q)
{
    if (isEmpty(q))
        return;

    int i = q->rear;
    printf("Queue content: ");
    while (i != q->front) {
        printf("%d ", q->data[i]);
        i = (i + 1) % q->size;
    }
    printf("\n");
}

void testQueue()
{
    CircularQ *q = newCircularQueue(5);

    printf("Enqueuing 1, 2, 3, 4\n");
    enqueue(q, 1);
    enqueue(q, 2);
    enqueue(q, 3);
    enqueue(q, 4);
    printQueue(q);

    printf("Enqueuing 5\n");
    enqueue(q, 5);

    int value;

    dequeue(q, &value);
    printf("Dequeued: %d\n", value);
    printQueue(q);

    printf("Enqueuing 5\n");
    enqueue(q, 5);
    printQueue(q);

    dequeue(q, &value);
    printf("Dequeued: %d\n", value);
    printQueue(q);

    printf("Enqueuing 6\n");
    enqueue(q, 6);
    printQueue(q);

    printf("Dequeuing all\n");
    while (!isEmpty(q)) {
        dequeue(q, &value);
        printf("Dequeued: %d\n", value);
    }
    printQueue(q);

    destroyCircularQueue(q);
}
int main()
{
    testQueue();

    return 0;
}