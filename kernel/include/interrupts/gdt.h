#pragma once


#define GDT_MAX_DESCRIPTORS 6


void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// Установка GDT
void gdt_init();
