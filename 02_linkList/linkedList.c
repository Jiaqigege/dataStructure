#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 链表节点定义
typedef struct Node {
    void *data;
    struct Node *next;
} Node;

// 链表结构定义
typedef struct LinkedList {
    Node *head;
    int elementSize; // 每个元素的大小
} LinkedList;

// 初始化一个空链表
LinkedList *createLinkedList(int elementSize)
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (!list)
        return NULL;
    list->head = NULL;
    list->elementSize = elementSize;
    return list;
}

// 在链表尾部添加一个新元素
int appendLinkedList(LinkedList *list, void *element)
{
    if (!list || !element)
        return -1;

    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
        return -1;

    newNode->data = malloc(list->elementSize);
    if (!newNode->data) {
        free(newNode);
        return -1;
    }

    memcpy(newNode->data, element, list->elementSize);
    newNode->next = NULL;

    if (!list->head) {
        list->head = newNode;
    } else {
        Node *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }

    return 0;
}

// 释放链表内存
void destroyLinkedList(LinkedList *list)
{
    if (!list)
        return;

    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

// 获取链表的第pos个元素，索引从0开始
void *getLinkedList(LinkedList *list, int pos)
{
    if (!list || pos < 0)
        return NULL;

    Node *current = list->head;
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

// 从链表中删除第pos个元素
int removeLinkedList(LinkedList *list, int pos)
{
    if (!list || pos < 0)
        return -1;

    Node *current = list->head;
    if (pos == 0) {
        list->head = current->next;
        free(current->data);
        free(current);
        return 0;
    }

    int index = 0;
    Node *prev = NULL;
    while (current && index != pos) {
        prev = current;
        current = current->next;
        index++;
    }
    if (!current)
        return -1;

    prev->next = current->next;
    free(current->data);
    free(current);
    return 0;
}

// 迭代法反转链表
void iterReverseLinkedList(LinkedList *list)
{
    if (!list)
        return;

    Node *prev = NULL;
    Node *current = list->head;

    while (current) {
        Node *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    list->head = prev;
}

Node *recursiveLinkedList_(Node *head)
{}

// 递归法反转链表
void recursiveLinkedList(LinkedList *list)
{
    if (!list)
        return;
    list->head = recursiveLinkedList_(list->head);
}

int main()
{
    // 创建一个链表，其中每个元素是一个整数
    LinkedList *list = createLinkedList(sizeof(int));
    if (!list) {
        printf("Failed to create list.\n");
        return -1;
    }

    int a = 5, b = 10, c = 15, d = 20;

    // 添加元素到链表
    appendLinkedList(list, &a);
    appendLinkedList(list, &b);
    appendLinkedList(list, &c);

    // 输出链表元素
    printf("Original list:\n");
    for (int i = 0; i < 3; i++) {
        int *value = (int *)getLinkedList(list, i);
        if (value) {
            printf("%d ", *value);
        }
    }
    printf("\n");

    // 插入元素到链表头部
    appendLinkedList(list, &d);

    // 输出链表元素
    printf("List after adding an element:\n");
    for (int i = 0; i < 4; i++) {
        int *value = (int *)getLinkedList(list, i);
        if (value) {
            printf("%d ", *value);
        }
    }
    printf("\n");

    // 从链表中删除元素
    removeLinkedList(list, 1);

    // 输出链表元素
    printf("List after removing an element:\n");
    for (int i = 0; i < 3; i++) {
        int *value = (int *)getLinkedList(list, i);
        if (value) {
            printf("%d ", *value);
        }
    }
    printf("\n");

    iterReverseLinkedList(list);
    printf("List after reversing:\n");
    for (int i = 0; i < 3; i++) {
        int *value = (int *)getLinkedList(list, i);
        if (value) {
            printf("%d ", *value);
        }
    }
    printf("\n");

    // 释放链表内存
    destroyLinkedList(list);
    return 0;
}