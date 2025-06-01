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
.extern _data
.extern _edata
.extern _data_loadaddr
.extern _bss
.extern _ebss
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
    /* Copy .data from flash (load) to RAM */
    ldr r0, =_data_loadaddr
    ldr r1, =_data
    ldr r2, =_edata
copy_data:
    cmp r1, r2
    ittt lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt copy_data

    /* Zero .bss section */
    ldr r0, =_bss
    ldr r1, =_ebss
    mov r2, #0
clear_bss:
    cmp r0, r1
    it lt
    strlt r2, [r0], #4
    blt clear_bss

    /* Call main */
    bl main
    /* If main returns (it should not happen), loop */
loop_forever:
    b loop_forever
