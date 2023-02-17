/**
 * @file dt.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief
 * @version 0.1.0
 * @date 21-10-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <arch.h>
#include <libk.h>

/**
 * @brief Загрузка регистра GDT
 *
 */
extern void gdt_flush(const void*);

/**
 * @brief Глобальная таблица дескрипторов
 *
 */
gdt_entry_t gdt_entries[5];

/**
 * @brief Структура указателей на GDT
 *
 */
const gdt_ptr_t gdt_ptr = {
    .limit = (sizeof(gdt_entries)) - 1,
    .base = (uint32_t)&gdt_entries,
};

/**
 * @brief Таблица дескрипторов прерываний
 *
 */
idt_entry_t idt_entries[256];

/**
 * @brief Структура указателей размещения IDT
 *
 */
const idt_ptr_t idt_ptr = {
    .limit = sizeof(idt_entries) - 1,
    .base = (uint32_t)&idt_entries,
};

/**
 * @brief Загрузка рагистра IDTR - внешняя ассемблерная функция
 *
 */
extern void idt_flush(const void*);

/**
 * @brief Основной обработчик прерываний
 *
 */
extern void int_common_stub();

static void idt_init();
static void gdt_init();
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

/**
 * @brief Установка значения в таблицу GDT
 *
 * @param num Номер сегмента
 * @param base База сегмента
 * @param limit Лимит сегмента
 * @param access Байт доступа
 * @param gran Байт гранулярности
 */
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Заполняем поля базы */
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    /* Заполняем поля лимита */
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0xF;

    /* Заполняем байты доступа и гранулярности */
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

/**
 * @brief Инициализации глобальной таблицы дескрипторов
 *
 */
void gdt_init()
{
    debug_log("[GDT]");

    /* Нулевой дескриптор */
    gdt_set_gate(0, 0, 0, 0, 0);
    /* Дескриптор кода ядра  (ring 0)*/
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    /* Дескриптор данных ядра (ring 0)*/
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    /* Дескриптор кода пользовательского режима (ring 3)*/
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    /* Дескриптор данных пользовательского режима (ring 3)*/
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    /* Включаем сегментную адресацию  */
    gdt_flush(&gdt_ptr);
}

/**
 * @brief  Установка значения в таблицу IDT
 *
 * @param num Номер прерывания
 * @param base Адрес обработчика
 * @param selector Селектор сегмента ядра
 * @param flags Флаги доступа(0x60 для юзермода)
 */
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;

    idt_entries[num].selector = selector;
    idt_entries[num].allways0 = 0;

    idt_entries[num].flags = flags;
}

/**
 * @brief Инициализация таблицы векторов прерываний
 *
 */
void idt_init()
{
    debug_log("[IDT]");

    memset(&idt_entries, 0, sizeof(idt_entries));

    ports_outb(0x20, PIC1_ICW1);
    ports_outb(0xA0, PIC2_ICW1);

    ports_outb(0x21, PIC1_ICW2);
    ports_outb(0xA1, PIC2_ICW2);

    ports_outb(0x21, PIC1_ICW3);
    ports_outb(0xA1, PIC2_ICW3);

    ports_outb(0x21, PIC1_ICW4);
    ports_outb(0xA1, PIC2_ICW4);

    ports_outb(0x21, 0x00);
    ports_outb(0xA1, 0x00);

    idt_flush(&idt_ptr);
}

/**
 * @brief Инициализация таблиц дескрипторов
 *
 * @return true В случае успешной настройки
 * @return false В случае ошибки
 */
bool dt_init()
{
    gdt_init();
    idt_init();
    int_init();

    return true;
}