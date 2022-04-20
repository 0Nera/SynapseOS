# Kernel startup asm code FROM EOS

#.macro  sum from=0x00000083, to=0x408C00083
#    .long   \from
#    .if     \to-\from
#    sum     "(\from+0x00400000)",\to
#    .endif
#.endm

# Declare constants for the multiboot header
.set ALIGN,		1<<0				# align loaded modules on page boundaries
.set MEMINFO,	1<<1				# please provide us a memory map
.set VBE_MODE, 1<<2       # VBE mode flag. GRUB will set it for us and provide info about it.
.set FLAGS,		ALIGN | MEMINFO	| VBE_MODE	# multiboot "flag" field
.set MAGIC,		0x1BADB002			# magic number to let the booloader find the header
.set CHECKSUM,	-(MAGIC + FLAGS)	# Checksum of the above

# Declare a multiboot header that marks the program as a kernel.  These
# are magic values that are documented in the multiboot standard.  The
# bootloader will search for this signature in the first 8KiB of the 
# kernel file, aligned at a 32-bit boundary.  The signature is in its
# own section so the header can be forced to be in the first 8 KiB of 
# the kernel file.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM #if you dont need vbe comment longs after this line
.long 0, 0, 0, 0, 0 # unused?
.long 0 # 0 = set graphics mode
.long 1024, 768, 32 # Width, height, depth


# The multiboot standard does not define the value of the stack pointer
# register esp, and it's up to the kernel to provide a stack.  The following
# lines allocate room for a small stack by creating a symbol at the bottom,
# then allocating 16KiB for it, then putting a symbol at the top.  The
# stack will grow downwards.  It's apparently in its own section so it
# can be marked nobits, but there doesn't seem to actually be anything
# that does that.  Odd.  In theory doing that allows us to avoid having
# 16KiB of dead bytes in the binary.
# The stack needs to be 16-byte alsigned for x86 according to the System
# V ABI standard, so we do that here.

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 65536 # 64 KiB for the stack             old: 16384  # 16KiB
stack_top:

# This is the virtual base address of kernel space. It must be used to convert virtual
# addresses into physical addresses until paging is enabled. Note that this is not
# the virtual address where the kernel image itself is loaded -- just the amount that must
# be subtracted from a virtual address to get a physical address.
.set KERNEL_VIRTUAL_BASE, 0xC0000000                  # 3GB
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22)  # Page directory index of kernel's 4MB PTE.

# Declares the boot Paging directory to load a virtual higher half kernel
.section .data
.align 0x1000
.global _boot_page_directory
_boot_page_directory:
    #.long 0x00000083
    #.fill (KERNEL_PAGE_NUMBER - 1), 4, 0x00000000
    #.long 0x00000083
    #.fill (1024 - KERNEL_PAGE_NUMBER - 1), 4, 0x00000000

