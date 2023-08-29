#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doublyLinkedList.h"

static DoublyNode *createDoublyNode(const void *element, size_t elementSize)
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

static DoublyNode *createAndInsertDoublyNode(const void *element, size_t elementSize,
                                             DoublyNode *prev, DoublyNode *next)
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

int rotateTailWithNewData(DoublyLinkedList *list, const void *element)
{
    if (!list)
        return -1;
    if (!list->head || !list->tail)
        return -1;

    if (list->tail != list->head) {
        list->head->prev = list->tail;
        list->tail->next = list->head;
        list->head = list->tail;

        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
        list->head->prev = NULL;
    }

    memcpy(list->head->element, element, list->elementSize);

    return 0;
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

typedef int (*match_method)(const void *listValue, const void *compareValue);

int matchElementDoublyLinkedList(DoublyLinkedList *list, match_method match,
                                 const void *compareValue, int *pos, void **listValue)
{
    int res;
    if (!list)
        return -1;

    int idx = 0;
    DoublyNode *current = list->head;
    while (current) {
        res = match(current->element, compareValue);
        if (res == 0) {
            if (pos)
                *pos = idx;
            if (listValue)
                *listValue = current->element;
            return 0;
        }
        current = current->next;
        ++idx;
    }
    return 1;
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

static int match_integer(const void *listValue, const void *compareValue)
{
    int *left = (int *)listValue;
    int *right = (int *)compareValue;
    if (*left == *right)
        return 0;
    else
        return -1;
}

int dobulyLinkedListTest(void)
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

    int pos;
    int compareValue = 20;
    int *listvalue;
    int ret =
        matchElementDoublyLinkedList(list, match_integer, &compareValue, &pos, (void **)&listvalue);
    if (ret != 0 && ret != 1) {
        printf("match error!");
        return -1;
    }
    if (ret == 0) {
        printf("pos %d, listvalue = %d\n", pos, *listvalue);
    } else {
        printf("doesn't match.\n");
    }

    compareValue = 0;
    ret =
        matchElementDoublyLinkedList(list, match_integer, &compareValue, &pos, (void **)&listvalue);
    if (ret != 0 && ret != 1) {
        printf("match error!");
        return -1;
    }
    if (ret == 0) {
        printf("pos %d, listvalue = %d\n", pos, *listvalue);
    } else {
        printf("doesn't match.\n");
    }

    printDoublyLinkedList(list);

    int g = 6000, h = 7000;
    rotateTailWithNewData(list, &g);
    rotateTailWithNewData(list, &h);
    printDoublyLinkedList(list);

    deleteElementDoublyLinkedList(list, 0);
    deleteElementDoublyLinkedList(list, 0);
    deleteElementDoublyLinkedList(list, -1);
    deleteElementDoublyLinkedList(list, -2);
    printDoublyLinkedList(list);

    int i = 8000;
    rotateTailWithNewData(list, &g);
    printDoublyLinkedList(list);

    prependDoublyLinkedList(list, &i);
    printDoublyLinkedList(list);

    int j = 9000;
    rotateTailWithNewData(list, &j);
    printDoublyLinkedList(list);

    return 0;
}
