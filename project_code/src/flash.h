
#define FLASH_registers_base 0x40022000
#define FLASH_ACR (*(volatile uint32_t*)(FLASH_registers_base + 0x00))

// bits 0, 1 and 2
// we write lat in binary to these bits
void set_latency(int lat) {
  FLASH_ACR &= ~(0x7);
  FLASH_ACR |= (lat);
}
