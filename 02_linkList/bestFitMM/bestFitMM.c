#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bestFitMM.h"

int init_mem_manager(MemManager *mm, size_t size, size_t page_size)
{
    MemoryBlock *block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    if (!block) {
        printf("ERROR: malloc failed\n");
        return -1;
    }
    block->size = size;
    block->free = 1;
    block->addr = 0;
    block->prev = NULL;
    block->next = NULL;

    mm->baseBlock = block;
    mm->total_size = size;
    mm->page_size = page_size;

    return 0;
}

void destroy_mem_manager(MemManager *mm)
{
    MemoryBlock *block = mm->baseBlock;
    while (block) {
        MemoryBlock *next_block = block->next;
        free(block);
        block = next_block;
    }
}

static MemoryBlock *find_best_fit(MemManager *mm, size_t size)
{
    MemoryBlock *best_fit = NULL;
    size_t min_difference = SIZE_MAX;

    for (MemoryBlock *block = mm->baseBlock; block; block = block->next) {
        if (block->free && block->size >= size) {
            size_t difference = block->size - size;
            if (difference < min_difference) {
                best_fit = block;
                min_difference = difference;

                if (difference == 0)
                    break;
            }
        }
    }

    return best_fit;
}

uint32_t mm_malloc(MemManager *mm, size_t size)
{
    if (size == 0) {
        return (uint32_t)-1;
    }

    if (size % mm->page_size != 0) {
        size = (size / mm->page_size + 1) * mm->page_size;
    }

    MemoryBlock *block = find_best_fit(mm, size);
    if (block) {
        if (block->size > size) {
            MemoryBlock *new_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
            if (!new_block) {
                printf("ERROR: malloc failed\n");
                return (uint32_t)-1;
            }
            new_block->size = block->size - size;
            new_block->free = 1;
            new_block->addr = block->addr + size;
            new_block->prev = block;
            new_block->next = block->next;

            if (block->next) {
                block->next->prev = new_block;
            }
            block->next = new_block;
            block->size = size;
        }

        block->free = 0;
        return block->addr;
    }

    return (uint32_t)-1;
}

static void merge_blocks(MemoryBlock *block)
{
    if (!block) {
        return;
    }

    while (block->prev && block->prev->free) {
        MemoryBlock *prev_block = block->prev;
        prev_block->size += block->size;
        prev_block->next = block->next;
        if (block->next) {
            block->next->prev = prev_block;
        }
        free(block);
        block = prev_block;
    }

    while (block->next && block->next->free) {
        MemoryBlock *next_block = block->next;
        block->size += next_block->size;
        block->next = next_block->next;
        if (next_block->next) {
            next_block->next->prev = block;
        }
        free(next_block);
    }
}

void mm_free(MemManager *mm, uint32_t addr)
{
    if (addr < 0 || addr > mm->total_size) {
        return;
    }

    MemoryBlock *block = mm->baseBlock;

    while (block && block->addr != addr) {
        block = block->next;
    }

    if (block) {
        block->free = 1;
        merge_blocks(block);
    } else {
        printf("ERROR: block not found\n");
    }
}

void memory_fragmentation_stats(MemManager *mm)
{
    int used_blocks = 0;
    size_t used_space = 0;
    int free_blocks = 0;
    size_t free_space = 0;

    for (MemoryBlock *block = mm->baseBlock; block; block = block->next) {
        if (block->free) {
            free_blocks++;
            free_space += block->size;
        } else {
            used_blocks++;
            used_space += block->size;
        }
    }
    printf("\tresult:\n");
    printf("\t\tused blocks: %d, ", used_blocks);
    printf("used space: %zu bytes, %f MB\n", used_space, (float)used_space / 1024 / 1024);
    printf("\t\tfree blocks: %d, ", free_blocks);
    printf("free space: %zu bytes, %f MB\n", free_space, (float)free_space / 1024 / 1024);
    if (used_space + free_space != mm->total_size) {
        printf("ERROR: used_space+free_space != MEMORY_SIZE\n");
    }
}
