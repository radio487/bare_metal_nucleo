#include <stdint.h>
// My headers
#include "SysTick.h"
#include "blink_LED.h"

int main() {
  setup_LED();
  enable_SysTick();
  while (1) {
     ;
  }
  return 0;
}
