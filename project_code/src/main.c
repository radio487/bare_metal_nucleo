#include <stdint.h>
// My header files
#include "GPIO.h"
// #include "flash.h"
#include "RCC.h"
#include "SysTick.h"
#include "blink_LED.h"
// #include "UART.h"

int main() {

  /* Going to max frequency */

  enable_HSI_clock();
  // HSI16
  PLL_source(2);
  int PLLN = 10, PLLM = 1, PLLR = 2;
  set_PLLN(PLLN);
  set_PLLM(PLLM);
  set_PLLM(PLLR);
  enable_PLLR();
  enable_PLL();
  // PLL as source for SYSCLK at 80MHz
  switch_SYSCLK(3);

  /* LED and the SysTick interrupt */

  LED2_init();
  enable_SysTick();
  
  /* UART */
  
  // First we configure the parameters of the comunication protocol
  // UART_message_length(8);
  // setup_BRR();
  // UART_enable_parity(0);
  // We now bring up the peripheral
  UART_init();

  while (1) {
    ;
    // We send the character
    // initiate_transmission();
  }
  return 0;
}
