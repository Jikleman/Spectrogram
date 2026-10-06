#include <peripheral_drivers/gpio.h>
#include <peripheral_drivers/rcc.h>
#include <peripheral_drivers/spi.h>

#include <stdint.h>

using namespace dd;

volatile bool x = true;

int main(void)
{
	RCC.allocate_peripheral(AHB4::gpioa);
	RCC.allocate_peripheral(APB1L::spi3);

    /* Loop forever */
	for(;;);
}
