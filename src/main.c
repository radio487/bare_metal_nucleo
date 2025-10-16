#include <stdint.h>
// My header files
#include "RCC.h"
#include "GPIO.h"
#include "flash.h"
#include "SysTick.h"
#include "blink_LED.h"
#include "UART.h"
#include "DHT22.h"

int main() {
  
  /* Going to max frequency */

  enable_HSI_clock();
  // HSI16
  PLL_source(2);
  int PLLN = 10, PLLM = 1, PLLR = 2;
  set_PLLN(PLLN);
  set_PLLM(PLLM);
  set_PLLR(PLLR);
  enable_PLLR();
  enable_PLL();
  // Before switching the clock frequency we need to change the flash latency for the cpu to be able to fetch instructions at the new frequency
  int lat = 4;
  set_latency(lat);

  // PLL as source for SYSCLK at 80MHz
  switch_SYSCLK(3);

  /* LED and the SysTick interrupt */

  LED2_init();
  enable_SysTick();

  /* UART */

  UART4_enable_clock();
  // We configure the parameters of the communication protocol
  UART_message_length(8);
  setup_BRR();
  // We now bring up the peripheral
  UART_init();
  
  char s[] = "hello embedded world";
  send_string(s);
 
  /* DHT 22 temperature and humidity sensor */
  
  // In the first stage of the protocol we ned to drive
  // the signal bus low for 1ms and then wait 20-40 us.
  setup_GPIOB_DHT22_output();


  return 0;
}
