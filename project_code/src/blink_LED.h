void idle_sec(int freq) {
  // freq is in MHz.
  // Using gdb we can verify this loop is 9 instructions per iteration.
  int volatile one_sec = freq/9;
  for (int i = 0; i < one_sec; ++i) {
    __asm__("nop");
  }
}
void LED2_init(void) {
  /* By default peripherals on the board are not powered which means we need to manually activate clock access to them. This is achieved with the RCC (Reset Clock Control) registers. LED2 is tied to GPIOA pin 5 (it is also tied to the Arduino header D13). This peripheral lives in the AHB2 bus
   */
  RCC_AHB2ENR |= (1 << 0);
  // We now configure GPIOA pin 5 to general purpose output mode.
  GPIOA_MODER &= ~(1 << 11);
  GPIOA_MODER |= (1 << 10);
  // Output push pull
  GPIOA_OTYPER &= ~(1 << 15);
  // Low speed
  GPIOA_OSPEEDR &= ~(1 << 11);
  GPIOA_OSPEEDR &= ~(1 << 10);
  // no pull-up pull-down
  GPIOA_PUPDR &= ~(1 << 11);
  GPIOA_PUPDR &= ~(1 << 10);
  // Atomic bit changes. This modifies the corresponding GPIOA_ODR bit as well
  GPIOA_BSRR |= (1 << 5);
}

// We pack all the prep in one function
// void setup_LED2(void) {
//   enable_RCC_LED();
//   set_GPIOC_output_mode_2MHz();
// }
// If LED is on it switches it off and vice-versa
// void toggle_LED(void) {
//   // PC13 is active low, so to switch the LED off we need to set the Output Data Register's 13-th bit to 1. To switch it on again we would change the same bit. These two operations can be elegantly combined with XOR
//   GPIOC_ODR ^= (1 << 13);
// }
// void manual_LED_on_off(void) {
//   setup_LED();
//   while (1) {
//     idle_sec();
//     // When configuring the GPIO the default is to have the LED on so we need to start offing
//     toggle_LED();
//     idle_sec();
//     toggle_LED();
//   }
//   disable_RCC_LED();
// }
