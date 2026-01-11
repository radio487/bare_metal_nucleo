/*
 * Startup code for the STM32L476RG Nucleo board.
 * Originally it was meant to be based on the vector.c file of
 * the libopencm3 project but it ended up being written from scratch.
 */


/*
 * The various assembler directives can be found in the GNU
 * assembler manual (Section 9.4):
 * https://www.sourceware.org/binutils/docs/as/index.html
 */

/*
 * The cortex M4 processor only runs in thumb mode (thumb and thumb2).
 * For whatever historical reason, some mnemonics take slightly
 * different form for thumb and ARM instruction sets, so the following
 * assembler directive allows us to use either mnemonic, and be sure
 * that we actually get the correct instruction. 
 */
.syntax unified
/*
 * The architecture of Cortex-M4 is armv7E-M, which can be checked
 * in ARM's Cortex-M4 datasheet, first page.
 */
.arch armv7e-m
/* We specify the processor */
.cpu cortex-m4
/*
 * Floating point peripheral of the Cortex-M4 processor.
 * It is specified in section 4.6 of the Cortex-M4 programmer's
 * manual by ST. It is also mentioned in the datasheet (Section 2,
 * first paragraph). sp stands for single precission.
 * From the Cortex-M4 user manual from ST, section 4.6, I am
 * guessing d16 refers to the 32 single precission registers
 * of the FPU itself, which can be also accesed as 16 doubleword
 * registers.
 */
.fpu fpv4-sp-d16
/*
 * Cortex-M4 only uses thumb and thumb2 instructions, as the
 * ARM Cortex-M4 datasheet says on its first page. Recall that
 * thumb instructions are 16 bit (hence producing very tight
 * binaries), and thumb2 adds some 32 bit instructions for
 * extra complexity.
 */
.thumb
/*
 * The .cpu, .fpu and .thumb directives are repeated again in
 * the flags passed to the assembler by the Makefile. Still,
 * they improve readability so I keep them here.
 */

/*
 * extern directives for symbols defined in separate files.
 * This tells the assembler to look for these symbols somewhere
 * else. All defined in the linker script except main and
 * the Interrupt Request Handlers.
 */
.extern _stack_pointer
.extern _beg_data_ram
.extern _end_data_ram
.extern _beg_data_flash
.extern _beg_bss_ram
.extern _end_bss_ram
.extern main
/* Needs refactoring. Interrupt symbols
.extern TIM2_IRQHandler */

/*
 * I make the symbol vector_table global because I have forced
 * the linker script to look for this symbol with
 * EXTERN(vector_table). If I did not have this line in the
 * linker script, I may get some weird bug where the linker
 * omits the vector table. This is worthy of a test, so at
 * some point comment out EXTERN(vector_table) in the linker
 * script and see what happens. Also, as an added benefit,
 * if I have this symbol I can see it from gdb.
 */
.global vector_table
.global reset_handler
/*
 * It is essential to specify the .type directive on all
 * interrup handlers for otherwise we do not get the
 * thumb bit set and we get errors 
 */
.type reset_handler, %function
.global systick_handler
.type systick_handler, %function

/*
 * ====================================
 * Vector Table
 * ====================================
 *
 * This is the first part of the .text section as specified
 * in the linker script.
 * The "a" is a section flag. It stands for allocatable which means
 * that we need it to allocate space for it in the binary.
 * The options are "a" allocatable, "x" executable and "w" writable.
 * .vectors is not meant to be executable code as it is essentially
 * an array of pointers, so no "x", and it is not meant to be changed
 * so no "w".
 * .section .text, .section .bss and .section .data are special 
 * in the sense that the assembler implicitly knows the correct flags
 * so they can be omitted. I will write them for consistency and
 * documentation purposes. 
 * Apparently one can also find:
 * .section .vectors, "a", %progbits
 * but for now I omit the %progbits as I do not seem to need it
 */
.section .vectors, "a"
vector_table:
  /* Cortex M4 Interrupts */
  .word _stack_pointer   /* Stack pointer */
  .word reset_handler    /* Reset handler */
  .word loop_forever     /* NMI */
  .word loop_forever     /* HardFault */
  .word loop_forever     /* MemMan */
  .word loop_forever     /* BusFault */
  .word loop_forever     /* UsageFault */
  .word 0                /* Reserved */
  .word 0                /* Reserved */
  .word 0                /* Reserved */
  .word 0                /* Reserved */
  .word loop_forever     /* SuperVisor Call */
  .word loop_forever     /* Debug */
  .word 0                /* Reserved */
  .word loop_forever     /* PendSV */
  .word systick_handler  /* SysTick */
  /* External Interrupts, Those of STM32. We got 83 total */
  .rept 28
    .word loop_forever
  .endr
  .word loop_forever /* TIM2_IRQHandler /* TIM2 global interrupt */
  .rept 23
    .word loop_forever
  .endr
  .word loop_forever     /* UART4 global interrupt */
  .rept 26
    .word loop_forever
  .endr
  .word 0                /* Missing in my micro */
  .word loop_forever     /* RNG interrupt */
  .word loop_forever     /* Floating point interrupt */
  .word loop_forever     /* CRS interrupt */


/*
 * ====================================
 * Executable Code
 * ====================================
 */

/* allocatable executable. These flags are implicit but I keep them */
.section .text, "ax"
/*
 * This symbol is the entry point. We have ENTRY(reset_handler)
 * in the linker script. The cpu always starts execution here after
 * a reset
 */
reset_handler:
/* nop is added because for some reason gdb seems to
 * take one instruction to load register values. */
  nop
/*
 * The FPU is disabled by default. I want to be able to 
 * have floats in my code so I enable it. This activation code
 * is actually extracted from ST's Cortex-M4 programmer's manual
 * section 4.6.6 which is about enabling the FPU.
 */
activate_fpu:
  /* CPACR is located at address 0xE000ED88 */
  /* CPACR is the Coprocessor Access Control Register */
  /* Study this ldr.w instruction at some point */
  ldr.w   r0, =0xE000ED88
  /* Read CPACR */
  ldr     r1, [r0]
  /* Set bits 20-23 to enable CP10 and CP11 coprocessors */
  orr     r1, r1, #(0xF << 20)
  /* Write back the modified value to the CPACR */
  str     r1, [r0]/* wait for store to complete */
  dsb
  /* reset pipeline now the FPU is enabled */
  isb

  /* Copy .data from FLASH to RAM before calling main() */
  ldr r0, =_beg_data_flash
  ldr r1, =_beg_data_ram
  ldr r2, =_end_data_ram
data_flash_to_ram:
  cmp r1, r2
  ittt lt
  ldrlt r3, [r0], #4
  strlt r3, [r1], #4
  blt data_flash_to_ram

  /* Zero out .bss section before calling main() */
  ldr r0, =_beg_bss_ram
  ldr r1, =_end_bss_ram
  mov r2, #0
zero_bss_loop:
  cmp r0, r1
  itt lt
  strlt r2, [r0], #4
  blt zero_bss_loop

  /* Call main */
  bl main
  /* If main returns trap the cpu in a loop */
loop_forever:
  b .

/*
 * ====================================
 * Interrupts Implemented in Assembly
 * ====================================
 */

systick_handler:
  /* 
   * This is fine for now but make it atomic eventually.
   * We load the GPIOA_ODR register address to r0.
   */
  ldr r0, =0x48000000
  add r0, r0, #0x14
  /* We load the contents pointed at by r0 in r1 */
  ldr r1, [r0]
  /* This toggles the 5th bit which controls LED2 */
  eor r1, r1, #0x00000020
  str r1, [r0]
  bx lr
