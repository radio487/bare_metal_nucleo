#ifndef RCC_HEADER
#define RCC_HEADER


#include <stdint.h>

/*
 * Register defines
 */

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


/*
 * Structs
 */

// bus clock frequency prescalers
struct prescalers {
  int AHB;
  int APB1;
  int APB2;
};
// bus clock frequencies
struct clock_freq {
  int SYSCLK;
  int HCLK;
  int PCLK1;
  int PCLK2;
};


/*
 * Function Prototypes
 */

void enable_HSI_clock(void);
void enable_HSE_clock(void);
void PLL_source(int b);
void set_PLLN(int f);
void set_PLLM(int f);
void set_PLLR(int d);
void enable_PLLR(void);
void enable_PLL(void);
void switch_SYSCLK(int b);
void set_max_freq(struct clock_freq *f, struct prescalers *pre);


#endif
