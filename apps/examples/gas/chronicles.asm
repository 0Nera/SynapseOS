# Это попытка сделать бэкэнд-компилятор Chronicles на Python

# Она генерирует LLVM код который компилируется с помощью программы ...
# ... llc, на выходе появляется ассемблерный файл

# И кстати - llc умеет генерировать кроссплатформеннве программы!
# Если она заработает, это будет просто круто!

# Она просто выводит NDRAEY

# Арен, если ты это видишь, удачи в дальнейшнй разработке ОС!

	.text
	.file	"<stdin>"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	$1095910478, 12(%esp)           # imm = 0x4152444E
											# 0x41  52  44  4E
											#   'A'<'R'<'D'<'N'
	movw	$22853, 16(%esp)                # imm = 0x5945
											# 0x59  45
											#   'Y'<'E'
	movb	$0, 18(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	calll	print_str@PLT
	xorl	%eax, %eax
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	retl
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
