// The communication protocol of the DHT 22 requires one to alternate the GPIO between open drain output, and input modes. This is because at some moments during the protocol the micro needs to be the one driving the line low, and at other the sensor needs to be the one driving the line low

// The communication protocol starts with the microcontroller "pinging"
void setup_GPIOB_DHT22_output(void) {
  // First we need to activate the clock
  init_GPIOB();
}
void setup_GPIOB_DHT22_input(void) {
  // First we need to activate the clock
  init_GPIOB();
  
  // WARNING This is copy pasted from elsewhere, I need to configure properly
  GPIOA_MODER &= ~(0x3);
  GPIOA_MODER |= (0x2);
  // OTYPER to push-pull
  GPIOA_OTYPER &= ~(1);
  // OSPEED to medium speed
  GPIOA_OSPEEDR &= ~(0x3);
  GPIOA_OSPEEDR |= (0x1);
  // no pull
  GPIOA_PUPDR &= ~(0x3);
  GPIOA_PUPDR |= (0x0);
}

