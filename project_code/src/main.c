#include <stdint.h>
// My headers
#include "SysTick.h"
#include "blink_LED.h"

int main() {
  while (1) {
    toggle_LED_on_off();
  }
  return 0;
}
