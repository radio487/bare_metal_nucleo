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
 * Startup code for the bluepill which is meant to handle the vector table.
 * It is based on the vector.c file on the libopencm3 project but I have
 * decided to write an assembly file instead.
 */

// Unified syntax between ARM and thumb modes
.syntax unified
.cpu cortex-m3
// Apparently, Cortex-M3 does not have hardware built for floats
// so it has to emulate it with software
.fpu softvfp
// Cortex-M3 runs on thumb mode
.thumb

/* extern directives. All defined in the linker script except main */
.extern _stack
.extern _beg_data_ram
.extern _end_data_ram
.extern _beg_data_flash
.extern _beg_bss_ram
.extern _end_bss_ram
.extern main

/* Vector Table */
// This is the first part of the .text section as specified
// in the linker script.
.section .vectors, "a"
vector_table:
    .word _stack           /* Initial stack pointer */
    .word reset_handler    /* Reset handler */
    .word loop_forever
    .word loop_forever
    .word loop_forever
    .word loop_forever
    .word loop_forever
    .word 0
    .word 0
    .word 0
    .word 0
    .word loop_forever
    .word loop_forever
    .word 0
    .word loop_forever
    .word loop_forever
    /* External Interrupts (IRQ) */
    .rept 43
        .word loop_forever
    .endr

.section .text
/* Reset handler */
.global reset_handler
.type reset_handler, %function
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
// SysTick handler



