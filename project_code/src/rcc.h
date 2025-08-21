#define RCC_base 0x40021000
#define RCC_CR (*(volatile uint32_t*)(RCC_base + 0x00))
#define RCC_CFGR (*(volatile uint32_t*)(RCC_base + 0x04))
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_base + 0x18))

void enable_PLL(void) {
  RCC_CR |= (1 << 24);
  // wait till hardware confirms PLL is on
  while (((RCC_CR >> 25) ^ 1)) {
    ;
  }
}
void enable_HSE_clock(void) {
  RCC_CR |= (1 << 16);
  // wait till the hardware condirms HSE is on
  while (!((RCC_CR >> 17) & 1)) {
    ;
  }
}
void enable_HSI_clock(void) {
  RCC_CR |= (1 << 0);
  // wait till the hardware condirms HSI is on
  while (!((RCC_CR >> 1) & 1)) {
    ;
  }
}
// Do not exceed 72MHz according to the ref manual, that is f <= 9
void PLL_mult_factor(int f) {
  // there is a hackier way to do this by noticing the bits to be written are f-2 in binary (except the last case which is a corner case and kinda redundat), but for now, a switch statement will do
  switch (f) {
    case 2:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR &= ~(1 << 18);
      // This is more programmatic but perhaps less readable
      // for (int i = 18; i < 22; ++i) {
      //   RCC_CFGR &= ~(1 << i);
      // }
      break;
    case 3:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 4:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
    case 5:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 6:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
    case 7:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 8:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
    case 9:
      RCC_CFGR &= ~(1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 10:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
    case 11:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 12:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
    case 13:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR &= ~(1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 14:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
    case 15:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR &= ~(1 << 19);
      RCC_CFGR |= (1 << 18);
      break;
    case 16:
      RCC_CFGR |= (1 << 21);
      RCC_CFGR |= (1 << 20);
      RCC_CFGR |= (1 << 19);
      RCC_CFGR &= ~(1 << 18);
      break;
  }
}
// bit == 0 -> HSI/2, bit == 1 -> HSE
void PLL_source(int bit) {
  if (bit == 1) {
    RCC_CFGR |= (1 << 16);
  }
  else if (bit == 0) {
    RCC_CFGR &= ~(1 << 16);
  }
}
void switch_SYSCLK(int b) {
  switch (b) {
    case 0:
      RCC_CFGR &= ~(1 << 1);
      RCC_CFGR &= ~(1 << 0);
      break;
    case 1:
      RCC_CFGR &= ~(1 << 1);
      RCC_CFGR |= (1 << 0);
      break;
    case 2:
      RCC_CFGR |= (1 << 1);
      RCC_CFGR &= ~(1 << 0);
      break;
  }
}
