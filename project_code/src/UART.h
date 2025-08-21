#define USART1_base 0x40013800
// The SR Statust Register is read only
#define USART1_SR (*(volatile uint32_t*)(USART1_base + 0x00))
// the emitted information goes here
#define USART1_DR (*(volatile uint32_t*)(USART1_base + 0x04))
// Baud Rate Register
#define USART1_BRR (*(volatile uint32_t*)(USART1_base + 0x08))
