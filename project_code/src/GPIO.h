#define GPIOA_base 0x48000000
// GPIOA from 0 to 7
#define GPIOA_MODER (*(volatile uint32_t*)(GPIOA_base + 0x00))
#define GPIOA_OTYPER (*(volatile uint32_t*)(GPIOA_base + 0x04))
#define GPIOA_OSPEEDR (*(volatile uint32_t*)(GPIOA_base + 0x08))
#define GPIOA_PUPDR (*(volatile uint32_t*)(GPIOA_base + 0x0C))
#define GPIOA_IDR (*(volatile uint32_t*)(GPIOA_base + 0x10))
#define GPIOA_ODR (*(volatile uint32_t*)(GPIOA_base + 0x14))
#define GPIOA_BSRR (*(volatile uint32_t*)(GPIOA_base + 0x18))
#define GPIOA_LCKR (*(volatile uint32_t*)(GPIOA_base + 0x1C))
#define GPIOA_AFRL (*(volatile uint32_t*)(GPIOA_base + 0x20))
#define GPIOA_AFRH (*(volatile uint32_t*)(GPIOA_base + 0x24))
#define GPIOA_BRR (*(volatile uint32_t*)(GPIOA_base + 0x28))
#define GPIOA_ASCR (*(volatile uint32_t*)(GPIOA_base + 0x2C))

#define GPIOB_base 0x48000400
#define GPIOC_base 0x48000800
#define GPIOD_base 0x48000C00
#define GPIOE_base 0x48001000
#define GPIOF_base 0x48001400
#define GPIOG_base 0x48001800
#define GPIOH_base 0x48001C00
