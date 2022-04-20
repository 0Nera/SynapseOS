
#include <stdbool.h>
#include <stdint.h>

typedef struct __attribute__((packed)) kheap_item { 
    struct kheap_item *next;
    uint32_t size;
    unsigned char used;
} kheap_item;

void kheap_init();

void *kheap_morecore(uint32_t size);
void kheap_free(void *address);
void *kheap_malloc(uint32_t size);

void kheap_print_stat();
void kheap_test();