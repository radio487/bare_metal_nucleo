/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>,
 * Copyright (C) 2012 chrysn <chrysn@fsfe.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Startup code for the STM32 NUCLEO board, which is meant to handle the vector table.
 * It is based on the vector.c file on the libopencm3 project but I have
 * decided to write an assembly file instead.
 */

// Unified syntax between ARM and thumb modes
.syntax unified
.cpu cortex-m4
// Directive for proper floating point handling with the NUCLEO board
.fpu fpv4-sp-d16
// Cortex-M4 runs on thumb2 
.thumb

/* extern directives for symbols defined in a separate file. All defined in the linker script except main and the Interrupt Request Handlers */
.extern _stack_pointer
.extern _beg_data_ram
.extern _end_data_ram
.extern _beg_data_flash
.extern _beg_bss_ram
.extern _end_bss_ram
.extern main
// Interrupt symbols
.extern TIM2_IRQHandler

.global vector_table
.global reset_handler

/* Vector Table */
// This is the first part of the .text section as specified
// in the linker script.
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
    .word SysTick_handler  /* SysTick */
    /* External Interrupts, Those of STM32. We got 83 total */
    .rept 28
      .word loop_forever
    .endr
    .word TIM2_IRQHandler /* TIM2 global interrupt */
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

.section .text

// This gets run after any and all resets
reset_handler:
    nop
    /* Copy .data FLASH to RAM */
    ldr r0, =_beg_data_flash
    ldr r1, =_beg_data_ram
    ldr r2, =_end_data_ram
data_flash_to_ram:
    cmp r1, r2
    ittt lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt data_flash_to_ram

    /* Zero out .bss section */
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
SysTick_handler:
    // This is fine for now but make it atomic eventually
    // We load the GPIOA_ODR register address to r0
    ldr r0, =0x48000000
    add r0, r0, #0x14
    // We load the contents pointed at by r0 in r1
    ldr r1, [r0]
    // This toggles the 5th bit which controls LED2 
    eor r1, r1, #0x00000020
    str r1, [r0]
    bx lr
