# Она просто выводит NDRAEY

# Это попытка сделать бэкэнд-компилятор Chronicles на Python

# Она генерирует LLVM код который компилируется с помощью программы ...
# ... llc, на выходе появляется ассемблерный файл

# И кстати - llc умеет генерировать кроссплатформенные программы!
# Если она заработает, это будет просто круто!

# Арен, если ты читаешь это, удачи в дальнейшей разработке ОС!

	.text
	.file	"<stdin>"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	subl	$108, %esp
	.cfi_def_cfa_offset 112
	movl	$1819043144, 12(%esp)           # imm = 0x6C6C6548
	movl	$1092627567, 16(%esp)           # imm = 0x41202C6F
	movl	$745432434, 20(%esp)            # imm = 0x2C6E6572
	movl	$1802063392, 24(%esp)           # imm = 0x6B694E20
	movl	$543257705, 28(%esp)            # imm = 0x20617469
	movl	$543452769, 32(%esp)            # imm = 0x20646E61
	movl	$1701344367, 36(%esp)           # imm = 0x6568746F
	movl	$539063154, 40(%esp)            # imm = 0x20217372
	movl	$1931965513, 44(%esp)           # imm = 0x73277449
	movl	$1881170208, 48(%esp)           # imm = 0x70206120
	movl	$1919381362, 52(%esp)           # imm = 0x72676F72
	movl	$1998613857, 56(%esp)           # imm = 0x77206D61
	movl	$1953786226, 60(%esp)           # imm = 0x74746972
	movl	$1998614117, 64(%esp)           # imm = 0x77206E65
	movl	$543716457, 68(%esp)            # imm = 0x20687469
	movl	$1749229665, 72(%esp)           # imm = 0x68432061
	movl	$1768845170, 76(%esp)           # imm = 0x696E6F72
	movl	$1936026723, 80(%esp)           # imm = 0x73656C63
	movl	$1836016416, 84(%esp)           # imm = 0x6D6F6320
	movl	$1701603696, 88(%esp)           # imm = 0x656C6970
	movw	$114, 92(%esp)
	leal	12(%esp), %eax
	movl	%eax, (%esp)
	calll	print_str@PLT
	xorl	%eax, %eax
	addl	$108, %esp
	.cfi_def_cfa_offset 4
	retl
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.ident	"Chronicles compiler v0.0.1 by NDRAEY"
	.section	".note.GNU-stack","",@progbits
