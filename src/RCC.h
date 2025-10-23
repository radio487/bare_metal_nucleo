#define RCC_base 0x40021000
#define RCC_CR (*(volatile uint32_t*)(RCC_base + 0x00))
#define RCC_ICSCR (*(volatile uint32_t*)(RCC_base + 0x04))
#define RCC_CFGR (*(volatile uint32_t*)(RCC_base + 0x08))
#define RCC_PLLCFGR (*(volatile uint32_t*)(RCC_base + 0x0C))
#define RCC_PLLSAI1CFGR (*(volatile uint32_t*)(RCC_base + 0x10))
#define RCC_PLLSAI2CFGR (*(volatile uint32_t*)(RCC_base + 0x14))
#define RCC_CIER (*(volatile uint32_t*)(RCC_base + 0x18))
#define RCC_CIFR (*(volatile uint32_t*)(RCC_base + 0x1C))
#define RCC_CICR (*(volatile uint32_t*)(RCC_base + 0x20))
#define RCC_AHB1RSTR (*(volatile uint32_t*)(RCC_base + 0x28))
#define RCC_AHB2RSTR (*(volatile uint32_t*)(RCC_base + 0x2C))
#define RCC_AHB3RSTR (*(volatile uint32_t*)(RCC_base + 0x30))
#define RCC_APB1RSTR1 (*(volatile uint32_t*)(RCC_base + 0x38))
#define RCC_APB1RSTR2 (*(volatile uint32_t*)(RCC_base + 0x3C))
#define RCC_APB2RSTR (*(volatile uint32_t*)(RCC_base + 0x40))
#define RCC_AHB1ENR (*(volatile uint32_t*)(RCC_base + 0x48))
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC_base + 0x4C))
#define RCC_AHB3ENR (*(volatile uint32_t*)(RCC_base + 0x50))
#define RCC_APB1ENR1 (*(volatile uint32_t*)(RCC_base + 0x58))
#define RCC_APB1ENR2 (*(volatile uint32_t*)(RCC_base + 0x5C))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_base + 0x60))
#define RCC_AHB1SMENR (*(volatile uint32_t*)(RCC_base + 0x68))
#define RCC_AHB2SMENR (*(volatile uint32_t*)(RCC_base + 0x6C))
#define RCC_AHB3SMENR (*(volatile uint32_t*)(RCC_base + 0x70))
#define RCC_APB1SMENR1 (*(volatile uint32_t*)(RCC_base + 0x78))
#define RCC_APB1SMENR2 (*(volatile uint32_t*)(RCC_base + 0x7C))
#define RCC_APB2SMENR (*(volatile uint32_t*)(RCC_base + 0x80))
#define RCC_CCIPR (*(volatile uint32_t*)(RCC_base + 0x88))
#define RCC_BDCR (*(volatile uint32_t*)(RCC_base + 0x90))
#define RCC_CSR (*(volatile uint32_t*)(RCC_base + 0x94))

void enable_HSI_clock(void) {
  RCC_CR |= (1 << 8);
  // wait till the hardware confirms HSI is on
  while (!(RCC_CR & (1 << 10))) {
    ;
  }
}
void enable_HSE_clock(void) {
  RCC_CR |= (1 << 16);
  // wait till the hardware confirms HSE is on
  while (!(RCC_CR & (1 << 17))) {
    ;
  }
}
// bit == 0 -> No clock, bit == 1 -> MSI, bit == 2 -> HSI16, bit == 3 ->HSE
void PLL_source(int b) {
  // We have to write b in binary in bits 0 and 1
  // We first blank bits 0 to 1
  RCC_PLLCFGR &= ~(0x3);
  // And now writing the blanked bits is trivial
  RCC_PLLCFGR |= b;
}
// void enable_HSI_clock(void) {
//   RCC_CR |= (1 << 0);
//   // wait till the hardware condirms HSI is on
//   while (!((RCC_CR >> 1) & 1)) {
//     ;
//   }
// }
// The multiplication factor
void set_PLLN(int f) {
  // 8 <= f <= 86
  if (f < 8 || f > 86) {
    while (1) {
      ;
    }
  }
  // Notice we need to write f in binary starting at bit 8
  RCC_PLLCFGR &= ~(0x7F << 8);
  RCC_PLLCFGR |= (f << 8);
}
// The division factor
// Notice we need to write f-1 in binary starting at bit 4
void set_PLLM(int f) {
  RCC_PLLCFGR &= ~(0x7 << 4);
  RCC_PLLCFGR |= ((f-1) << 4);
}
void set_PLLR(int d) {
  // we edit bits 26 and 25
  switch (d) {
    case 2:
      RCC_PLLCFGR &= ~(0x3 << 25);
      break;
    case 4:
      RCC_PLLCFGR |= (0x3 << 25);
      RCC_PLLCFGR &= ~(0x1 << 25);
      break;
    case 6:
      RCC_PLLCFGR |= (0x3 << 25);
      RCC_PLLCFGR &= ~(0x2 << 25);
      break;
    case 8:
      RCC_PLLCFGR |= (0x3 << 25);
      RCC_PLLCFGR &= ~(0x3 << 25);
      break;
  }
}
// We need to enable this for SYSCLK
void enable_PLLR(void) {
  RCC_PLLCFGR |= (1 << 24);
}
void enable_PLL(void) {
  RCC_CR |= (1 << 24);
  // wait till hardware confirms PLL is on
  while (((RCC_CR >> 25) ^ 1)) {
    ;
  }
}
void switch_SYSCLK(int b) {
  switch (b) {
    case 0:
      RCC_CFGR &= ~(1 << 1);
      RCC_CFGR &= ~(1 << 0);
      while ((RCC_CFGR & 0xc) != 0x0) {
        ;
      }
      break;
    case 1:
      RCC_CFGR &= ~(1 << 1);
      RCC_CFGR |= (1 << 0);
      while ((RCC_CFGR & 0xc) != 0x4) {
        ;
      }
      break;
    case 2:
      RCC_CFGR |= (1 << 1);
      RCC_CFGR &= ~(1 << 0);
      while ((RCC_CFGR & 0xc) != 0x8) {
        ;
      }
      break;
    case 3:
      RCC_CFGR |= (1 << 1);
      RCC_CFGR |= (1 << 0);
      while ((RCC_CFGR & 0xc) != 0xc) {
        ;
      }
      break;
  }
}
void set_max_freq(int *freq) {
  int PLLN = 10, PLLM = 1, PLLR = 2, lat = 4;

  // At starupt HSI16 should be enabled CHECK so this step is redundant at startup. Maybe take it off.
  enable_HSI_clock();
  // HSI16
  PLL_source(2);
  set_PLLN(PLLN);
  set_PLLM(PLLM);
  set_PLLR(PLLR);
  enable_PLLR();
  enable_PLL();

  // Before switching the clock frequency we need to change the flash latency for the cpu to be able to fetch instructions at the new frequency
  set_latency(lat);
  
  // PLL as source for SYSCLK at 80MHz
  switch_SYSCLK(3);

  // We need to update
  *freq *= PLLN/PLLM/PLLR;
}
