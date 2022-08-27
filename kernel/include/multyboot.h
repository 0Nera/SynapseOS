#pragma once


#include <stdint.h>




#define MBOOT_FLAG_MEM     0x001
#define MBOOT_FLAG_DEVICE  0x002
#define MBOOT_FLAG_CMDLINE 0x004
#define MBOOT_FLAG_MODS    0x008
#define MBOOT_FLAG_AOUT    0x010
#define MBOOT_FLAG_ELF     0x020
#define MBOOT_FLAG_MMAP    0x040
#define MBOOT_FLAG_CONFIG  0x080
#define MBOOT_FLAG_LOADER  0x100
#define MBOOT_FLAG_APM     0x200
#define MBOOT_FLAG_VBE     0x400

struct multiboot_info {
    uint32_t flags; /* Header flags */
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    char* cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint32_t vbe_mode;
    uint32_t vbe_interface_seg;
    uint32_t vbe_interface_off;
    uint32_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;

    union {
        struct {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        };

        struct {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
};

typedef struct multiboot_info multiboot_info;

struct multiboot_memory_map_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
};

typedef struct multiboot_memory_map_entry multiboot_memory_map_entry;

// More full multiboot.h take from DiOS
