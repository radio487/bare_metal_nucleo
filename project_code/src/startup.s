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

.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

/* Vector Table */
.section .vectors, "a"
.type vector_table, %object
.size vector_table, .-vector_table

vector_table:
    .word _stack           /* Initial stack pointer */
    .word reset_handler    /* Reset handler */
    .word nmi_handler
    .word hard_fault_handler
    .word mem_manage_handler
    .word bus_fault_handler
    .word usage_fault_handler
    .word 0                /* Reserved */
    .word 0                /* Reserved */
    .word 0                /* Reserved */
    .word 0                /* Reserved */
    .word sv_call_handler
    .word debug_monitor_handler
    .word 0                /* Reserved */
    .word pend_sv_handler
    .word sys_tick_handler

    /* External Interrupts (IRQ) - fill with 0 or handlers */
    .rept 43
        .word 0
    .endr

/* Handlers - weak aliases */
.weak nmi_handler
.weak hard_fault_handler
.weak mem_manage_handler
.weak bus_fault_handler
.weak usage_fault_handler
.weak sv_call_handler
.weak debug_monitor_handler
.weak pend_sv_handler
.weak sys_tick_handler

.nmi_handler:
    b .
.hard_fault_handler:
    b .
.mem_manage_handler:
    b .
.bus_fault_handler:
    b .
.usage_fault_handler:
    b .
.sv_call_handler:
    b .
.debug_monitor_handler:
    b .
.pend_sv_handler:
    b .
.sys_tick_handler:
    b .

/* External IRQ handlers default */
    .rept 43
        .weak irq_handler
        .set irq_handler, default_handler
    .endr

default_handler:
    b .

/* Reset handler */
.global reset_handler
.type reset_handler, %function
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
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
clear_bss:
    cmp r0, r1
    it lt
    strlt r2, [r0], #4
    blt clear_bss

    /* Call main */
    bl main

    /* If main returns, loop */
hang:
    b hang

/* Symbols from linker script */
.extern _stack
.extern _data_loadaddr
.extern _data
.extern _edata
.extern _bss_start
.extern _bss_end
.extern main
