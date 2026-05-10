CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld

CFLAGS = -ffreestanding -nostdlib -Wall -Wextra
LDFLAGS = -T hypervisor.ld

OBJS = src/boot/entry.o src/core/main.o src/core/uart.o \
       src/core/hypervisor.o src/arch/arm64/exceptions.o \
       src/core/cpu_info.o src/core/memory_info.o \
       src/core/vm.o src/core/hypercall.o \
       src/guest/entry.o src/guest/main.o src/guest/scheduler.o \
       src/guest/hypercalls.o src/guest/tasks.o

all: hypervisor.bin

hypervisor.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o hypervisor.elf
	aarch64-linux-gnu-objcopy -O binary hypervisor.elf hypervisor.bin

src/boot/entry.o: src/boot/entry.S
	$(AS) -c src/boot/entry.S -o src/boot/entry.o

src/core/main.o: src/core/main.c
	$(CC) $(CFLAGS) -c src/core/main.c -o src/core/main.o

src/core/uart.o: src/core/uart.c
	$(CC) $(CFLAGS) -Iinclude -c src/core/uart.c -o src/core/uart.o

src/core/hypervisor.o: src/core/hypervisor.c
	$(CC) $(CFLAGS) -Iinclude -c src/core/hypervisor.c -o src/core/hypervisor.o

src/arch/arm64/exceptions.o: src/arch/arm64/exceptions.S
	$(AS) -c src/arch/arm64/exceptions.S -o src/arch/arm64/exceptions.o

src/core/cpu_info.o: src/core/cpu_info.c
	$(CC) $(CFLAGS) -Iinclude -c src/core/cpu_info.c -o src/core/cpu_info.o

src/core/memory_info.o: src/core/memory_info.c
	$(CC) $(CFLAGS) -Iinclude -c src/core/memory_info.c -o src/core/memory_info.o

src/core/vm.o: src/core/vm.c
	$(CC) $(CFLAGS) -Iinclude -c src/core/vm.c -o src/core/vm.o

src/core/hypercall.o: src/core/hypercall.c
	$(CC) $(CFLAGS) -Iinclude -c src/core/hypercall.c -o src/core/hypercall.o

src/core/guest.o: src/core/guest.S
	$(AS) -c src/core/guest.S -o src/core/guest.o

src/guest/entry.o: src/guest/entry.S
	$(AS) -c src/guest/entry.S -o src/guest/entry.o

src/guest/main.o: src/guest/main.c
	$(CC) $(CFLAGS) -Iinclude -c src/guest/main.c -o src/guest/main.o

src/guest/scheduler.o: src/guest/scheduler.c
	$(CC) $(CFLAGS) -Iinclude -c src/guest/scheduler.c -o src/guest/scheduler.o

src/guest/hypercalls.o: src/guest/hypercalls.c
	$(CC) $(CFLAGS) -Iinclude -c src/guest/hypercalls.c -o src/guest/hypercalls.o

src/guest/tasks.o: src/guest/tasks.c
	$(CC) $(CFLAGS) -Iinclude -c src/guest/tasks.c -o src/guest/tasks.o

clean:
	rm -f src/boot/*.o src/core/*.o hypervisor.elf hypervisor.bin src/guest/*.o

run:
	qemu-system-aarch64 \
		-machine virt,virtualization=on \
		-cpu cortex-a57 \
		-nographic \
		-kernel hypervisor.bin \
		-m 128M
