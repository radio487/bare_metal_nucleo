#include <stdint.h>
#include "../include/flash.h"
#include "../include/RCC.h"


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
void set_max_freq(struct clock_freq *f, struct prescalers *pre) {
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
  f->SYSCLK *= PLLN/PLLM/PLLR;
  f->HCLK = f->SYSCLK / pre->AHB;
  f->PCLK1 = f->HCLK / pre->APB1;
  f->PCLK2 = f->HCLK / pre->APB2;
}
