/**
 * @file boot.s
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief Подзагрузчик ядра
 * @version 0.1.0
 * @date 18-10-2022
 * 
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License. 
 * @copyright Арен Елчинян (c) 2022 - 2023
 * 
 */


# Использованы материалы
# https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

#include <pmm.h>

# 32х битный код
.code32

# Размер стека
# 4096 * 16 * 8 = 524288 байт. 
# 524288 байт = 512 килобайт
# 4 * 16 = 64 килобайт. 
.set STACK_SIZE, 4096 * 16

# Multiboot флаги
# Multiboot константы
.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, (1 << 0) | (1 << 1) | (1<<2)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

# Объявляем мультизагрузочный заголовок, который помечает программу как ядро.
# Это магические значения, которые задокументированы в стандарте мультизагрузки.
# Загрузчик будет искать этот заголовок в первых 8 килобайтах файла ядра, выровненного по 32-битной границе.
# Подпись находится в отдельном разделе, поэтому заголовок можно принудительно разместить в первых 8 килобайтах файла ядра.
.section .multiboot
multiboot_start:
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM
    .long 0, 0, 0, 0, 0
    .long 0
    .long 1024, 768, 32
multiboot_end:
 
.section .bss
    .align 16
    stack_bottom:
        .skip STACK_SIZE 
    stack_top:

.section    .text
.global _start


# Входная точка
_start:
    cli

    mov  $stack_top, %esp


    push    %esp    # Стек
    push    %ebx    # Структура multiboot
    push    %eax    # Магическое число
    
    call    kernel_startup
    

# Останавливаем процессор    
    cli
__halt_me:
    hlt
    jmp    __halt_me

.comm _stack, STACK_SIZE
