#ifndef UART_HEADER
#define UART_HEADER


#include <stdint.h>


/*
 * Register defines
 */

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


/*
 * Function Prototypes
 */

void UART4_enable_clock(void);
void setup_BRR(void);
void UART_message_length(int l);
void UART_init(void);
void send_char(char c);
void send_string(char *c);


#endif
