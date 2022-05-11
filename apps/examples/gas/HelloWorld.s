  .data
msg: .string "\nHello, World!"
  .text
  .global main
main:
  mov $msg, %ebx
  mov (%ebx), %edi
  .extern printf
  call printf
  ret
