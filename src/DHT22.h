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
  // Pull-up
  GPIOB_PUPDR &= ~(0x30);
  GPIOB_PUPDR |= (0x10);
}
// void setup_GPIOB_DHT22_input(void) {
//   // First we need to activate the clock
//   init_GPIOB();
//
//   // WARNING This is copy pasted from elsewhere, I need to configure properly
//   GPIOA_MODER &= ~(0x3);
//   GPIOA_MODER |= (0x2);
//   // OTYPER to push-pull
//   GPIOA_OTYPER &= ~(1);
//   // OSPEED to medium speed
//   GPIOA_OSPEEDR &= ~(0x3);
//   GPIOA_OSPEEDR |= (0x1);
//   // no pull
//   GPIOA_PUPDR &= ~(0x3);
//   GPIOA_PUPDR |= (0x0);
// }

