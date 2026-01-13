// Standard includes
#include <stdint.h>
// My header files
#include "../include/flash.h"
#include "../include/RCC.h"
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/SysTick.h"
#include "../include/UART.h"
#include "../include/TIM2.h"
#include "../include/debug.h"
// #include "DHT22.h"
// #include "Interrupts.h"

// No OS so no need for int argc, char **argv
int main(void) {

  /*
   * ====================================
   * Bringing the board to Max Frequency
   * ====================================
   */

  // struct bus_prescalers pre;
  // struct clock_freq_Hz f;
  // Initialize with reset values
  // pre.AHB = 1U;
  // pre.APB1 = 1U; 
  // pre.APB2 = 1U;
  // f.SYSCLK = 16000000U; 
  
  // f.HCLK = f.SYSCLK / pre.AHB;
  // f.PCLK1 = f.HCLK / pre.APB1;
  // f.PCLK2 = f.HCLK / pre.APB2;

  set_max_freq(&f, &pre);

  /*
   * ==============================
   * LED and the SysTick Interrupt
   * ==============================
   */

  LED2_init();
  // The SysTick interrupt is toggling the LED
  enable_SysTick();

  /*
   * =========================
   * UART with a Raspberry Pi
   * =========================
   */
  
  // UART will be useful to "printf" debug with a Raspberry Pi
  UART4_enable_clock();
  // We configure the parameters of the communication protocol
  UART_message_length(8);
  setup_BRR();
  // We now bring up the peripheral
  UART_init();

  char s0[] = "\rUART comms working between the Nucleo and the Pi";
  send_string(s0);
  
  /*
   * =======================================
   * TIM2 timer
   * =======================================
   */

  // We will use TIM2 to implement the communication protocol with the sensor
  init_clock_TIM2();
  // TIM2 will freeze with the CPU while debugging
  enable_TIM2_debug(); 

  /*
   * The clock frequency of the Timer depends on the prescaler of the APB1 prescaler. If this prescaler is not 1, then it is twice PCLK1.
   * Time is in seconds and frequency in Hz.
   * Do not set t_delay > 50 seconds, as the tick number will overflow the uint32_t variable.
   */
  int f_TIM2;
  // one milisecond
  float t_delay = 0.001;

  f_TIM2 = TIM2_freq(&pre, f.PCLK1);
  
  char s1[] = "\rTim2 delay start";
  char s2[] = "\rTim2 delay end";

  send_string(s1);
  TIM2_delay(t_delay, f_TIM2);
  send_string(s2);
  t_delay = 10.0;
  send_string(s1);
  TIM2_delay(t_delay, f_TIM2);
  send_string(s2);

  // In the first stage of the protocol we need to drive
  // the Data bus low for 1ms and then wait 20-40 us.
  // setup_GPIOB_DHT22_output();
  
  while (1) {
    ;
  }
  return 0;
}
