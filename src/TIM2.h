#define TIM2_base 0x40000000
// Basic Control registers
#define TIM2_CR1 (*(volatile uint32_t*)(TIM2_base + 0x00))
#define TIM2_CR2 (*(volatile uint32_t*)(TIM2_base + 0x04))
// I can ignore this for now
#define TIM2_SMCR (*(volatile uint32_t*)(TIM2_base + 0x08))
// DMA and interrupt enable register
#define TIM2_DIER (*(volatile uint32_t*)(TIM2_base + 0x0C))
#define TIM2_SR (*(volatile uint32_t*)(TIM2_base + 0x10))
#define TIM2_EGR (*(volatile uint32_t*)(TIM2_base + 0x14))
#define TIM2_CCMR1 (*(volatile uint32_t*)(TIM2_base + 0x18))
#define TIM2_CCMR2 (*(volatile uint32_t*)(TIM2_base + 0x1C))
#define TIM2_CCER (*(volatile uint32_t*)(TIM2_base + 0x20))
// Counter register
#define TIM2_CNT (*(volatile uint32_t*)(TIM2_base + 0x24))
// Prescaler register
#define TIM2_PSC (*(volatile uint32_t*)(TIM2_base + 0x28))
// Auto-Reload register
#define TIM2_ARR (*(volatile uint32_t*)(TIM2_base + 0x2C))
#define TIM2_CCR1 (*(volatile uint32_t*)(TIM2_base + 0x34))
#define TIM2_CCR2 (*(volatile uint32_t*)(TIM2_base + 0x38))
#define TIM2_CCR3 (*(volatile uint32_t*)(TIM2_base + 0x3C))
#define TIM2_CCR4 (*(volatile uint32_t*)(TIM2_base + 0x40))
#define TIM2_DCR (*(volatile uint32_t*)(TIM2_base + 0x48))
#define TIM2_DMAR (*(volatile uint32_t*)(TIM2_base + 0x4C))
#define TIM2_OR1 (*(volatile uint32_t*)(TIM2_base + 0x50))
#define TIM2_OR2 (*(volatile uint32_t*)(TIM2_base + 0x60))

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
/* IMPORTANT. TIM2 lives in APB1. For Timer clocks, there is a gotcha (specified on the RCC chapter Timer Clock section) that the clock frequency fed to the Timer depends on the prescaler of the bus (in this case APB1). If the prescaler is 1 then the frequency fed to the clock is the same as that of the bus, otherwise thery are twice that of APB1. Thus, I define a helper function that yields the TIM2 clock frequency */
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
  TIM2_CNT = cnt;
}
// delay in seconds
void TIM2_delay(int t, int f_TIM2) {
  uint32_t ticks = (uint32_t)(t*f_TIM2);

  TIM2_set_counter_value(ticks);
  TIM2_direction('d');
  TIM2_counter_enable();
  while (!(TIM2_SR & 0x1)) {
    ;
  }
}

