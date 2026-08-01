#include <peripheral_drivers/gpio.h>
#include <peripheral_drivers/rcc.h>

#include <stdint.h>

int main(void)
{
	GPIOA.reset_pin(dd::gpio_pin::P0);
    /* Loop forever */
	for(;;);
}
