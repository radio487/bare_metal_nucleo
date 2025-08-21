#define FLASH_ACR (*(volatile uint32_t*)(0x40022000))

//max_freq is in MHz
void setup_FLASH_latency(int max_freq) {
  if (0 < max_freq && max_freq <= 24) {
    FLASH_ACR &= ~(1 << 2);
    FLASH_ACR &= ~(1 << 1);
    FLASH_ACR &= ~(1 << 0);
  }
  else if (24 < max_freq && max_freq <= 48) {
    FLASH_ACR &= ~(1 << 2);
    FLASH_ACR &= ~(1 << 1);
    FLASH_ACR |= (1 << 0);
  }
  else if (48 < max_freq && max_freq <= 72) {
    FLASH_ACR &= ~(1 << 2);
    FLASH_ACR |= (1 << 1);
    FLASH_ACR &= ~(1 << 0);
  }
}
