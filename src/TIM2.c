#include <stdint.h>
#include "../include/RCC.h"
#include "../include/TIM2.h"
#include "../include/UART.h"
#include "../include/debug.h"


// Enables the clock to TIM2. It does nothing if it is already set
void init_clock_TIM2(void) {
  // TIM2 lives in the APB1 bus
  if (RCC_APB1ENR1 & 0x1) {
    return;
  }
  else {
    RCC_APB1ENR1 |= (1 << 0);
    // Making sure we wait till the bit shifts
    while (!(RCC_APB1ENR1 & 0x1)) {
      ;
    }
    return;
  }
}
/* IMPORTANT. TIM2 lives in APB1. For Timer clocks, there is a gotcha (specified on the RCC chapter Timer Clock section) that the clock frequency fed to the Timer depends on the prescaler of the bus (in this case APB1). If the prescaler is 1 then the frequency fed to the clock is the same as that of the bus, otherwise they are twice that of APB1. Thus, I define a helper function that yields the TIM2 clock frequency */
int TIM2_freq(struct prescalers *p, int PCLK1) {
  if (p->AHB == 1) {
    return PCLK1;
  }
  else {
    return 2*PCLK1;
  }
}
void TIM2_counter_enable(void) {
  TIM2_CR1 |= (1 << 0);
  while (!(TIM2_CR1 & 0x1)) {
    ;
  }
}
// count up or down, default is up
void TIM2_direction(char c) {
  if (c == 'u') {
    TIM2_CR1 &= ~(1 << 4);
    while (TIM2_CR1 & 0x10) {
      ;
    }
  }
  else if (c == 'd') {
    TIM2_CR1 |= (1 << 4);
    while (!(TIM2_CR1 & 0x10)) {
      ;
    }
  }
  else {
    // Bad input, freeze execution
    while(1) {
      ;
    }
  }
}
void TIM2_set_counter_value(uint32_t cnt) {
  /* I had a fun bug here. So, the ARR register
   * holds the value that is loaded to the CNT 
   * register, which is the counter. Now, the CNT
   * register starts at reset at 0, so I had configured
   * TIM2 to downcounting and it immediately underflowed
   * at the first cycle, triggering an update event. 
   * This is why we write both the ARR and the CNT 
   * registers. */
  TIM2_ARR = cnt;
  TIM2_CNT = cnt;
}
void TIM2_set_one_pulse_mode() {
  TIM2_CR1 |= (1 << 3);
  while (!(TIM2_CR1 & 0x8)) {
    ;
  }
}
// delay in seconds
void TIM2_delay(int t, int f_TIM2) {
  uint32_t ticks = (uint32_t)(t*f_TIM2);
  char s[] = "\rin while loop";

  TIM2_set_counter_value(ticks);
  TIM2_direction('d');
  TIM2_counter_enable();
  // We wait until TIM2 counts down
  while (!(TIM2_SR & 0x1)) {
    ;
  }
  // we want the timer to stop after a delay
  // TIM2_set_one_pulse_mode();
}
