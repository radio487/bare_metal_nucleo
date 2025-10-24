#include <stdint.h>
// My header files
#include "structs.h"
#include "flash.h"
#include "RCC.h"
#include "GPIO.h"
#include "SysTick.h"
#include "TIM2.h"
#include "blink_LED.h"
#include "UART.h"
#include "DHT22.h"
#include "Interrupts.h"


int main(int argc, char **argv) {
  
  /* Going to max frequency */

  struct prescalers pre;
  // Reset values
  pre.AHB = 1;
  pre.APB1 = 1; 
  pre.APB2 = 1;

  struct clock_freq f;
  // Reset frequency is the HSI16 oscilator: 16MHz
  f.SYSCLK = 16000000; 
  f.HCLK = f.SYSCLK / pre.AHB;
  f.PCLK1 = f.HCLK / pre.APB1;
  f.PCLK2 = f.HCLK / pre.APB2;

  set_max_freq(&f, &pre);

  /* LED and the SysTick interrupt */

  LED2_init();
  // The interrupt of SysTick is toggling the LED
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
  
  // Let us first bring the clock on TIM2 up
  // The clock frequency of the Timer depends on the prescaler of the APB1 prescaler. If this prescaler is not 1, then it iw twice PCLK1.
  // This means it will be handy to keep a separate variable
  int f_TIM2;
  // HERE DECIDE HOW TO DEFINE THE TIMER CLOCK FREQUENCIES. MAYBE pack them in a peripheral clock struct
  init_clock_TIM2(&f_TIM2);

  // In the first stage of the protocol we ned to drive
  // the signal bus low for 1ms and then wait 20-40 us.
  setup_GPIOB_DHT22_output();
  
  while (1) {
    ;
  }
  return 0;
}
