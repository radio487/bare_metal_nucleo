#define UART4_base 0x40004C00
// Control Register 1
#define UART4_CR1 (*(volatile uint32_t*)(UART4_base + 0x00))
// Control Register 2
#define UART4_CR2 (*(volatile uint32_t*)(UART4_base + 0x04))
// Control Register 3
#define UART4_CR3 (*(volatile uint32_t*)(UART4_base + 0x08))
// Baud Rate Register
#define UART4_BRR (*(volatile uint32_t*)(UART4_base + 0x0C))
// Guard Time Prescaler reguster
#define UART4_GTPR (*(volatile uint32_t*)(UART4_base + 0x10))
// Receiver Timeout Register
#define UART4_RTOR (*(volatile uint32_t*)(UART4_base + 0x14))
// Request Register
#define UART4_RQR (*(volatile uint32_t*)(UART4_base + 0x18))
// Interrupts ans Status Register
#define UART4_ISR (*(volatile uint32_t*)(UART4_base + 0x1C))
// Interrupts flag Clear Register
#define UART4_ICR (*(volatile uint32_t*)(UART4_base + 0x20))
// Receive Data Register
#define UART4_RDR (*(volatile uint32_t*)(UART4_base + 0x24))
// Transmit Data Register
#define UART4_TDR (*(volatile uint32_t*)(UART4_base + 0x28))

void UART4_enable_clock(void) {
  // The UART4 peripheral can be mapped to either GPIOA or GPIOC. For no particular reason I choose GPIOA
  // The actual pins are PA0 for Tx and PA1 for Rx 
  // First we activate the clock on GPIOA
  RCC_AHB2ENR |= (1 << 0);
  // Then we activate the clock on the UART4 peripheral, which even if it goes through GPIOA, it needs to have its clock enabled as well
  // It is amusing to note that GPIOA and UART4 do not live in the same bus
  RCC_APB1ENR1 |= (1 << 19);
}
void setup_BRR(void) {
  UART4_BRR = 0x2B6;
}
void UART_message_length(int l) {
  // This is a bit weird in the sense that we need to edit two bits M1 and M0, but they are not contiguous
  if (l == 8) {
    UART4_CR1 &= ~(1 << 28);
    UART4_CR1 &= ~(1 << 12);
  }
  else if (l == 9) {
    UART4_CR1 &= ~(1 << 28);
    UART4_CR1 |= (1 << 12);
  }
  else if (l == 7) {
    UART4_CR1 |= (1 << 28);
    UART4_CR1 &= ~(1 << 12);
  }
}
void UART_init(void) {
  /* GPIO config */

  // We now choose the appropriate Alternate Function for pin A0
  // We blank bits 0 to 3
  GPIOA_AFRL &= ~(0xF);
  // From the Datasheet we know we need to write AF8 which is 1000
  GPIOA_AFRL |= (0x8);
  // We now configure pin A0 to alternate function mode 
  GPIOA_MODER &= ~(0x3);
  GPIOA_MODER |= (0x2);
  // OTYPER to push-pull
  GPIOA_OTYPER &= ~(1);
  // OSPEED to medium speed
  GPIOA_OSPEEDR &= ~(0x3);
  GPIOA_OSPEEDR |= (0x1);
  // no pull
  GPIOA_PUPDR &= ~(0x3);
  GPIOA_PUPDR |= (0x0);

  /* UART peripheral registers config */

  // Now we enable the UART4 peripheral
  UART4_CR1 |= (1 << 0);
  // This is the TE bit, it enables transmission
  UART4_CR1 |= (1 << 3);
}
void initiate_transmission(void) {
  // Wait until TXE == 1 meaning we can write to TDR
  while (!(UART4_ISR & (1 << 7))) {
    ;
  }
  // Never modify individual bits in this register
  UART4_TDR = 'a';
}
