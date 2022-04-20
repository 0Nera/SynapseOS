#include <kernel.h>


bool kheap_is_init; // Was kheap manager initialised or not. TODO: is it unused?

//virtual_addr kheap_last_alloc_addr; // Address of last allocation
virtual_addr kheap_begin; // Address where kheap begins
virtual_addr kheap_end; // Address where kheap ends

uint32_t kheap_memory_used; // How many memory was used
int kheap_allocs_num; // How many allocations now

void kheap_init() {
    kheap_begin = KHEAP_START_VADDR;
    kheap_end = NULL;

    kheap_allocs_num = 0;
    kheap_memory_used = 0;

    //tty_printf("Kernel heap manager initialised!\n");
}

// Increase kernel heap by some amount, this will be rounded up by the page size 
void *kheap_morecore(uint32_t size) {
    // Calculate how many pages we will need
    int pages = (size / PAGE_SIZE) + 1;
    // When kheap_end == NULL we must create the initial heap
    if (kheap_end == NULL) {
        kheap_end = kheap_begin;
    }

    // Set the address to return
    void *prev_kheap_end = kheap_end;
    // Create the pages
    for (; pages-- > 0; kheap_end += PAGE_SIZE) {
        vmm_alloc_page(kheap_end);
        memset(kheap_end, 0x00, PAGE_SIZE);
    }

    // Return the start address of the memory we allocated to the heap
    //tty_printf("(prev_kheap_end) = %x\n", prev_kheap_end);
    return prev_kheap_end;
}

// Free a previously allocated item from the kernel heap
void kheap_free(void *address) {
    kheap_item *tmp_item, *item;
    // Sanity check
    if (address == NULL) {
        return;
    }

    // Set the item to remove
    item = (kheap_item*) ((uint32_t) address - (uint32_t) sizeof(kheap_item));
    // Find it

    for (tmp_item = kheap_begin; tmp_item != NULL; tmp_item = tmp_item->next) {
        //tty_printf("tmp_item = %x\n", tmp_item);
        if (tmp_item == item) {
            // Free it
            tmp_item->used = 0;
            kheap_memory_used -= tmp_item->size;
            kheap_allocs_num--;

            // Coalesce any adjacent free items
            for (tmp_item = kheap_begin; tmp_item != NULL; tmp_item = tmp_item->next) {
                while (!tmp_item->used && tmp_item->next != NULL && !tmp_item->next->used) {
                    tmp_item->size += sizeof(kheap_item) + tmp_item->next->size;
                    tmp_item->next = tmp_item->next->next;
                }
            }

            // Break and return as we are finished
            break;
        }
    }
}

// Allocates an arbiturary size of memory (via first fit) from the kernel heap
void *kheap_malloc(uint32_t size) {
    kheap_item *new_item = NULL, *tmp_item;
    uint32_t total_size;
    // Sanity check
    if (size == 0) {
        return NULL;
    }

    // Round up by 8 bytes and add header size
    total_size = ((size + 7) & ~7) + sizeof(kheap_item);

    kheap_item *last_item;
    // If the heap exists
    if (kheap_end != NULL) {
        // Search for first fit
        for (new_item = kheap_begin; new_item != NULL; new_item = new_item->next) {
            if (new_item->next == NULL) {
                last_item = new_item;
            }
            if (!new_item->used && (total_size <= new_item->size)) {
                break;
            }
        }
    }

    // If we found one
    if (new_item != NULL) {
        tmp_item = (kheap_item*) ((uint32_t) new_item + total_size);
        tmp_item->size = new_item->size - total_size;
        tmp_item->used = 0;
        tmp_item->next = new_item->next;
    } else {
        // Didn't find a fit so we must increase the heap to fit
        new_item = kheap_morecore(total_size);
        if (new_item == NULL) {
            // Return NULL as we are out of physical memory!
            return NULL;
        }

        // Create an empty item for the extra space kheap_morecore() gave us
        // We can calculate the size because morecore() allocates space that is page aligned
        tmp_item = (kheap_item*) ((uint32_t) new_item + total_size);
        tmp_item->size = PAGE_SIZE - (total_size % PAGE_SIZE 
                                      ? total_size % PAGE_SIZE 
                                      : total_size) - sizeof(kheap_item);
        tmp_item->used = 0;
        tmp_item->next = NULL;

        //tty_printf("last_item = %x", last_item); // Why commenting this causes exception??? ANSWER IS BECAUSE OF FUCKING OPTIMIZATION -O1. i disabled it and it works now witout this line
        last_item->next = new_item;
    }
    // !!! A KAK etot new_item svyazan c posledney item v spiske???? to est new_item eto ne poslednyaa item. solved!
    // Create the new item
    new_item->size = size;
    new_item->used = 1;
    new_item->next = tmp_item;

    kheap_allocs_num++;
    kheap_memory_used += total_size;

    // Return the newly allocated memory location
    return (void*) ((uint32_t) new_item + (uint32_t) sizeof(kheap_item));
}

void kheap_print_stat() {
    tty_printf("\nallocs number = %d", kheap_allocs_num);
    tty_printf("\nmemory used = %d bytes\n", kheap_memory_used);
}

void kheap_test() {
    uint32_t sz = 1024 * 768 * 4;
    uint8_t *mas = kheap_malloc(sz);
    //mas[0x003FFFFF] = 17;

    memset(mas, 5, sz);
    tty_printf("mas[sz-1] = %d\n", mas[sz - 1]);
    tty_printf("mas_addr = %x\n", mas);

    kheap_print_stat();

    int cnt = 12;
    int *arr = (int*) kheap_malloc(cnt * sizeof(int));

    int i;
    for (i = 0; i < cnt; i++) {
        arr[i] = i * 2;
    }

    //tty_printf("\n");

    /*
    for (i = 0; i < cnt; i++) {
        tty_printf("%d ", arr[i]);
    }
    */

    tty_printf("arr = %x", arr);
    kheap_print_stat();

    //kheap_free(arr);
    //tty_printf("\narr[0] = %d ", arr[1]);
    //kheap_print_stat();

    int *arr2 = (int*) kheap_malloc(24 * sizeof(int));
    for (i = 0; i < 24; i++) {
        arr2[i] = i * 3;
    }

    tty_printf("\n");

    /*
    for (i = 0; i < 24; i++) {
        tty_printf("%d ", arr2[i]);
    }
    */

    tty_printf("arr2 = %x", arr2);
    kheap_print_stat();
    kheap_free(arr2);
    kheap_print_stat();

    char *arr3 = (char*) kheap_malloc(5 * sizeof(char));
    tty_printf("arr3 = %x", arr3);

    int *arr4 = (int*) kheap_malloc(8200 * sizeof(int));
    for (i = 0; i < 8200; i++) {
        arr4[i] = i * 2;
    }

    tty_printf("\n");

    /*
    for (i = 0; i < 8200; i++) {
        tty_printf("%d ", arr4[i]);
    }
    */

    tty_printf("(arr4) = %x\n", arr4);
    tty_printf("(arr4-hdr) = %x   kheap_end = %x\n", (uint32_t) arr4 - (uint32_t) sizeof(kheap_item), kheap_end); // If without uint32_t will be icorrect result

    kheap_print_stat();
    kheap_free(arr4);
    kheap_print_stat();
}
