#ifndef FLASH_HEADER
#define FLASH_HEADER


#include <stdint.h>

/*
 * Register defines
 */

#define FLASH_registers_base 0x40022000
#define FLASH_ACR (*(volatile uint32_t*)(FLASH_registers_base + 0x00))


/*
 * Function Prototypes
 */

void set_latency(int lat);


#endif
