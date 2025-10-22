#define TIM2_base 0x40000000
#define TIM2_CR1 (*(volatile uint32_t*)(TIM2_base + 0x00))
#define TIM2_CR2 (*(volatile uint32_t*)(TIM2_base + 0x04))
#define TIM2_SMCR (*(volatile uint32_t*)(TIM2_base + 0x08))
#define TIM2_DIER (*(volatile uint32_t*)(TIM2_base + 0x0C))
#define TIM2_SR (*(volatile uint32_t*)(TIM2_base + 0x10))
#define TIM2_EGR (*(volatile uint32_t*)(TIM2_base + 0x14))
#define TIM2_CCMR1 (*(volatile uint32_t*)(TIM2_base + 0x18))
#define TIM2_CCMR2 (*(volatile uint32_t*)(TIM2_base + 0x1C))
#define TIM2_CCER (*(volatile uint32_t*)(TIM2_base + 0x20))
#define TIM2_CNT (*(volatile uint32_t*)(TIM2_base + 0x24))
#define TIM2_PSC (*(volatile uint32_t*)(TIM2_base + 0x28))
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
