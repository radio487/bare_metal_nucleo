// flash.h
void set_latency(int lat);
// RCC.h
void enable_HSI_clock(void);
void enable_HSE_clock(void);
void PLL_source(int b);
void set_PLLN(int f);
void set_PLLM(int f);
void set_PLLR(int d);
void enable_PLLR(void);
void enable_PLL(void);
void switch_SYSCLK(int b);
void set_max_freq(struct clock_freq *f, struct prescalers *pre);
// GPIO.h
void init_GPIOA(void);
void init_GPIOB(void);
// SysTick.h
void enable_SysTick(void);
// TIM2.h
void init_clock_TIM2();
int TIM2_freq(struct prescalers *p, int PCLK1);
void TIM2_counter_enable(void);
void TIM2_direction(char c);
void TIM2_set_counter_value(uint32_t cnt);
void TIM2_set_one_pulse_mode();
void TIM2_delay(int t, int f_TIM2);
// blink_LED.h
void idle_sec(int freq);
void LED2_init(void);
// UART.h
void UART4_enable_clock(void);
void setup_BRR(void);
void UART_message_length(int l);
void UART_init(void);
void send_char(char c);
void send_string(char *c);
// DHT22.h
void setup_GPIOB_DHT22_output(void);
void setup_GPIOB_DHT22_input(void);
// interrupts.h
void TIM2_IRQHandler(void);
