#ifndef DOUBLY_LINKEDLIST_H
#define DOUBLY_LINKEDLIST_H

#include <stddef.h>

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

DoublyLinkedList *createDoublyLinkedList(size_t elementsize);
void destoryDoublyLinkedList(DoublyLinkedList *list);
int insertDoublyLinkedList(DoublyLinkedList *list, int pos, const void *element);
int appendDoublyLinkedList(DoublyLinkedList *list, const void *element);
int prependDoublyLinkedList(DoublyLinkedList *list, const void *element);
int rotateTailWithNewData(DoublyLinkedList *list, const void *element);
int deleteElementDoublyLinkedList(DoublyLinkedList *list, int pos);

typedef int (*match_method)(const void *listValue, const void *compareValue);
int matchElementDoublyLinkedList(DoublyLinkedList *list, match_method match,
                                 const void *compareValue, int *pos, void **listValue);
int getElementDoublyLinkedList(DoublyLinkedList *list, int pos, void **element);
void printDoublyLinkedList(DoublyLinkedList *list);
int dobulyLinkedListTest(void);

#endif // DOUBLY_LINKEDLIST_H