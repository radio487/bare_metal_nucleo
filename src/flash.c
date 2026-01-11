#include <stdint.h>
#include "../include/flash.h"


// bits 0, 1 and 2
// we write lat in binary to these bits
void set_latency(int lat) {
  FLASH_ACR &= ~(0x7);
  FLASH_ACR |= (lat);
}
