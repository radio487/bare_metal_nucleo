#include <stdint.h>
#include "../include/debug.h"

/*
 * It is noteworthy that the debug peripheral does not need to have RCC set its clock. As long as the st-link is powered, this unit is clocked properly.
*/

// Halts TIM2 if we are debugging
void enable_TIM2_debug(void) {
  DBGMCU_APB1FZR1 |= (1 << 0);
  while (!(DBGMCU_APB1FZR1 & 0x1)) {
    ;
  }
}
