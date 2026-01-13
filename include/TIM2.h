#ifndef TIM2_HEADER
#define TIM2_HEADER


#include <stdint.h>


/*
 * Register defines
 */

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


/*
 * Function Prototypes
 */

void init_clock_TIM2();
int TIM2_freq(struct prescalers *p, int PCLK1);
void TIM2_counter_enable(void);
void TIM2_direction(char c);
void TIM2_set_counter_value(uint32_t cnt);
void TIM2_set_one_pulse_mode();
void TIM2_delay(float t, int f_TIM2);


#endif
