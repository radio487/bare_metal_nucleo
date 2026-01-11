SRC_DIR =../src
LD_DIR = ../ld

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Some of these flags are repeated in startup.s.
# Chech the GNU assembler manual for a list of options:
# https://www.sourceware.org/binutils/docs/as/index.html
# -mfloat-abi=hard makes the cpu to generate instructions for foating point units, and makes sure the floating point registers of the FPU are used. The option soft uses the normal registers of the Cortex-M4 and software simulated floating point instructions, and the softfp uses the better instructions but the standard registers.
# -ffreestanding makes the program meant to be run without an OS. This sets some constraints.
# -nostartfiles makes sure that no startup code given by the compiler is used. crt0.o is the typical name of such an object file that is used in a hosted environment. Maybe I should name my startup.s crt0.s?
# -g generates debug info, which is useful for gdb. The binary grows in size as a consequence
CFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffreestanding -nostartfiles -g -c
ASFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -g
# This forces the use of my custom linker script
LDFLAGS = -T ../ld/linker_script.ld

# Since this is a baremetal program, in principle, we need a binary and not an .elf file which is an executable format with lots of metadata. Nevertheless, we still generate a .elf file. Why? So that we can use gdb to debug. The .elf contains metadata about the program that is used by gdb to assist in debugging. When we run the load command in gdb we only load the proper binary. Thus, I could get away without generating a .bin, but I do so not to lose sight of the actual goal of the proccess. If I wanted, I could use openocd to flash the .bin without gdb, although I would need to provide some info to it.
all: firmware.bin

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

startup.o: $(SRC_DIR)/startup.s
	$(AS) $(ASFLAGS) -o startup.o $(SRC_DIR)/startup.s

main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o main.o $(SRC_DIR)/main.c
	# trivial main function for debugging purposes
	# $(CC) $(CFLAGS) -o main.o $(SRC_DIR)/main_test.c

firmware.elf: startup.o main.o
	$(LD) $(LDFLAGS) -o firmware.elf startup.o main.o

clean:
	rm -f ./*.o ./*.elf ./*.bin
