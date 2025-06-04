/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2013 Frantisek Burian <BuFran@seznam.cz>
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
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Trimmed down linker script for the bluepill based on the libopencm3 version. */

/* Enforce emmition of the vector table. */
EXTERN(vector_table)

/* Define the entry point of the output file. */
ENTRY(reset_handler)

/* Define memory regions. */
MEMORY {
  /* bluepill specific */
	/*FLASH (rx) : ORIGIN = 0x08000000 , LENGTH = 64K */
	/*RAM (rwx)  : ORIGIN = 0x20000000 , LENGTH = 20K*/
  /* mps2-an385 specific */
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16M
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Define sections. */
SECTIONS {
	.text : {
		*(.vectors)	/* Vector table */
		*(.text*)	/* Program code */
		. = ALIGN(4);
		*(.rodata*)	/* Read-only data */
		. = ALIGN(4);
	} >FLASH

	.data : {
		_data = .;
		*(.data*)	/* Read-write initialized data */
		*(.ramtext*)	/* "text" functions to run in ram */
		. = ALIGN(4);
		_edata = .;
	} > RAM AT > FLASH
	_data_loadaddr = LOADADDR(.data);

	.bss : {
    _bss = .;
		*(.bss*)	/* Read-write zero initialized data */
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > RAM
}

PROVIDE(_stack = ORIGIN(RAM) + LENGTH(RAM));
