TARGET = ../../initrd/apps/basic
CC = clang -target i386-pc-none-elf
CFLAGS = -std=gnu11 -lgcc -mno-sse -mno-avx -O0 -ffreestanding -I ../include/ -c

OBJS = tmp/basic_io.o tmp/lexical.o tmp/syntax.o tmp/main.o

# $(OBJS): tmp/%.o : src/%.c
	# @echo "BUILDING " $<
	# @$(CC) $(CFLAGS) $< -o $@
# 
# $(TARGET): $(OBJS)
	# @echo "ALL OBJECT FILES WERE BUILT!"

all: 
	$(CC) $(CFLAGS) src/basic_io.c -o tmp/basic_io.o
	$(CC) $(CFLAGS) src/lexical.c -o tmp/lexical.o
	$(CC) $(CFLAGS) src/syntax.c -o tmp/syntax.o
	$(CC) $(CFLAGS) src/main.c -o tmp/main.o

synchronize:
	@echo $(OBJS)
