#include <stdint.h>
// My header files
#include "GPIO.h"
#include "flash.h"
#include "rcc.h"
#include "SysTick.h"
#include "blink_LED.h"
#include "UART.h"

int main() {
  enable_HSE_clock();
  // For some reason I cannot go higher than f = 7. Probably if I go higher I exceed some frequency limit in some bus
  int pll_factor = 9;
  PLL_mult_factor(pll_factor);
  // HSE chosen
  PLL_source(1);
  enable_PLL();
  // We decide whether to change the FLASH instruction access latency
  int freq = pll_factor*8;
  setup_FLASH_latency(freq);
  // we go from HSI to PLL with HSE as a source 
  switch_SYSCLK(2);
  setup_LED();
  enable_SysTick();
  while (1) {
     ;
  }
  return 0;
}
