// See the Cortex M3 programming manual
#define SysTick_base 0xE000E010
#define STK_CTRL (*(volatile uint32_t*)(SysTick_base + 0x00))
#define STK_LOAD (*(volatile uint32_t*)(SysTick_base + 0x04))
#define STK_VAL (*(volatile uint32_t*)(SysTick_base + 0x08))
#define STK_CALIB (*(volatile uint32_t*)(SysTick_base + 0x0C))

void enable_SysTick(void) {
  // The order of manipulation of these registers is important. If for instance we were to modify STK_CTRL first, the clock would go off with whatever garbage value was set in the other registers.
  // Writing anything to this register clears it and sets the COUNTFLAG in STK_CTRL to 0 so we conventionally write zero to it.
  STK_VAL = 0x00000000;
  // With a SYSCLK at 8MHz, the maxixmum (remember SysTick is 24 bits) value corresponds to approx 2.097 seconds between interrupts.
  STK_LOAD = 0x00FFFFFF;
  // STK_LOAD = 0x0000000A;
  // Now that we have configured the other registers, we trigger the timer with STK_CTRL
  // Manipulating bit 2 we choose the AHB clock
  STK_CTRL |= (1 << 2);
  // Manipulating bit 1 we assert that the interrupt will trigger when reaching zero
  STK_CTRL |= (1 << 1);
  // Manipulating bit 0 we trigger the clock
  STK_CTRL |= (1 << 0);
}
