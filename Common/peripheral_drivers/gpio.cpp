#include "gpio.h"
#include "bitwise_functions.h"

//Configures the pin mode for a gpio pin
void dd::gpio_def::conf_pin_mode(gpio_pin pin, gpio_pin_mode mode){
	mask_bits(MODER, (uint32_t) pin * 2, (uint32_t) mode, 2);
}

//Configures the pin output type for a gpio pin
void dd::gpio_def::conf_pin_otype(gpio_pin pin, gpio_pin_otype otype){
	write_bit(OTYPER, (uint32_t) pin, (uint32_t) otype);
}

//Configures the pin pull for a gpio pin
void dd::gpio_def::conf_pin_pull(gpio_pin pin, gpio_pin_pull pull){
	mask_bits(PUPDR, (uint32_t) pin * 2, (uint32_t) pull, 2);
}

//Configures the pin alt func for a gpio pin
void dd::gpio_def::conf_pin_af(gpio_pin pin, gpio_pin_af af){
	if ((uint32_t) pin / 8 == 0){
		const uint32_t shift = (uint32_t) pin * 4;
		mask_bits(AFRL, shift, (uint32_t) af, 4);
	} else {
		const uint32_t shift = ((uint32_t) pin - 8) * 4;
		mask_bits(AFRH, shift, (uint32_t) af, 4);
	}
}

//Configures the pin output speed for a gpio pin
void dd::gpio_def::conf_pin_ospeed(gpio_pin pin, gpio_pin_ospeed ospeed){
	mask_bits(OSPEEDR, (uint32_t) pin * 2, (uint32_t) ospeed, 2);
}

//Sets the value of the pin to 1
void dd::gpio_def::set_pin(gpio_pin pin){
	set_bit_nomask(BSRR, (uint32_t) pin);
}

//Sets the value of the pin to 0
void dd::gpio_def::reset_pin(gpio_pin pin){
	set_bit_nomask(BSRR, (uint32_t) pin + 16);
}

//Returns the state of the pin
bool dd::gpio_def::read_pin(gpio_pin pin){
	return read_bit(IDR, (uint32_t) pin);
}

//Writes the given state, 1 or 0, to the pin.
void dd::gpio_def::write_pin(gpio_pin pin, bool state){
	write_bit(ODR, (uint32_t) pin, state);
}