.long 0x00000083
.long 0x00400083
.long 0x00800083
.long 0x00c00083
.long 0x01000083
.long 0x01400083
.long 0x01800083
.long 0x01c00083
.long 0x02000083
.long 0x02400083
.long 0x02800083
.long 0x02c00083
.long 0x03000083
.long 0x03400083
.long 0x03800083
.long 0x03c00083
.long 0x04000083
.long 0x04400083
.long 0x04800083
.long 0x04c00083
.long 0x05000083
.long 0x05400083
.long 0x05800083
.long 0x05c00083
.long 0x06000083
.long 0x06400083
.long 0x06800083
.long 0x06c00083
.long 0x07000083
.long 0x07400083
.long 0x07800083
.long 0x07c00083
.long 0x08000083
.long 0x08400083
.long 0x08800083
.long 0x08c00083
.long 0x09000083
.long 0x09400083
.long 0x09800083
.long 0x09c00083
.long 0x0a000083
.long 0x0a400083
.long 0x0a800083
.long 0x0ac00083
.long 0x0b000083
.long 0x0b400083
.long 0x0b800083
.long 0x0bc00083
.long 0x0c000083
.long 0x0c400083
.long 0x0c800083
.long 0x0cc00083
.long 0x0d000083
.long 0x0d400083
.long 0x0d800083
.long 0x0dc00083
.long 0x0e000083
.long 0x0e400083
.long 0x0e800083
.long 0x0ec00083
.long 0x0f000083
.long 0x0f400083
.long 0x0f800083
.long 0x0fc00083
.long 0x10000083
.long 0x10400083
.long 0x10800083
.long 0x10c00083
.long 0x11000083
.long 0x11400083
.long 0x11800083
.long 0x11c00083
.long 0x12000083
.long 0x12400083
.long 0x12800083
.long 0x12c00083
.long 0x13000083
.long 0x13400083
.long 0x13800083
.long 0x13c00083
.long 0x14000083
.long 0x14400083
.long 0x14800083
.long 0x14c00083
.long 0x15000083
.long 0x15400083
.long 0x15800083
.long 0x15c00083
.long 0x16000083
.long 0x16400083
.long 0x16800083
.long 0x16c00083
.long 0x17000083
.long 0x17400083
.long 0x17800083
.long 0x17c00083
.long 0x18000083
.long 0x18400083
.long 0x18800083
.long 0x18c00083
.long 0x19000083
.long 0x19400083
.long 0x19800083
.long 0x19c00083
.long 0x1a000083
.long 0x1a400083
.long 0x1a800083
.long 0x1ac00083
.long 0x1b000083
.long 0x1b400083
.long 0x1b800083
.long 0x1bc00083
.long 0x1c000083
.long 0x1c400083
.long 0x1c800083
.long 0x1cc00083
.long 0x1d000083
.long 0x1d400083
.long 0x1d800083
.long 0x1dc00083
.long 0x1e000083
.long 0x1e400083
.long 0x1e800083
.long 0x1ec00083
.long 0x1f000083
.long 0x1f400083
.long 0x1f800083
.long 0x1fc00083
.long 0x20000083
.long 0x20400083
.long 0x20800083
.long 0x20c00083
.long 0x21000083
.long 0x21400083
.long 0x21800083
.long 0x21c00083
.long 0x22000083
.long 0x22400083
.long 0x22800083
.long 0x22c00083
.long 0x23000083
.long 0x23400083
.long 0x23800083
.long 0x23c00083
.long 0x24000083
.long 0x24400083
.long 0x24800083
.long 0x24c00083
.long 0x25000083
.long 0x25400083
.long 0x25800083
.long 0x25c00083
.long 0x26000083
.long 0x26400083
.long 0x26800083
.long 0x26c00083
.long 0x27000083
.long 0x27400083
.long 0x27800083
.long 0x27c00083
.long 0x28000083
.long 0x28400083
.long 0x28800083
.long 0x28c00083
.long 0x29000083
.long 0x29400083
.long 0x29800083
.long 0x29c00083
.long 0x2a000083
.long 0x2a400083
.long 0x2a800083
.long 0x2ac00083
.long 0x2b000083
.long 0x2b400083
.long 0x2b800083
.long 0x2bc00083
.long 0x2c000083
.long 0x2c400083
.long 0x2c800083
.long 0x2cc00083
.long 0x2d000083
.long 0x2d400083
.long 0x2d800083
.long 0x2dc00083
.long 0x2e000083
.long 0x2e400083
.long 0x2e800083
.long 0x2ec00083
.long 0x2f000083
.long 0x2f400083
.long 0x2f800083
.long 0x2fc00083
.long 0x30000083
.long 0x30400083
.long 0x30800083
.long 0x30c00083
.long 0x31000083
.long 0x31400083
.long 0x31800083
.long 0x31c00083
.long 0x32000083
.long 0x32400083
.long 0x32800083
.long 0x32c00083
.long 0x33000083
.long 0x33400083
.long 0x33800083
.long 0x33c00083
.long 0x34000083
.long 0x34400083
.long 0x34800083
.long 0x34c00083
.long 0x35000083
.long 0x35400083
.long 0x35800083
.long 0x35c00083
.long 0x36000083
.long 0x36400083
.long 0x36800083
.long 0x36c00083
.long 0x37000083
.long 0x37400083
.long 0x37800083
.long 0x37c00083
.long 0x38000083
.long 0x38400083
.long 0x38800083
.long 0x38c00083
.long 0x39000083
.long 0x39400083
.long 0x39800083
.long 0x39c00083
.long 0x3a000083
.long 0x3a400083
.long 0x3a800083
.long 0x3ac00083
.long 0x3b000083
.long 0x3b400083
.long 0x3b800083
.long 0x3bc00083
.long 0x3c000083
.long 0x3c400083
.long 0x3c800083
.long 0x3cc00083
.long 0x3d000083
.long 0x3d400083
.long 0x3d800083
.long 0x3dc00083
.long 0x3e000083
.long 0x3e400083
.long 0x3e800083
.long 0x3ec00083
.long 0x3f000083
.long 0x3f400083
.long 0x3f800083
.long 0x3fc00083
.long 0x40000083
.long 0x40400083
.long 0x40800083
.long 0x40c00083
.long 0x41000083
.long 0x41400083
.long 0x41800083
.long 0x41c00083
.long 0x42000083
.long 0x42400083
.long 0x42800083
.long 0x42c00083
.long 0x43000083
.long 0x43400083
.long 0x43800083
.long 0x43c00083
.long 0x44000083
.long 0x44400083
.long 0x44800083
.long 0x44c00083
.long 0x45000083
.long 0x45400083
.long 0x45800083
.long 0x45c00083
.long 0x46000083
.long 0x46400083
.long 0x46800083
.long 0x46c00083
.long 0x47000083
.long 0x47400083
.long 0x47800083
.long 0x47c00083
.long 0x48000083
.long 0x48400083
.long 0x48800083
.long 0x48c00083
.long 0x49000083
.long 0x49400083
.long 0x49800083
.long 0x49c00083
.long 0x4a000083
.long 0x4a400083
.long 0x4a800083
.long 0x4ac00083
.long 0x4b000083
.long 0x4b400083
.long 0x4b800083
.long 0x4bc00083
.long 0x4c000083
.long 0x4c400083
.long 0x4c800083
.long 0x4cc00083
.long 0x4d000083
.long 0x4d400083
.long 0x4d800083
.long 0x4dc00083
.long 0x4e000083
.long 0x4e400083
.long 0x4e800083
.long 0x4ec00083
.long 0x4f000083
.long 0x4f400083
.long 0x4f800083
.long 0x4fc00083
.long 0x50000083
.long 0x50400083
.long 0x50800083
.long 0x50c00083
.long 0x51000083
.long 0x51400083
.long 0x51800083
.long 0x51c00083
.long 0x52000083
.long 0x52400083
.long 0x52800083
.long 0x52c00083
.long 0x53000083
.long 0x53400083
.long 0x53800083
.long 0x53c00083
.long 0x54000083
.long 0x54400083
.long 0x54800083
.long 0x54c00083
.long 0x55000083
.long 0x55400083
.long 0x55800083
.long 0x55c00083
.long 0x56000083
.long 0x56400083
.long 0x56800083
.long 0x56c00083
.long 0x57000083
.long 0x57400083
.long 0x57800083
.long 0x57c00083
.long 0x58000083
.long 0x58400083
.long 0x58800083
.long 0x58c00083
.long 0x59000083
.long 0x59400083
.long 0x59800083
.long 0x59c00083
.long 0x5a000083
.long 0x5a400083
.long 0x5a800083
.long 0x5ac00083
.long 0x5b000083
.long 0x5b400083
.long 0x5b800083
.long 0x5bc00083
.long 0x5c000083
.long 0x5c400083
.long 0x5c800083
.long 0x5cc00083
.long 0x5d000083
.long 0x5d400083
.long 0x5d800083
.long 0x5dc00083
.long 0x5e000083
.long 0x5e400083
.long 0x5e800083
.long 0x5ec00083
.long 0x5f000083
.long 0x5f400083
.long 0x5f800083
.long 0x5fc00083
.long 0x60000083
.long 0x60400083
.long 0x60800083
.long 0x60c00083
.long 0x61000083
.long 0x61400083
.long 0x61800083
.long 0x61c00083
.long 0x62000083
.long 0x62400083
.long 0x62800083
.long 0x62c00083
.long 0x63000083
.long 0x63400083
.long 0x63800083
.long 0x63c00083
.long 0x64000083
.long 0x64400083
.long 0x64800083
.long 0x64c00083
.long 0x65000083
.long 0x65400083
.long 0x65800083
.long 0x65c00083
.long 0x66000083
.long 0x66400083
.long 0x66800083
.long 0x66c00083
.long 0x67000083
.long 0x67400083
.long 0x67800083
.long 0x67c00083
.long 0x68000083
.long 0x68400083
.long 0x68800083
.long 0x68c00083
.long 0x69000083
.long 0x69400083
.long 0x69800083
.long 0x69c00083
.long 0x6a000083
.long 0x6a400083
.long 0x6a800083
.long 0x6ac00083
.long 0x6b000083
.long 0x6b400083
.long 0x6b800083
.long 0x6bc00083
.long 0x6c000083
.long 0x6c400083
.long 0x6c800083
.long 0x6cc00083
.long 0x6d000083
.long 0x6d400083
.long 0x6d800083
.long 0x6dc00083
.long 0x6e000083
.long 0x6e400083
.long 0x6e800083
.long 0x6ec00083
.long 0x6f000083
.long 0x6f400083
.long 0x6f800083
.long 0x6fc00083
.long 0x70000083
.long 0x70400083
.long 0x70800083
.long 0x70c00083
.long 0x71000083
.long 0x71400083
.long 0x71800083
.long 0x71c00083
.long 0x72000083
.long 0x72400083
.long 0x72800083
.long 0x72c00083
.long 0x73000083
.long 0x73400083
.long 0x73800083
.long 0x73c00083
.long 0x74000083
.long 0x74400083
.long 0x74800083
.long 0x74c00083
.long 0x75000083
.long 0x75400083
.long 0x75800083
.long 0x75c00083
.long 0x76000083
.long 0x76400083
.long 0x76800083
.long 0x76c00083
.long 0x77000083
.long 0x77400083
.long 0x77800083
.long 0x77c00083
.long 0x78000083
.long 0x78400083
.long 0x78800083
.long 0x78c00083
.long 0x79000083
.long 0x79400083
.long 0x79800083
.long 0x79c00083
.long 0x7a000083
.long 0x7a400083
.long 0x7a800083
.long 0x7ac00083
.long 0x7b000083
.long 0x7b400083
.long 0x7b800083
.long 0x7bc00083
.long 0x7c000083
.long 0x7c400083
.long 0x7c800083
.long 0x7cc00083
.long 0x7d000083
.long 0x7d400083
.long 0x7d800083
.long 0x7dc00083
.long 0x7e000083
.long 0x7e400083
.long 0x7e800083
.long 0x7ec00083
.long 0x7f000083
.long 0x7f400083
.long 0x7f800083
.long 0x7fc00083
.long 0x80000083
.long 0x80400083
.long 0x80800083
.long 0x80c00083
.long 0x81000083
.long 0x81400083
.long 0x81800083
.long 0x81c00083
.long 0x82000083
.long 0x82400083
.long 0x82800083
.long 0x82c00083
.long 0x83000083
.long 0x83400083
.long 0x83800083
.long 0x83c00083
.long 0x84000083
.long 0x84400083
.long 0x84800083
.long 0x84c00083
.long 0x85000083
.long 0x85400083
.long 0x85800083
.long 0x85c00083
.long 0x86000083
.long 0x86400083
.long 0x86800083
.long 0x86c00083
.long 0x87000083
.long 0x87400083
.long 0x87800083
.long 0x87c00083
.long 0x88000083
.long 0x88400083
.long 0x88800083
.long 0x88c00083
.long 0x89000083
.long 0x89400083
.long 0x89800083
.long 0x89c00083
.long 0x8a000083
.long 0x8a400083
.long 0x8a800083
.long 0x8ac00083
.long 0x8b000083
.long 0x8b400083
.long 0x8b800083
.long 0x8bc00083
.long 0x8c000083
.long 0x8c400083
.long 0x8c800083
.long 0x8cc00083
.long 0x8d000083
.long 0x8d400083
.long 0x8d800083
.long 0x8dc00083
.long 0x8e000083
.long 0x8e400083
.long 0x8e800083
.long 0x8ec00083
.long 0x8f000083
.long 0x8f400083
.long 0x8f800083
.long 0x8fc00083
.long 0x90000083
.long 0x90400083
.long 0x90800083
.long 0x90c00083
.long 0x91000083
.long 0x91400083
.long 0x91800083
.long 0x91c00083
.long 0x92000083
.long 0x92400083
.long 0x92800083
.long 0x92c00083
.long 0x93000083
.long 0x93400083
.long 0x93800083
.long 0x93c00083
.long 0x94000083
.long 0x94400083
.long 0x94800083
.long 0x94c00083
.long 0x95000083
.long 0x95400083
.long 0x95800083
.long 0x95c00083
.long 0x96000083
.long 0x96400083
.long 0x96800083
.long 0x96c00083
.long 0x97000083
.long 0x97400083
.long 0x97800083
.long 0x97c00083
.long 0x98000083
.long 0x98400083
.long 0x98800083
.long 0x98c00083
.long 0x99000083
.long 0x99400083
.long 0x99800083
.long 0x99c00083
.long 0x9a000083
.long 0x9a400083
.long 0x9a800083
.long 0x9ac00083
.long 0x9b000083
.long 0x9b400083
.long 0x9b800083
.long 0x9bc00083
.long 0x9c000083
.long 0x9c400083
.long 0x9c800083
.long 0x9cc00083
.long 0x9d000083
.long 0x9d400083
.long 0x9d800083
.long 0x9dc00083
.long 0x9e000083
.long 0x9e400083
.long 0x9e800083
.long 0x9ec00083
.long 0x9f000083
.long 0x9f400083
.long 0x9f800083
.long 0x9fc00083
.long 0xa0000083
.long 0xa0400083
.long 0xa0800083
.long 0xa0c00083
.long 0xa1000083
.long 0xa1400083
.long 0xa1800083
.long 0xa1c00083
.long 0xa2000083
.long 0xa2400083
.long 0xa2800083
.long 0xa2c00083
.long 0xa3000083
.long 0xa3400083
.long 0xa3800083
.long 0xa3c00083
.long 0xa4000083
.long 0xa4400083
.long 0xa4800083
.long 0xa4c00083
.long 0xa5000083
.long 0xa5400083
.long 0xa5800083
.long 0xa5c00083
.long 0xa6000083
.long 0xa6400083
.long 0xa6800083
.long 0xa6c00083
.long 0xa7000083
.long 0xa7400083
.long 0xa7800083
.long 0xa7c00083
.long 0xa8000083
.long 0xa8400083
.long 0xa8800083
.long 0xa8c00083
.long 0xa9000083
.long 0xa9400083
.long 0xa9800083
.long 0xa9c00083
.long 0xaa000083
.long 0xaa400083
.long 0xaa800083
.long 0xaac00083
.long 0xab000083
.long 0xab400083
.long 0xab800083
.long 0xabc00083
.long 0xac000083
.long 0xac400083
.long 0xac800083
.long 0xacc00083
.long 0xad000083
.long 0xad400083
.long 0xad800083
.long 0xadc00083
.long 0xae000083
.long 0xae400083
.long 0xae800083
.long 0xaec00083
.long 0xaf000083
.long 0xaf400083
.long 0xaf800083
.long 0xafc00083
.long 0xb0000083
.long 0xb0400083
.long 0xb0800083
.long 0xb0c00083
.long 0xb1000083
.long 0xb1400083
.long 0xb1800083
.long 0xb1c00083
.long 0xb2000083
.long 0xb2400083
.long 0xb2800083
.long 0xb2c00083
.long 0xb3000083
.long 0xb3400083
.long 0xb3800083
.long 0xb3c00083
.long 0xb4000083
.long 0xb4400083
.long 0xb4800083
.long 0xb4c00083
.long 0xb5000083
.long 0xb5400083
.long 0xb5800083
.long 0xb5c00083
.long 0xb6000083
.long 0xb6400083
.long 0xb6800083
.long 0xb6c00083
.long 0xb7000083
.long 0xb7400083
.long 0xb7800083
.long 0xb7c00083
.long 0xb8000083
.long 0xb8400083
.long 0xb8800083
.long 0xb8c00083
.long 0xb9000083
.long 0xb9400083
.long 0xb9800083
.long 0xb9c00083
.long 0xba000083
.long 0xba400083
.long 0xba800083
.long 0xbac00083
.long 0xbb000083
.long 0xbb400083
.long 0xbb800083
.long 0xbbc00083
.long 0xbc000083
.long 0xbc400083
.long 0xbc800083
.long 0xbcc00083
.long 0xbd000083
.long 0xbd400083
.long 0xbd800083
.long 0xbdc00083
.long 0xbe000083
.long 0xbe400083
.long 0xbe800083
.long 0xbec00083
.long 0xbf000083
.long 0xbf400083
.long 0xbf800083
.long 0xbfc00083
.long 0x00000083
.long 0xc0400083
.long 0xc0800083
.long 0xc0c00083
.long 0xc1000083
.long 0xc1400083
.long 0xc1800083
.long 0xc1c00083
.long 0xc2000083
.long 0xc2400083
.long 0xc2800083
.long 0xc2c00083
.long 0xc3000083
.long 0xc3400083
.long 0xc3800083
.long 0xc3c00083
.long 0xc4000083
.long 0xc4400083
.long 0xc4800083
.long 0xc4c00083
.long 0xc5000083
.long 0xc5400083
.long 0xc5800083
.long 0xc5c00083
.long 0xc6000083
.long 0xc6400083
.long 0xc6800083
.long 0xc6c00083
.long 0xc7000083
.long 0xc7400083
.long 0xc7800083
.long 0xc7c00083
.long 0xc8000083
.long 0xc8400083
.long 0xc8800083
.long 0xc8c00083
.long 0xc9000083
.long 0xc9400083
.long 0xc9800083
.long 0xc9c00083
.long 0xca000083
.long 0xca400083
.long 0xca800083
.long 0xcac00083
.long 0xcb000083
.long 0xcb400083
.long 0xcb800083
.long 0xcbc00083
.long 0xcc000083
.long 0xcc400083
.long 0xcc800083
.long 0xccc00083
.long 0xcd000083
.long 0xcd400083
.long 0xcd800083
.long 0xcdc00083
.long 0xce000083
.long 0xce400083
.long 0xce800083
.long 0xcec00083
.long 0xcf000083
.long 0xcf400083
.long 0xcf800083
.long 0xcfc00083
.long 0xd0000083
.long 0xd0400083
.long 0xd0800083
.long 0xd0c00083
.long 0xd1000083
.long 0xd1400083
.long 0xd1800083
.long 0xd1c00083
.long 0xd2000083
.long 0xd2400083
.long 0xd2800083
.long 0xd2c00083
.long 0xd3000083
.long 0xd3400083
.long 0xd3800083
.long 0xd3c00083
.long 0xd4000083
.long 0xd4400083
.long 0xd4800083
.long 0xd4c00083
.long 0xd5000083
.long 0xd5400083
.long 0xd5800083
.long 0xd5c00083
.long 0xd6000083
.long 0xd6400083
.long 0xd6800083
.long 0xd6c00083
.long 0xd7000083
.long 0xd7400083
.long 0xd7800083
.long 0xd7c00083
.long 0xd8000083
.long 0xd8400083
.long 0xd8800083
.long 0xd8c00083
.long 0xd9000083
.long 0xd9400083
.long 0xd9800083
.long 0xd9c00083
.long 0xda000083
.long 0xda400083
.long 0xda800083
.long 0xdac00083
.long 0xdb000083
.long 0xdb400083
.long 0xdb800083
.long 0xdbc00083
.long 0xdc000083
.long 0xdc400083
.long 0xdc800083
.long 0xdcc00083
.long 0xdd000083
.long 0xdd400083
.long 0xdd800083
.long 0xddc00083
.long 0xde000083
.long 0xde400083
.long 0xde800083
.long 0xdec00083
.long 0xdf000083
.long 0xdf400083
.long 0xdf800083
.long 0xdfc00083
.long 0xe0000083
.long 0xe0400083
.long 0xe0800083
.long 0xe0c00083
.long 0xe1000083
.long 0xe1400083
.long 0xe1800083
.long 0xe1c00083
.long 0xe2000083
.long 0xe2400083
.long 0xe2800083
.long 0xe2c00083
.long 0xe3000083
.long 0xe3400083
.long 0xe3800083
.long 0xe3c00083
.long 0xe4000083
.long 0xe4400083
.long 0xe4800083
.long 0xe4c00083
.long 0xe5000083
.long 0xe5400083
.long 0xe5800083
.long 0xe5c00083
.long 0xe6000083
.long 0xe6400083
.long 0xe6800083
.long 0xe6c00083
.long 0xe7000083
.long 0xe7400083
.long 0xe7800083
.long 0xe7c00083
.long 0xe8000083
.long 0xe8400083
.long 0xe8800083
.long 0xe8c00083
.long 0xe9000083
.long 0xe9400083
.long 0xe9800083
.long 0xe9c00083
.long 0xea000083
.long 0xea400083
.long 0xea800083
.long 0xeac00083
.long 0xeb000083
.long 0xeb400083
.long 0xeb800083
.long 0xebc00083
.long 0xec000083
.long 0xec400083
.long 0xec800083
.long 0xecc00083
.long 0xed000083
.long 0xed400083
.long 0xed800083
.long 0xedc00083
.long 0xee000083
.long 0xee400083
.long 0xee800083
.long 0xeec00083
.long 0xef000083
.long 0xef400083
.long 0xef800083
.long 0xefc00083
.long 0xf0000083
.long 0xf0400083
.long 0xf0800083
.long 0xf0c00083
.long 0xf1000083
.long 0xf1400083
.long 0xf1800083
.long 0xf1c00083
.long 0xf2000083
.long 0xf2400083
.long 0xf2800083
.long 0xf2c00083
.long 0xf3000083
.long 0xf3400083
.long 0xf3800083
.long 0xf3c00083
.long 0xf4000083
.long 0xf4400083
.long 0xf4800083
.long 0xf4c00083
.long 0xf5000083
.long 0xf5400083
.long 0xf5800083
.long 0xf5c00083
.long 0xf6000083
.long 0xf6400083
.long 0xf6800083
.long 0xf6c00083
.long 0xf7000083
.long 0xf7400083
.long 0xf7800083
.long 0xf7c00083
.long 0xf8000083
.long 0xf8400083
.long 0xf8800083
.long 0xf8c00083
.long 0xf9000083
.long 0xf9400083
.long 0xf9800083
.long 0xf9c00083
.long 0xfa000083
.long 0xfa400083
.long 0xfa800083
.long 0xfac00083
.long 0xfb000083
.long 0xfb400083
.long 0xfb800083
.long 0xfbc00083
.long 0xfc000083
.long 0xfc400083
.long 0xfc800083
.long 0xfcc00083
.long 0xfd000083
.long 0xfd400083
.long 0xfd800083
.long 0xfdc00083
.long 0xfe000083
.long 0xfe400083
.long 0xfe800083
.long 0xfec00083
.long 0xff000083
.long 0xff400083
.long 0xff800083
.long 0xffc00083


    #try to identity map all the memory
    #SUM 0x00000083,0x408C00083
    #.set i, 0x00000083
    #.rept 1024
    #    .long i
    #    .set i, i+0x00400000
    #.endr



    #next two lines of comments is rubbish delete them
    #.fill (1024 - KERNEL_PAGE_NUMBER - 2), 4, 0x00000000
    #.long (_boot_page_directory | 0x00000003) #store the page dir as the last entry in itself (fractal mapping)

