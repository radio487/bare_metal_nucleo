void idle_sec(int freq) {
  // freq is in MHz.
  // Using gdb we can verify this loop is 9 instructions per iteration.
  int volatile one_sec = freq/9;
  for (int i = 0; i < one_sec; ++i) {
    __asm__("nop");
  }
}
void enable_RCC_LED(void) {
  // By default peripherals on the board are not powered which means we need to manually activate the clock. This is achieved with the RCC (Reset Clock Control) registers
  // The onboard LED (the green one, not the automatic red one) is GPIO13 which is tied to the APB2 bus and according to the reference manual we need to tweak the RCC_APB2ENR register, in particular shifting bit 4 up which controls GPIOC
  RCC_APB2ENR |= (1 << 4);
}
void disable_RCC_LED(void) {
  // BUILD
}
void set_GPIOC_output_mode_2MHz(void) {
  // 2MHz is a built in freq for the GPIO and it tells us the speed at which the GPIO can change from logic high to low.
  // I change the CNF bits to general purpose output push-pull, that is we go from reset value 01 to 00 in bits 23 and 22
  // Thus I need to only zero bit 22
  GPIOC_CRH &= 0xFFBFFFFF;
  // To light the LED we set MODE to output 2MHz. I choose 2MHz because the online pinouts
  // of the bluepill say this pin has a max 2MHz.
  // Thus, according to the reference manual the mode goes from reset 00 to 10 in bits 21 and 20
  // This will turn the LED on, but we should not use it as a handler. There is another register for that.
  GPIOC_CRH |= (1 << 21);
}
// We pack all the prep in one function
void setup_LED(void) {
  enable_RCC_LED();
  set_GPIOC_output_mode_2MHz();
}
// If LED is on it switches it off and vice-versa
void toggle_LED(void) {
  // PC13 is active low, so to switch the LED off we need to set the Output Data Register's 13-th bit to 1. To switch it on again we would change the same bit. These two operations can be elegantly combined with XOR
  GPIOC_ODR ^= (1 << 13);
}
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
