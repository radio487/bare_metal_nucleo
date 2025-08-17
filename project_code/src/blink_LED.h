// RCC registers for the APB2 bus where the green LED lives
#define RCC_CR_base 0x40021000
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_CR_base + 0x18))

// GPIOC registers
#define GPIOC_base 0x40011000
// GPIOC from 0 to 7
#define GPIOC_CRL (*(volatile uint32_t*)(GPIO_base + 0x00))
// GPIOC from 8 to 15. LED is 13.
#define GPIOC_CRL2 (*(volatile uint32_t*)(GPIO_base + 0x04))
#define GPIOC_ODR (*(volatile uint32_t*)(GPIO_base + 0x0C))

void idle_sec(void) {
  // gdb tells me this loop is 9 instructions per iteration
  // Recall the HSI clock is 8MHz
  int volatile one_sec = 8000000/9;
  for (int i = 0; i < one_sec; ++i) {
    __asm__("nop");
  }
  return;
}
void enable_RCC_LED(void) {
  // By default peripherals on the board are not powered
  // This means we need to manually activate the clock
  // This is achieved with the RCC (Reset Clock Control) registers
  // The onboard LED (the green one, not the automatic red one) is GPIO13 which is tied to the APB2 bus and according to the reference manual we need to tweak the RCC_APB2ENR register, in particular shifting bit 4 up which controls GPIOC
  RCC_APB2ENR |= (1 << 4);
  return;
}
void disable_RCC_LED(void) {
  // BUILD
  return;
}
void set_GPIOC_output_mode_2MHz(void) {
  // 2MHz is a built in freq for the GPIO and it tells us the speed at which the GPIO can change from logic high to low.
  // I change the CNF bits to general purpose output push-pull, that is we go from reset value 01 to 00 in bits 23 and 22
  // Thus I need to only zero bit 22
  GPIOC_CRL2 &= 0xFFBFFFFF;
  // To light the LED we set MODE to output 2MHz. I choose 2MHz because the online pinouts
  // of the bluepill say this pin has a max 2MHz.
  // Thus, according to the reference manual the mode goes from reset 00 to 10 in bits 21 and 20
  // This will turn the LED on, but we should not use it as a handler. There is another register for that.
  GPIOC_CRL2 |= (1 << 21);
  return;
}
void switch_LED_off(void) {
  // PC13 is active low, so to switch the LED off we need to set the
  // Output Data Register's 13-th bit to 1
  GPIOC_ODR |= (1 << 13);
  return;
}
void switch_LED_on(void) {
  // Make the bit change when offing 0 again to switch the LED again
  GPIOC_ODR &= 0x00000000;
  return;
}
void toggle_LED_on_off(void) {
  enable_RCC_LED();
  set_GPIOC_output_mode_2MHz();
  while (1) {
    idle_sec();
    // When configuring the GPIO the default is to have the LED on so we need to start offing
    switch_LED_off();
    idle_sec();
    switch_LED_on();
  }
  disable_RCC_LED();
  return;
}
