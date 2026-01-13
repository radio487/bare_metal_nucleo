#include <stdint.h>
#include "../include/flash.h"
#include "../include/RCC.h"
#include "../include/TIM2.h"

/* =========================
 * Structs in .data 
 * ========================= */

// Initialize with reset values;
static bus_prescalers pre = {1U, 1U, 1U};
/* At reset MSI at 4MHZ is SYSCLK
 * HSI16 is off at reset
 * HSE is off at reset
 * PLL is off at reset
 * TIM2 is not clocked at reset */
static clock_freq_Hz f = {
  4000000U,
  0U,
  0U,
  0U,
  4000000U,
  4000000U,
  4000000U,
  4000000U,
  0U};

/* Recall this is the logic
 * f.HCLK = f.SYSCLK / pre.AHB;
 * f.PCLK1 = f.HCLK / pre.APB1;
 * f.PCLK2 = f.HCLK / pre.APB2; */

/* =========================
 * SYSCLK Sources Except PLL
 * ========================= */

void enable_MSI(void) {
  RCC_CR |= (1U << 0U);
  // The ON anr RDY bits are different
  while (~(RCC_CR & 0x2U)) {
    ;
  }
  f.MSICLK = 4000000U;
}
void disable_MSI(void) {
  RCC_CR &= ~(1U << 0U);
  // The ON anr RDY bits are different
  while (RCC_CR & 0x2U) {
    ;
  }
  f.MSICLK = 0U;
}
void enable_HSI16(void) {
  RCC_CR |= (1 << 8);
  // The ON anr RDY bits are different
  while (!(RCC_CR & (1 << 10))) {
    ;
  }
  f.HSI16CLK = 16000000U;
}
void disable_HSI16(void) {
  RCC_CR &= ~(1 << 8);
  // The ON anr RDY bits are different
  while (RCC_CR & (1 << 10)) {
    ;
  }
  f.HSI16CLK = 0U;
}
void enable_HSE(uint32_t f_HSE) {
  RCC_CR |= (1 << 16);
  // The ON anr RDY bits are different
  while (!(RCC_CR & (1 << 17))) {
    ;
  }
  f.HSECLK = f_HSE;
}
void disable_HSE(void) {
  RCC_CR &= ~(1 << 16);
  // The ON anr RDY bits are different
  while (RCC_CR & (1 << 17)) {
    ;
  }
  f.HSECLK = 0U;
}

/* ==================
 * PLL Configuration
 * ================== */

/* The micro has three PLL-s, each with yet another
 * three outputs. These three PLL-s are: PLL,
 * PLLSAI1 and PLLSAI2. Here we are only concerned
 * with PLL. The three outputs of PLL are PLLP,
 * PLLQ and PLLR, and all these can be enabled.
 * PLLR is the one that can be used for SYSCLK.
 * This means that enabling PLL is not enough and 
 * we also need to enable PLLP, PLLQ or PLLR to use 
 * them.
 *
 * The important equations are
 *
 * f(PLL) = f(PLLin) * PLLN / PLLM
 * f(PLL_P) = f(PLL) / PLLP
 * f(PLL_Q) = f(PLL) / PLLQ
 * f(PLL_R) = f(PLL) / PLLR 
 */

