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

static Node *createNode(void *element, int elementSize)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = malloc(elementSize);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->data, element, elementSize);
    node->next = NULL;
}

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

// 在链表尾部添加一个新元素
int appendLinkedList(LinkedList *list, void *element)
{
    if (!list || !element)
        return -1;

    Node *newNode = createNode(element, list->elementSize);
    if (newNode == NULL)
        return -1;

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

int insertLinkedList(LinkedList *list, int pos, void *element)
{
    if (!list || pos < 0 || !element)
        return -1;

    if (pos == 0) { // 头插
        Node *newNode = createNode(element, list->elementSize);
        if (newNode == NULL)
            return -1;

        newNode->next = list->head;
        list->head = newNode;

    } else {
        int i = 0;
        Node *current = list->head;
        while (current->next && i != pos - 1) {
            i++;
            current = current->next;
        }

        if (i != pos - 1)
            return -1;

        Node *newNode = createNode(element, list->elementSize);
        if (newNode == NULL)
            return -1;
        newNode->next = current->next;
        current->next = newNode;
    }

    return 0;
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
    if (!list || !list->head || !list->head->next)
        return;

    Node *prev = NULL;
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        current->next = prev;

        // 为下次循环做准备
        prev = current;
        current = next;
    }

    // 循环退出之前最后一个指针交给了prev，current指向NULL
    list->head = prev;
}

Node *recursiveReverseLinkedList_(Node *head)
{
    if (!head->next)
        return head;

    Node *newHead = recursiveReverseLinkedList_(head->next);

    head->next->next = head;
    head->next = NULL;
    return newHead;
}

// 递归法反转链表
void recursiveReverseLinkedList(LinkedList *list)
{
    if (!list || !list->head || !list->head->next)
        return;
    list->head = recursiveReverseLinkedList_(list->head);
}

// 头插法反转链表
void headinsReverseLinkedList(LinkedList *list)
{
    if (!list || !list->head || !list->head->next)
        return;

    Node *newHead = NULL;
    Node *current = list->head;

    while (current) {
        Node *next = current->next;

        current->next = newHead;
        newHead = current;

        current = next;
    }

    list->head = newHead;
}

// 就地逆置法反转链表
void localReverseLinkedList(LinkedList *list)
{}

void printIntLinkedList(LinkedList *list)
{
    if (!list || !list->head)
        return;

    Node *current = list->head;
    while (current) {
        printf("%d ", *(int *)current->data);
        current = current->next;
    }
    printf("\n");
}

int main()
{
    // 创建一个链表，其中每个元素是一个整数
    LinkedList *list = createLinkedList(sizeof(int));
    if (!list) {
        printf("Failed to create list.\n");
        return -1;
    }

    // 添加元素到链表
    int a = 5, b = 10, c = 15, d = 20;
    appendLinkedList(list, &a);
    appendLinkedList(list, &b);
    appendLinkedList(list, &c);
    printf("Original list:\n\t");
    printIntLinkedList(list);

    // 插入元素到链表头部
    appendLinkedList(list, &d);
    printf("List after adding an element:\n\t");
    printIntLinkedList(list);

    // 从链表中删除元素
    removeLinkedList(list, 1);
    printf("List after removing an element:\n\t");
    printIntLinkedList(list);

    iterReverseLinkedList(list);
    printf("List after iter reversing:\n\t");
    printIntLinkedList(list);

    recursiveReverseLinkedList(list);
    printf("List recursive reversing:\n\t");
    printIntLinkedList(list);

    headinsReverseLinkedList(list);
    printf("List head insert reversing:\n\t");
    printIntLinkedList(list);

    int x = 100, y = 101, z = 102;
    insertLinkedList(list, 0, &x);
    insertLinkedList(list, 1, &y);
    insertLinkedList(list, 5, &z);
    printf("List pos insert:\n\t");
    printIntLinkedList(list);
    // 释放链表内存
    destroyLinkedList(list);
    return 0;
}
