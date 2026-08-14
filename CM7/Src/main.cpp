#include <peripheral_drivers/gpio.h>
#include <peripheral_drivers/rcc.h>

#include <stdint.h>

volatile bool x = true;

int main(void)
{
	x = RCC.is_peripheral_allocated(AHB4::gpioa);
	RCC.allocate_peripheral(AHB4::gpiob);
	x = RCC.is_peripheral_allocated(AHB4::gpiob);
    /* Loop forever */
	for(;;);
}