void enable_PLL(void) {
  RCC_CR |= (1 << 24);
  // The ON anr RDY bits are different
  while (!(RCC_CR & (1U << 25U))) {
    ;
  }
}
void disable_PLL(void) {
  RCC_CR &= ~(1 << 24);
  // The ON anr RDY bits are different
  while (RCC_CR & (1U << 25U)) {
    ;
  }
}
/* THIS IS TOO CLEVER MAKE A SWITCH */
// bit == 0 -> Input 0, bit == 1 -> MSI, bit == 2 -> HSI16, bit == 3 ->HSE
// This gives f(PLLin)
void PLL_source(int b) {
  // We have to write b in binary in bits 0 and 1
  // We first blank bits 0 to 1
  RCC_PLLCFGR &= ~(0x3);
  // And now writing the blanked bits is trivial
  RCC_PLLCFGR |= b;
}
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
// We need to enable this for SYSCLK
void enable_PLLR(void) {
  RCC_PLLCFGR |= (1 << 24);
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

/* ================
 * Changing SYSCLK
 * ================ */

// m = MSI, i = HSI16, e = HSE, p = PLL
void switch_SYSCLK(char c) {
  switch (c) {
    case 'm':
      RCC_CFGR &= ~(1 << 1);
      RCC_CFGR &= ~(1 << 0);
      while ((RCC_CFGR & 0xc) != 0x0) {
        ;
      }
      // For now we do not tweak the MSI frequency
      // although it can be tweaked.
      f.SYSCLK = f.MSICLK;
      break;
    case 'i':
      RCC_CFGR &= ~(1 << 1);
      RCC_CFGR |= (1 << 0);
      while ((RCC_CFGR & 0xc) != 0x4) {
        ;
      }
      f.SYSCLK = f.HSI16CLK;
      break;
    case 'e':
      RCC_CFGR |= (1 << 1);
      RCC_CFGR &= ~(1 << 0);
      while ((RCC_CFGR & 0xc) != 0x8) {
        ;
      }
      f.SYSCLK = f.HSECLK;
      break;
    case 'p':
      RCC_CFGR |= (1 << 1);
      RCC_CFGR |= (1 << 0);
      while ((RCC_CFGR & 0xc) != 0xc) {
        ;
      }
      f.SYSCLK = MESS?!?!;
      break;
  }
  // Changing SYSCLK changes all bus clocks
  f.HCLK = f.SYSCLK / pre.AHB;
  f.PCLK1 = f.HCLK / pre.APB1;
  f.PCLK2 = f.HCLK / pre.APB2;
}

/* ========================
 * Bus Clock Configuration
 * ======================== */

// options: 1, 2, 4, 8, 16, 64, 128, 256, 512
// Beware 32 is missing
void change_AHB_prescaler(uint32_t ahb) {
  switch (ahb) {
    case 1:
      RCC_CFGR &= ~(1U << 7U);
      break;
    case 2:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0x80);
      break;
    case 4:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0x90);
      break;
    case 8:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0xA0);
      break;
    case 16:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0xB0);
      break;
    case 64:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0xC0);
      break;
    case 128:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0xD0);
      break;
    case 256:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0xE0);
      break;
    case 512:
      RCC_CFGR &= ~(0xF0);
      RCC_CFGR |= (0xF0);
      break;
    default:
      // bad input trap cpu
      while(1) {
        ;
      }
  }
  pre.AHB = ahb;
  // Update all bus clocks as they depend on HCLK
  f.HCLK = f.SYSCLK / pre.AHB;
  f.PCLK1 = f.HCLK / pre.APB1;
  f.PCLK2 = f.HCLK / pre.APB2;
}
// options: 1, 2, 4, 8, 16
void change_APB1_prescaler(uint32_t apb1) {
  switch (apb1) {
    case 1:
      RCC_CFGR &= ~(1U << 10U);
      break;
    case 2:
      RCC_CFGR &= ~(0x700);
      RCC_CFGR |= (0x400);
      break;
    case 4:
      RCC_CFGR &= ~(0x700);
      RCC_CFGR |= (0x500);
      break;
    case 8:
      RCC_CFGR &= ~(0x700);
      RCC_CFGR |= (0x600);
      break;
    case 16:
      RCC_CFGR &= ~(0x700);
      RCC_CFGR |= (0x700);
      break;
    default:
      // bad input trap cpu
      while(1) {
        ;
      }
  }
  pre.APB1 = apb1;
  f.APB1 = f.HCLK / pre.APB1;
  // TIM2 clock depends on the APB1 prescaler

}
// options: 1, 2, 4, 8, 16
void change_APB2_prescaler(uint32_t apb2) {
  switch (apb2) {
    case 1:
      RCC_CFGR &= ~(1U << 13U);
      break;
    case 2:
      RCC_CFGR &= ~(0x3800);
      RCC_CFGR |= (0x2000);
      break;
    case 4:
      RCC_CFGR &= ~(0x3800);
      RCC_CFGR |= (0x2800);
      break;
    case 8:
      RCC_CFGR &= ~(0x3800);
      RCC_CFGR |= (0x3000);
      break;
    case 16:
      RCC_CFGR &= ~(0x3800);
      RCC_CFGR |= (0x3800);
      break;
    default:
      // bad input trap cpu
      while(1) {
        ;
      }
  }
}

/* ================================
 * Going to Max Frequency at 80MHz
 * ================================ */

void set_max_freq(struct clock_freq_Hz *f, struct bus_prescalers *pre) {
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
  f.SYSCLK *= PLLN/PLLM/PLLR;
  f.HCLK = f.SYSCLK / pre.AHB;
  f.PCLK1 = f.HCLK / pre.APB1;
  f.PCLK2 = f.HCLK / pre.APB2;
}