#Why 0x00000083
# the first entry identity maps the first 4MB of memory
# All bits are clear except the following:
# bit 7: PS The kernel page is 4MB.
# bit 1: RW The kernel page is read/write.
# bit 0: P  The kernel page is present.
# 0x00000083 in binary this is 10000011


#Text section
.section .text
.global _loader
_loader:
  mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %ecx
  mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %edx
  or $0x00000003, %ecx
  mov %ecx, 0xFFC(%edx) #bpd + 4092 i.e we write address of page_dir|0000003 to last pde

  # Load Page Directory Base Register. Until paging is set up, the code must
  # be position-independent and use physical addresses, not virtual ones
  mov $(_boot_page_directory - KERNEL_VIRTUAL_BASE), %ecx
  mov %ecx, %cr3

  # Set PSE bit in CR4 to enable 4MB pages.
  mov %cr4, %ecx
  or $0x00000010, %ecx
  mov %ecx, %cr4

  # Set PG bit in CR0 to enable paging.
  mov %cr0, %ecx
  or $0x80000000, %ecx
  mov %ecx, %cr0

  # Start fetching instructions in kernel space.
  # Since eip at this point holds the physical address of this command
  # (approximately 0x00100000) we need to do a long jump to the correct
  # virtual address of _start which is approximately 0xC0100000.
  movl $_start, %edx
  jmp *%edx

