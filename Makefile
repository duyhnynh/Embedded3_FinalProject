#--------------------------------------Makefile-------------------------------------
BUILD_DIR = ./build
SRC_DIR = ./src
KERNEL_DIR = ./kernel

CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib
LDFLAGS = -nostdlib 

all: clean kernel8.img run

$(BUILD_DIR)/boot.o: $(KERNEL_DIR)/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@
	
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(KERNEL_DIR)/main.c
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/entry_irq.o: $(KERNEL_DIR)/entry.S $(KERNEL_DIR)/interrupt.S
	aarch64-none-elf-gcc $(CFLAGS) -c $< -o $@

kernel8.img: $(BUILD_DIR)/boot.o $(OFILES) $(BUILD_DIR)/main.o
	aarch64-none-elf-ld $(LDFLAGS) $(BUILD_DIR)/boot.o $(BUILD_DIR)/main.o $(OFILES) -T $(KERNEL_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	aarch64-none-elf-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img

clean:
# For MacOS
	rm -rf .o *.img $(BUILD_DIR)/.o $(BUILD_DIR)/*.img $(BUILD_DIR)/*.elf
# For Linux
#	rm -f .o *.img $(BUILD_DIR)/.o $(BUILD_DIR)/*.img $(BUILD_DIR)/*.elf

# #For Windows
#	del .img .\build\kernel8.elf .\build\.o


# Run emulation with QEMU
run:
#	For MacOS
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio
# For Windows
#	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

# music:
# 	cmd /C start Mercury.mp3