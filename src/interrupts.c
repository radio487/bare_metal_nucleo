#include "../include/UART.h"
#include "../include/interrupts.h"


void TIM2_IRQHandler(void) {
  char s2[] = "End";
  send_string(s2);
}
