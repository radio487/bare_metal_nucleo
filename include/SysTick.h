#ifndef SYSTICK_HEADER
#define SYSTICK_HEADER


#include <stdint.h>

/*
 * Register defines
 */

#define SysTick_base 0xE000E010
#define STK_CTRL (*(volatile uint32_t*)(SysTick_base + 0x00))
#define STK_LOAD (*(volatile uint32_t*)(SysTick_base + 0x04))
#define STK_VAL (*(volatile uint32_t*)(SysTick_base + 0x08))
#define STK_CALIB (*(volatile uint32_t*)(SysTick_base + 0x0C))

/*
 * Function Prototypes
 */

void enable_SysTick(void);

#endif
