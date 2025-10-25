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
  
  /* Going to Max Frequency */

  struct prescalers pre;
  // Reset values
  pre.AHB = 1;
  pre.APB1 = 1; 
  pre.APB2 = 1;

  struct clock_freq f;
  // Reset frequency is the HSI16 oscilator: 16MHz
  f.SYSCLK = 16000000; 
  // MAKE THIS FLOATS?!?!!?!
  f.HCLK = f.SYSCLK / pre.AHB;
  f.PCLK1 = f.HCLK / pre.APB1;
  f.PCLK2 = f.HCLK / pre.APB2;

  set_max_freq(&f, &pre);

  /* LED and the SysTick Interrupt */

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
 
  /* DHT 22 Temperature and Humidity Sensor */
  
  // Let us first bring the clock on TIM2 up
  init_clock_TIM2();

  // The clock frequency of the Timer depends on the prescaler of the APB1 prescaler. If this prescaler is not 1, then it is twice PCLK1.
  // Time is in seconds
  // Do NOT set t_delay > 50 seconds, as the tick number will overflow the uint32_t variable
  int f_TIM2, t_delay = 10;
  f_TIM2 = TIM2_freq(&pre, f.PCLK1);
  
  char s1[] = "Start";
  send_string(s1);
  TIM2_delay(t_delay, f_TIM2);
  char s2[] = "End";
  send_string(s2);

  // In the first stage of the protocol we ned to drive
  // the signal bus low for 1ms and then wait 20-40 us.
  // setup_GPIOB_DHT22_output();
  
  while (1) {
    ;
  }
  return 0;
}
