#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bestFitMM.h"

#define MEMORY_SIZE (512 * 1024 * 1024)
#define PAGE_SIZE   (512)
static uint8_t memory_pool[MEMORY_SIZE];

void print_hex_data(const unsigned char *buffer, size_t buffer_size, int step, int prioritize_high_bytes, int type)
{
    if (buffer == NULL) {
        printf("buffer == NULL\n");
        return;
    }

    printf("\t\t     \033[33;4mByte:  ");
    if (prioritize_high_bytes) {
        for (int i = step - 1; i >= 0; i--) {
            printf("%02x ", i);
        }
    } else {
        for (int i = 0; i < step; i++) {
            printf("%02x ", i);
        }
    }
    printf("\033[0m\n");

    for (size_t i = 0; i < buffer_size; i += step) {
        printf("\t%08ld %08zx:  ", i / 4, i);

        if (prioritize_high_bytes) {
            // Print data in little-endian order
            for (size_t j = i + step - 1; j >= i && j < buffer_size; j--) {
                if (type == 0) {
                    printf("%02x ", buffer[j]);
                } else if (type == 1) {
                    printf("%c ", buffer[j]);
                }
            }
        } else {
            // Print data in big-endian order
            for (size_t j = i; j < i + step && j < buffer_size; j++) {
                if (type == 0) {
                    printf("%02x ", buffer[j]);
                } else if (type == 1) {
                    printf("%c ", buffer[j]);
                }
            }
        }

        printf("\n");
    }
}

void test_case_1()
{
    printf("Test Case 1: Allocate and free a small block of memory\n");

    MemManager mm;
    init_mem_manager(&mm, MEMORY_SIZE, PAGE_SIZE);
    memory_fragmentation_stats(&mm);

    uint32_t a = mm_malloc(&mm, 100);
    memory_fragmentation_stats(&mm);

    memcpy(memory_pool + a, "Hello, world!", 13);
    print_hex_data(memory_pool + a, 32, 8, 0, 1);

    mm_free(&mm, a);
    memory_fragmentation_stats(&mm);

    destroy_mem_manager(&mm);
}

void test_case_2()
{
    printf("Test Case 2: Allocate and free multiple small blocks of memory\n");

    MemManager mm;
    init_mem_manager(&mm, MEMORY_SIZE, PAGE_SIZE);
    memory_fragmentation_stats(&mm);

    uint32_t a = mm_malloc(&mm, 4097);
    memory_fragmentation_stats(&mm);

    memcpy(memory_pool + a, "Hello, world!", 13);
    print_hex_data(memory_pool + a, 32, 8, 0, 1);

    uint32_t b = mm_malloc(&mm, 200);
    memory_fragmentation_stats(&mm);

    memcpy(memory_pool + b, "test string 1234567890", 22);
    print_hex_data(memory_pool + b, 32, 8, 0, 1);

    mm_free(&mm, a);
    memory_fragmentation_stats(&mm);

    uint32_t c = mm_malloc(&mm, 513);
    memory_fragmentation_stats(&mm);

    memcpy(memory_pool + c, "test string abcdefghijklmnopqrstuvwxyz", 38);
    print_hex_data(memory_pool + c, 40, 8, 0, 1);

    mm_free(&mm, b);
    memory_fragmentation_stats(&mm);

    mm_free(&mm, c);
    memory_fragmentation_stats(&mm);

    destroy_mem_manager(&mm);
}

void test_case_3()
{
    printf("Test Case 3: Allocate and free a large block of memory\n");

    MemManager mm;
    init_mem_manager(&mm, MEMORY_SIZE, PAGE_SIZE);
    memory_fragmentation_stats(&mm);

    uint32_t a = mm_malloc(&mm, 1280 * 720 * 20);
    memory_fragmentation_stats(&mm);

    mm_free(&mm, a);
    memory_fragmentation_stats(&mm);

    destroy_mem_manager(&mm);
}

void test_case_4()
{
    printf("Test Case 4: Allocate and free multiple large blocks of memory\n");

    MemManager mm;
    init_mem_manager(&mm, MEMORY_SIZE, PAGE_SIZE);
    memory_fragmentation_stats(&mm);

    uint32_t a = mm_malloc(&mm, 1280 * 720 * 20);
    uint32_t b = mm_malloc(&mm, 1280 * 720 * 10);
    memory_fragmentation_stats(&mm);

    mm_free(&mm, a);
    memory_fragmentation_stats(&mm);

    uint32_t c = mm_malloc(&mm, 1280 * 720 * 5);
    memory_fragmentation_stats(&mm);

    mm_free(&mm, b);
    memory_fragmentation_stats(&mm);

    mm_free(&mm, c);
    memory_fragmentation_stats(&mm);

    destroy_mem_manager(&mm);
}

int main()
{
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();

    return 0;
}
