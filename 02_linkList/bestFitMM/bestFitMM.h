#ifndef BEST_FIT_MEMORY_MANAGER_H
#define BEST_FIT_MEMORY_MANAGER_H

#include <stddef.h>
#include <stdint.h>

typedef struct MemoryBlock {
    size_t size;
    int free;
    uint32_t addr;
    struct MemoryBlock *prev;
    struct MemoryBlock *next;
} MemoryBlock;

typedef struct MemManager {
    MemoryBlock *baseBlock;
    size_t total_size;
    size_t page_size;
} MemManager;

int init_mem_manager(MemManager *mm, size_t size, size_t page_size);
void destroy_mem_manager(MemManager *mm);
uint32_t mm_malloc(MemManager *mm, size_t size);
void mm_free(MemManager *mm, uint32_t addr);
void memory_fragmentation_stats(MemManager *mm);

#endif // BEST_FIT_MEMORY_MANAGER_H
