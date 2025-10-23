#include <stdint.h>
// My header files
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
  
  // Startup frequency is that of the HSI16 oscilator: 16MHz
  // We wanna keep a variable in the scope of main() that keeps track of the current frequency to be able to configure delays in timers
  // In Hz
  int freq = 16000000;
  set_max_freq(&freq);

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
  init_clock_TIM2();

  // In the first stage of the protocol we ned to drive
  // the signal bus low for 1ms and then wait 20-40 us.
  setup_GPIOB_DHT22_output();
  
  while (1) {
    ;
  }
  return 0;
}
