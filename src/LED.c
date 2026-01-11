#include <stdint.h>
#include "../include/GPIO.h"
#include "../include/LED.h"


void idle_sec(int freq) {
  // freq is in MHz.
  // Using gdb we can verify this loop is 9 instructions per iteration.
  int volatile one_sec = freq/9;
  for (int i = 0; i < one_sec; ++i) {
    __asm__("nop");
  }
}
void LED2_init(void) {
  /* By default peripherals on the board are not powered which means we need to manually activate clock access to them. This is achieved with the RCC (Reset Clock Control) registers. LED2 is tied to GPIOA pin 5 (it is also tied to the Arduino header D13). This peripheral lives in the AHB2 bus
   */
  init_GPIOA();

  // We now configure GPIOA pin 5 to general purpose output mode.
  GPIOA_MODER &= ~(1 << 11);
  GPIOA_MODER |= (1 << 10);
  // Output push pull
  GPIOA_OTYPER &= ~(1 << 15);
  // Low speed
  GPIOA_OSPEEDR &= ~(1 << 11);
  GPIOA_OSPEEDR &= ~(1 << 10);
  // no pull-up pull-down
  GPIOA_PUPDR &= ~(1 << 11);
  GPIOA_PUPDR &= ~(1 << 10);
  // Atomic bit changes. This modifies the corresponding GPIOA_ODR bit as well
  GPIOA_BSRR |= (1 << 5);
}
