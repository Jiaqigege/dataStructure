#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 双链表节点定义
typedef struct DoublyNode {
    void *data;
    struct DoublyNode *prev;
    struct DoublyNode *next;
} DoubleNode;

// 双链表结构定义
typedef struct DoublyLinkedList {
    DoubleNode *head;
    DoubleNode *tail;
    int elementSize;
} DoublyLinkedList;

// 初始化一个空的双链表
DoublyLinkedList *createDoublyLinkedList(int elementSize)
{
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    if (!list)
        return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->elementSize = elementSize;
    return list;
}

// 释放双链表内存
void destroyDoublyLinkedList(DoublyLinkedList *list)
{
    DoubleNode *current = list->head;
    while (current) {
        DoubleNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

// 在双链表尾部添加一个新元素
int appendDoublyLinkedList(DoublyLinkedList *list, void *element)
{
    DoubleNode *newNode = (DoubleNode *)malloc(sizeof(DoubleNode));
    if (!newNode)
        return -1;

    newNode->data = malloc(list->elementSize);
    if (!newNode->data) {
        free(newNode);
        return -1;
    }

    memcpy(newNode->data, element, list->elementSize);
    newNode->prev = list->tail;
    newNode->next = NULL;

    if (!list->head) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }

    return 0;
}

// 获取双链表的第pos个元素，索引从0开始
void *getDoublyLinkedList(DoublyLinkedList *list, int pos)
{
    DoubleNode *current = list->head;
    int index = 0;
    while (current) {
        if (index == pos) {
            return current->data;
        }
        current = current->next;
        index++;
    }
    return NULL;
}

// 从双链表中删除第pos个元素
int removeDoublyLinkedList(DoublyLinkedList *list, int pos)
{
    if (!list->head || pos < 0)
        return -1;

    DoubleNode *current = list->head;
    if (pos == 0) {
        list->head = current->next;
        if (list->head)
            list->head->prev = NULL;
        else
            list->tail = NULL;
        free(current->data);
        free(current);
        return 0;
    }

    int index = 0;
    while (current && index != pos) {
        current = current->next;
        index++;
    }
    if (!current)
        return -1;

    if (current->prev)
        current->prev->next = current->next;
    if (current->next)
        current->next->prev = current->prev;
    if (current == list->tail)
        list->tail = current->prev;
    free(current->data);
    free(current);
    return 0;
}

// 主函数示例
int main()
{
    DoublyLinkedList *list = createDoublyLinkedList(sizeof(int));

    int values[] = {5, 10, 15, 20};
    for (int i = 0; i < 4; i++) {
        appendDoublyLinkedList(list, &values[i]);
    }

    for (int i = 0; i < 4; i++) {
        int *data = (int *)getDoublyLinkedList(list, i);
        if (data) {
            printf("%d\n", *data);
        }
    }

    removeDoublyLinkedList(list, 1);
    printf("After removal:\n");
    for (int i = 0; i < 3; i++) {
        int *data = (int *)getDoublyLinkedList(list, i);
        if (data) {
            printf("%d\n", *data);
        }
    }

    destroyDoublyLinkedList(list);

    return 0;
}
