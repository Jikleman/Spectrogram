#pragma once

#include <stdint.h>

#define GPIO_Base		(0x58020000)
#define GPIO_Offset		(0x400)

struct GPIOStruct{
	volatile uint32_t MODER;		// Mode
	volatile uint32_t OTYPER;		// Output type
	volatile uint32_t OSPEEDR;		// Output speed
	volatile uint32_t PUPDR;		// Pull-up/Pull-down
	volatile uint32_t IDR;			// Input data
	volatile uint32_t ODR;			// Output data
	volatile uint32_t BSRR;			// bit set/reset
	volatile uint32_t LCKR;			// Configuration lock
	volatile uint32_t AFRL;			// Alternate function low
	volatile uint32_t AFRH;			// Alternate function high
};

#define GPIOA			((GPIOStruct *) (GPIO_Base + 0*GPIO_Offset))
#define GPIOB			((GPIOStruct *) (GPIO_Base + 1*GPIO_Offset))
#define GPIOC			((GPIOStruct *) (GPIO_Base + 2*GPIO_Offset))
#define GPIOD			((GPIOStruct *) (GPIO_Base + 3*GPIO_Offset))
#define GPIOE			((GPIOStruct *) (GPIO_Base + 4*GPIO_Offset))
#define GPIOF			((GPIOStruct *) (GPIO_Base + 5*GPIO_Offset))
#define GPIOG			((GPIOStruct *) (GPIO_Base + 6*GPIO_Offset))
#define GPIOH			((GPIOStruct *) (GPIO_Base + 7*GPIO_Offset))
#define GPIOI			((GPIOStruct *) (GPIO_Base + 8*GPIO_Offset))
#define GPIOJ			((GPIOStruct *) (GPIO_Base + 9*GPIO_Offset))
#define GPIOK			((GPIOStruct *) (GPIO_Base + 10*GPIO_Offset))

enum class GPIO_Pin{
	P0,
	P1,
	P2,
	P3,
	P4,
	P5,
	P6,
	P7,
	P8,
	P9,
	P10,
	P11,
	P12,
	P13,
	P14,
	P15
};
enum class GPIO_AltFunc{
	AF0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15
};
enum class GPIO_PinMode{
	Input = 0,
	Output = 1,
	AltFunc = 2,
	Analog = 3
};
enum class GPIO_OutputType{
	PushPull = 0,
	OpenDrain = 1
};
enum class GPIO_PinPull{
	None = 0,
	Up = 1,
	Down = 2
};
enum class GPIO_OutputSpeed{
	Low = 0,
	Medium = 1,
	High = 2,
	VeryHigh = 3
};

void gpio_setPinMode(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_PinMode mode);
void gpio_setPinOutputType(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_OutputType oType);
void gpio_setPinPull(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_PinPull pull);
void gpio_setPinAltFunc(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_AltFunc AF);
void gpio_setPinOutputSpeed(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_OutputSpeed ospeed);

struct GPIO_PinCFG{
	GPIO_PinMode mode;
	GPIO_OutputType otype;
	GPIO_PinPull pull;
	GPIO_AltFunc AF;
	GPIO_OutputSpeed ospeed;
};

void gpio_configPin(GPIOStruct* GPIOx, GPIO_Pin pin, GPIO_PinCFG& cfg);

bool inline gpio_readPin(GPIOStruct* GPIOx, GPIO_Pin pin){
	return (GPIOx->IDR & (1<<(uint32_t)pin));
}

void inline gpio_writePin(GPIOStruct* GPIOx, GPIO_Pin pin, bool data){
	if (data){
		GPIOx->ODR |= (1<<(uint32_t)pin);
		return;
	}
	GPIOx->ODR &= ~(1<<(uint32_t)pin);
}

void inline gpio_setPin(GPIOStruct* GPIOx, GPIO_Pin pin){
	GPIOx->BSRR = (1<<(uint32_t)pin);
}

void inline gpio_resetPin(GPIOStruct* GPIOx, GPIO_Pin pin){
	GPIOx->BSRR = (1<<((uint32_t)pin + 16));
}

