#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DoublyNode {
    void *element;
    struct DoublyNode *prev;
    struct DoublyNode *next;
} DoublyNode;

typedef struct {
    DoublyNode *head;
    DoublyNode *tail;
    size_t elementSize;
} DoublyLinkedList;

DoublyNode *createDoublyNode(const void *element, size_t elementSize)
{
    DoublyNode *node = malloc(sizeof(DoublyNode));
    if (!node)
        return NULL;

    node->element = malloc(elementSize);
    if (!node->element) {
        free(node);
        return NULL;
    }

    memcpy(node->element, element, elementSize);

    node->prev = NULL;
    node->next = NULL;

    return node;
}

DoublyNode *createAndInsertDoublyNode(const void *element, size_t elementSize, DoublyNode *prev,
                                      DoublyNode *next)
{
    DoublyNode *newNode = createDoublyNode(element, elementSize);
    if (!newNode)
        return NULL;

    newNode->prev = prev;
    newNode->next = next;
    if (prev)
        prev->next = newNode;
    if (next)
        next->prev = newNode;

    return newNode;
}

DoublyLinkedList *createDoublyLinkedList(size_t elementsize)
{
    DoublyLinkedList *list = malloc(sizeof(DoublyLinkedList));
    if (!list)
        return NULL;

    list->head = NULL;
    list->tail = NULL;

    list->elementSize = elementsize;

    return list;
}

void destoryDoublyLinkedList(DoublyLinkedList *list)
{
    if (!list)
        return;

    DoublyNode *current = list->head;
    while (current) {
        DoublyNode *next = current->next;

        free(current->element);
        free(current);
        current = next;
    }
    free(list);
}

int insertDoublyLinkedList(DoublyLinkedList *list, int pos, const void *element)
{
    if (!list)
        return -1;

    // pos == 0 || pos == -1 需要换头尾
    if (!list->tail || !list->head) {
        if (pos != 0 && pos != -1)
            return -1;

        DoublyNode *newNode = createDoublyNode(element, list->elementSize);
        if (!newNode)
            return -1;

        newNode->prev = newNode->next = NULL;
        list->tail = newNode;
        list->head = newNode;
        return 0;
    }

    DoublyNode *current = pos >= 0 ? list->head : list->tail;
    int idx = pos >= 0 ? pos : -pos - 1;
    while (current && idx--)
        current = pos >= 0 ? current->next : current->prev;

    if (!current)
        return -1;

    DoublyNode *prev = pos >= 0 ? current->prev : current;
    DoublyNode *next = pos >= 0 ? current : current->next;

    DoublyNode *newNode = createAndInsertDoublyNode(element, list->elementSize, prev, next);
    if (!newNode)
        return -1;

    if (pos == 0)
        list->head = newNode;
    else if (pos == -1)
        list->tail = newNode;

    return 0;
}

int appendDoublyLinkedList(DoublyLinkedList *list, const void *element)
{
    return insertDoublyLinkedList(list, -1, element);
}

int prependDoublyLinkedList(DoublyLinkedList *list, const void *element)
{
    return insertDoublyLinkedList(list, 0, element);
}

int deleteElementDoublyLinkedList(DoublyLinkedList *list, int pos)
{
    if (!list)
        return -1;

    DoublyNode *current = pos >= 0 ? list->head : list->tail;
    int idx = pos >= 0 ? pos : -pos - 1;
    while (current && idx--)
        current = pos >= 0 ? current->next : current->prev;

    if (!current)
        return -1;
    if (current->prev)
        current->prev->next = current->next;
    else
        list->head = current->next;
    if (current->next)
        current->next->prev = current->prev;
    else
        list->tail = current->prev;
    free(current->element);
    free(current);

    return 0;
}

int getElementDoublyLinkedList(DoublyLinkedList *list, int pos, void **element)
{
    if (!list)
        return -1;

    DoublyNode *current = pos >= 0 ? list->head : list->tail;
    int idx = pos >= 0 ? pos : -pos - 1;
    while (current && idx--)
        current = pos >= 0 ? current->next : current->prev;

    if (!current)
        return -1;

    *element = current->element;
    return 0;
}

void printDoublyLinkedList(DoublyLinkedList *list)
{
    if (!list)
        return;

    DoublyNode *current = list->head;
    while (current) {
        printf("%d ", *(int *)current->element);
        current = current->next;
    }
    printf("\n");
}

int main()
{
    int a = 10, b = 15, c = 20, d = 101, e = 102, f = 103;

    DoublyLinkedList *list = createDoublyLinkedList(sizeof(int));

    appendDoublyLinkedList(list, &a);
    appendDoublyLinkedList(list, &b);
    appendDoublyLinkedList(list, &c);

    prependDoublyLinkedList(list, &d);
    prependDoublyLinkedList(list, &e);

    printDoublyLinkedList(list);

    insertDoublyLinkedList(list, 2, &f);
    insertDoublyLinkedList(list, -2, &f);
    insertDoublyLinkedList(list, -1, &f);

    printDoublyLinkedList(list);

    deleteElementDoublyLinkedList(list, 0);

    printDoublyLinkedList(list);
    deleteElementDoublyLinkedList(list, 3);
    printDoublyLinkedList(list);
    deleteElementDoublyLinkedList(list, -1);
    printDoublyLinkedList(list);

    int *p1;
    getElementDoublyLinkedList(list, 0, (void **)&p1);
    printf("*p1 = %d\n", *p1);

    getElementDoublyLinkedList(list, 1, (void **)&p1);
    printf("*p1 = %d\n", *p1);

    getElementDoublyLinkedList(list, 2, (void **)&p1);
    printf("*p1 = %d\n", *p1);

    getElementDoublyLinkedList(list, -1, (void **)&p1);
    printf("*p1 = %d\n", *p1);

    getElementDoublyLinkedList(list, -5, (void **)&p1);
    printf("*p1 = %d\n", *p1);
    return 0;
}