# The Higher-Half Kernel entry point.
.global _start
.type _start, @function
_start:
	# Unmap the identity-mapped first 4MB of physical address space.
  	# movl $0, (_boot_page_directory) #uncommenting this crashes all, figure why???
  	# invlpg (0)

  	# Enter protection mode
  	mov %cr0, %eax
  	or $1, %al
  	mov %eax, %cr0

	# set up the stack
	mov $stack_top, %esp

	# This is where we would set up the GDT if we were setting one
	# up -- but there's nothing here for now.

	push %ebx # Multiboot structure
	push %eax # Multiboot magic number

	# Now, call our C code.  The stack is untouched and thus 16-byte 
	# aligned, just like it was originally.  So we can safely call a 
	# C function that is expecting the System V ABI.

	call kernel

_looop:
  jmp _looop

	# As we have nothing left to do, we can go into an infinite loop.
	# We disable interrupts (probably unnecessary, but if the kernel
	# was buggy and enabled them then returned, we it's safest to fix 
	# that).
	cli

	# Then we halt and wait for the next interrupt.  Having disabled
	# them, we should never get them -- though there might be a non-
	# maskable one
1:	hlt
	
	# Just in case we get a non-maskable one...
	jmp 1b

# We set the size of the _start symbol -- which presumably means it's
# some kind of object with attributes, not just a simple label -- 
# to the difference between our current point and the symbol's location
.size _start, . - _start
