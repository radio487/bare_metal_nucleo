#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <stdint.h>


/*
 * Register defines
 */

#define DBGMCU_CR (*(volatile uint32_t*)(0xE0042004))
#define DBGMCU_APB1FZR1 (*(volatile uint32_t*)(0xE0042008))

/*
 * Function Prototypes
 */

void enable_TIM2_debug(void);


#endif
