/* Global struct definitions */

// bus clock frequency prescalers
struct prescalers {
  int AHB;
  int APB1;
  int APB2;
};
// bus clock frequencies
struct clock_freq {
  int SYSCLK;
  int HCLK;
  int PCLK1;
  int PCLK2;
};
