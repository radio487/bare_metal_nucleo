// The communication protocol of the DHT 22 requires one to alternate the GPIO between open drain output, and input modes. This is because at some moments during the protocol the micro needs to be the one driving the line low, and at other the sensor needs to be the one driving the line low

// The communication protocol starts with the microcontroller "pinging" the sensor by driving the data bus low
// Thus we need to configure the GPIO as output
void setup_GPIOB_DHT22_output(void) {
  // First we need to activate the clock
  init_GPIOB();
  // General purpose output mode to pin 2 in GPIOB so bits 4 and 5 in the register
  GPIOB_MODER &= ~(0x30);
  GPIOB_MODER |= (0x10);
  // Output open-drain
  GPIOB_OTYPER |= (0x4);
  // We switch it to very high speed because why not
  GPIOB_OSPEEDR &= ~(0x30);
  GPIOB_OSPEEDR |= (0x30);
  // No pull-up nor pull-down
  GPIOB_PUPDR &= ~(0x30);
  // This step is not necessary strictly speaking as we blank the bits for this setting, but in general we would need it so I leave it for now
  GPIOB_PUPDR |= (0x00);
}
// For when we need to let the sensor drive the Data bus low we need to configure an input mode
void setup_GPIOB_DHT22_input(void) {
  // First we need to activate the clock
  init_GPIOB();

  // Input mode
  GPIOA_MODER &= ~(0x30);
  GPIOA_MODER |= (0x00);
  // We switch it to very high speed because why not
  GPIOB_OSPEEDR &= ~(0x30);
  GPIOB_OSPEEDR |= (0x30);
  // Pull up
  GPIOA_PUPDR &= ~(0x30);
  GPIOB_PUPDR |= (0x10);
}
