.global enable_paging

enable_paging:
    # Переместить каталог в CR3
    mov 4(%esp), %eax
    #mov (0x00009000), %eax
    mov %eax, %cr3

    # Отключаем 4х мегабайтые страницы
    mov %cr4, %ecx
    and $~0x00000010, %ecx
    mov %ecx, %cr4

    # Включаем пейджинг
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    ret
