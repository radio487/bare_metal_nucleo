#define GPIOA_base 0x40010800
// GPIOA from 0 to 7
#define GPIOA_CRL (*(volatile uint32_t*)(GPIOA_base + 0x00))
// GPIOA from 8 to 15. LED is 13.
#define GPIOA_CRH (*(volatile uint32_t*)(GPIOA_base + 0x04))
#define GPIOA_ODR (*(volatile uint32_t*)(GPIOA_base + 0x0C))

#define GPIOC_base 0x40011000
// GPIOC from 0 to 7
#define GPIOC_CRL (*(volatile uint32_t*)(GPIOC_base + 0x00))
// GPIOC from 8 to 15. LED is 13.
#define GPIOC_CRH (*(volatile uint32_t*)(GPIOC_base + 0x04))
#define GPIOC_ODR (*(volatile uint32_t*)(GPIOC_base + 0x0C))
