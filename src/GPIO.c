#include <stdint.h>
#include "../include/RCC.h"
#include "../include/GPIO.h"


// Enables the clock to GPIOA. It does nothing if it is already set
void init_GPIOA(void) {
  // GPIOA lives in the AHB2 bus
  if (RCC_AHB2ENR & 0x1) {
    ;
  }
  else {
    RCC_AHB2ENR |= (1 << 0);
    // Making sure we wait till the bit shifts
    while (!(RCC_AHB2ENR & 0x1)) {
      ;
    }
  }
  return;
}
// Enables the clock to GPIOB. It does nothing if it is already set
void init_GPIOB(void) {
  // GPIOA lives in the AHB2 bus
  if (RCC_AHB2ENR & 0x2) {
    ;
  }
  else {
    RCC_AHB2ENR |= (1 << 1);
    // Making sure we wait till the bit shifts
    while (!(RCC_AHB2ENR & 0x2)) {
      ;
    }
  }
  return;
}
