#pragma once

#include <stdint.h>


typedef struct {
    uint8_t  name[16];
    uint8_t  layers[16];
    uint32_t colors[256];
    uint32_t data[512][512];
} __attribute__((packed)) vggi_t;


vggi_t vggi_gen();
void vggi_show(vggi_t vggi);
void vggi_save(const char *file);