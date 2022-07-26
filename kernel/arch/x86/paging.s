.global enable_paging
.global _read_cr0
.global _write_cr0
.global _read_cr3
.global _write_cr3

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


/*
_read_cr0:
	mov eax, cr0
	retn


_write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn


_read_cr3:
	mov eax, cr3
	retn


_write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn
*/