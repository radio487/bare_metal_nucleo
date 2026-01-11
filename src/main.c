// Standard includes
#include <stdint.h>
// My header files
// #include "function_prototypes.h"
#include "../include/flash.h"
#include "../include/RCC.h"
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/SysTick.h"
#include "../include/UART.h"
#include "../include/TIM2.h"
// #include "DHT22.h"
// #include "Interrupts.h"

// No OS so no need for int argc, char **argv
int main(void) {

  // Testing the FPU
  volatile float tf0 = 1.3, tf1 = 2.4;
  volatile float tf2 = tf0 + tf1;
  
  /* Going to Max Frequency */

  struct prescalers pre;
  // Reset values
  pre.AHB = 1;
  pre.APB1 = 1; 
  pre.APB2 = 1;

  struct clock_freq f;
  // Reset frequency comes from the HSI16 oscilator 16MHz
  f.SYSCLK = 16000000; 
  // MAKE THESE FLOATS?!?
  f.HCLK = f.SYSCLK / pre.AHB;
  f.PCLK1 = f.HCLK / pre.APB1;
  f.PCLK2 = f.HCLK / pre.APB2;

  set_max_freq(&f, &pre);

  /*
   * ==============================
   * LED and the SysTick Interrupt
   * ==============================
   */

  LED2_init();
  // The interrupt of SysTick is toggling the LED
  enable_SysTick();

  /*
   * =====
   * UART 
   * =====
   */
  
  // UART will be useful to "printf" debug with a Raspberry Pi
  UART4_enable_clock();
  // We configure the parameters of the communication protocol
  UART_message_length(8);
  setup_BRR();
  // We now bring up the peripheral
  UART_init();
  
  /*
   * =======================================
   * DHT 22 Temperature and Humidity Sensor
   * =======================================
   */

  /* In Construction */

  // We will use TIM2 to implement the communication protocol with the sensor
  init_clock_TIM2();
  /*
   * The clock frequency of the Timer depends on the prescaler of the APB1 prescaler. If this prescaler is not 1, then it is twice PCLK1.
   * Time is in seconds.
   * Do not set t_delay > 50 seconds, as the tick number will overflow the uint32_t variable.
   */
  int f_TIM2, t_delay = 50;
  f_TIM2 = TIM2_freq(&pre, f.PCLK1);
  
  char s1[] = "Start";
  send_string(s1);
  TIM2_delay(t_delay, f_TIM2);
  // char s2[] = "End";
  // send_string(s2);

  // In the first stage of the protocol we ned to drive
  // the Data bus low for 1ms and then wait 20-40 us.
  // setup_GPIOB_DHT22_output();
  
  while (1) {
    ;
  }
  return 0;
}
