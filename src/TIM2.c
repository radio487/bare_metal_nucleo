#include <stdint.h>
#include "../include/RCC.h"
#include "../include/TIM2.h"


// Enables the clock to TIM2. It does nothing if it is already set
void init_clock_TIM2() {
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
  // We need to write to the ARR register NOT the CNT directly
  TIM2_ARR = cnt;
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

  TIM2_set_counter_value(ticks);
  TIM2_direction('d');
  // we want the timer to stop after a delay
  // TIM2_set_one_pulse_mode();
  TIM2_counter_enable();
  // while (!(TIM2_SR & 0x1)) {
  //   ;
  // }
}
