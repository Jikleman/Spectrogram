#include "gpio.h"

//Configures the pin mode of a pin.
void gpio_setPinMode(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_PinMode mode){
	uint32_t shiftAmount = (uint32_t)pin * 2;
	GPIOx->MODER &= ~(0x3<<shiftAmount);
	GPIOx->MODER |= ((uint32_t)mode<<shiftAmount);
}

//Configures the output type of a pin.
void gpio_setPinOutputType(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_OutputType otype){
	GPIOx->OTYPER &= ~(1<<(uint32_t)pin);
	GPIOx->OTYPER |= ((uint32_t)otype<<(uint32_t)pin);
}

//Configures the pull resistor for a pin.
void gpio_setPinPull(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_PinPull pull){
	uint32_t shiftAmount = (uint32_t)pin * 2;
	GPIOx->PUPDR &= ~(0x3<<shiftAmount);
	GPIOx->PUPDR |= ((uint32_t)pull<<shiftAmount);
}

//Sets the alternate function for a pin.
void gpio_setPinAltFunc(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_AltFunc AF){
	if ((uint32_t)pin/7 == 0){		//Low AF Register
		uint32_t shiftAmount = ((uint32_t)pin * 4) - 32;
		GPIOx->AFRL &= ~(0xF<<shiftAmount);
		GPIOx->AFRL |= ((uint32_t)AF<<shiftAmount);
	} else {						//High AF Register
		uint32_t shiftAmount = ((uint32_t)pin * 4) - 32;
		GPIOx->AFRH &= ~(0xF<<shiftAmount);
		GPIOx->AFRH |= ((uint32_t)AF<<shiftAmount);
	}
}

//Sets the output speed for a pin.
void gpio_setPinOutputSpeed(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_OutputSpeed ospeed){
	uint32_t shiftAmount = (uint32_t)pin * 2;
	GPIOx->OSPEEDR &= ~(0x3<<shiftAmount);
	GPIOx->OSPEEDR |= ((uint32_t)ospeed<<shiftAmount);
}


void gpio_configPin(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_PinCFG& cfg){
	gpio_setPinMode(GPIOx, pin, cfg.mode);
	gpio_setPinOutputType(GPIOx, pin, cfg.otype);
	gpio_setPinPull(GPIOx, pin, cfg.pull);
	gpio_setPinAltFunc(GPIOx,  pin, cfg.AF);
	gpio_setPinOutputSpeed(GPIOx, pin, cfg.ospeed);
}
