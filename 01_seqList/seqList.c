#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义顺序表结构体
typedef struct {
    void *data;      // 使用void指针来存储任意类型的数据
    int size;        // 当前元素数量
    int capacity;    // 总容量
    int elementSize; // 单个元素的大小，用于分配内存和复制操作
} SeqList;

// 创建顺序表
SeqList *createSeqList(int initCapacity, int elementSize)
{
    SeqList *list = (SeqList *)malloc(sizeof(SeqList));
    if (list == NULL) {
        return NULL;
    }
    list->data = malloc(initCapacity * elementSize);
    if (list->data == NULL) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = initCapacity;
    list->elementSize = elementSize;
    return list;
}

// 销毁顺序表
void destroySeqList(SeqList *list)
{
    if (list) {
        if (list->data) {
            free(list->data);
        }
        free(list);
    }
}

// 预留容量
int reserveSeqList(SeqList *list, int newCapacity)
{
    if (newCapacity <= list->capacity) {
        return 0; // 不减小容量
    }
    void *newData = realloc(list->data, newCapacity * list->elementSize);
    if (newData == NULL) {
        return -1;
    }
    list->data = newData;
    list->capacity = newCapacity;
    return 0;
}

// 增
int insertSeqList(SeqList *list, int pos, void *element)
{
    if (pos < 0 || pos > list->size) {
        return -2; // 位置无效
    }

    if (list->size == list->capacity) {
        if (reserveSeqList(list, list->capacity * 2) == -1) {
            return -1; // 内存分配失败
        }
    }

    if (pos != list->size) { // 不是在末尾插入
        memmove((char *)list->data + (pos + 1) * list->elementSize,
                (char *)list->data + pos * list->elementSize,
                (list->size - pos) * list->elementSize);
    }

    memcpy((char *)list->data + pos * list->elementSize, element, list->elementSize);
    list->size++;
    return 0; // 成功
}

// 删
int removeSeqList(SeqList *list, int pos)
{
    if (pos < 0 || pos >= list->size) {
        return -1; // 位置无效
    }
    if (pos < list->size - 1) { // 如果不是最后一个元素
        memmove((char *)list->data + pos * list->elementSize,
                (char *)list->data + (pos + 1) * list->elementSize,
                (list->size - pos - 1) * list->elementSize);
    }
    list->size--;
    return 0; // 成功
}

// 查
void *getSeqList(SeqList *list, int pos)
{
    if (pos < 0 || pos >= list->size) {
        return NULL; // 位置无效
    }
    return (char *)list->data + pos * list->elementSize;
}

// 改
int modifySeqList(SeqList *list, int pos, void *newElement)
{
    if (pos < 0 || pos >= list->size) {
        return -1; // 位置无效
    }
    memcpy((char *)list->data + pos * list->elementSize, newElement, list->elementSize);
    return 0; // 成功
}

int main()
{
    // 创建一个顺序表，其中每个元素是一个整数
    SeqList *list = createSeqList(10, sizeof(int));
    if (!list) {
        printf("Failed to create list.\n");
        return -1;
    }

    int a = 5, b = 10, c = 15;
    insertSeqList(list, 0, &a);
    insertSeqList(list, 1, &b);
    insertSeqList(list, 2, &c);

    int *result = (int *)getSeqList(list, 1);
    if (result) {
        printf("Element at pos 1: %d\n", *result);
    }

    destroySeqList(list);
    return 0;
}
