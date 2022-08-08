.globl	outbasm
.type	outbasm, @function
outbaasm:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movw	%dx, -4(%ebp)
	movb	%al, -8(%ebp)
	movzbl	-8(%ebp), %eax
	movzwl	-4(%ebp), %edx
	ret
