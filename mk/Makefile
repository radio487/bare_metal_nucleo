SRC_DIR =../src
LD_DIR = ../ld

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# CFLAGS = -mcpu=cortex-m3 -mthumb -ffreestanding -nostartfiles -g -c
CFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffreestanding -nostartfiles -g -c
ASFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -g
LDFLAGS = -T ../ld/linker_script.ld

all: firmware.bin

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

startup.o: $(SRC_DIR)/startup.s
	$(AS) $(ASFLAGS) -o startup.o $(SRC_DIR)/startup.s

main.o: $(SRC_DIR)/main.c
# main.o: $(SRC_DIR)/addition.c
	$(CC) $(CFLAGS) -o main.o $(SRC_DIR)/main.c
	# $(CC) $(CFLAGS) -o main.o $(SRC_DIR)/addition.c

firmware.elf: startup.o main.o
	$(LD) $(LDFLAGS) -o firmware.elf startup.o main.o

clean:
	rm -f ./*.o ./*.elf ./*.bin
