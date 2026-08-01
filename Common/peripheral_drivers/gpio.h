#pragma once

#include "bitwise_functions.h"
#include <stdint.h>

namespace dd {

enum class gpio_pin : uint32_t {
	P0=0,   P1=1,   P2=2,   P3=3,
	P4=4,   P5=5,   P6=6,   P7=7,
	P8=8,   P9=9,   P10=10, P11=11,
	P12=12, P13=13, P14=14, P15=15
};

enum class gpio_pin_af {
	AF0=0,   AF1=1,   AF2=2,   AF3=3,
	AF4=4,   AF5=5,   AF6=6,   AF7=7,
	AF8=8,   AF9=9,   AF10=10, AF11=11,
	AF12=12, AF13=13, AF14=14, AF15=15
};

enum class gpio_pin_mode {
	In=0, Out=1, AF=2, Analog=3
};

enum class gpio_pin_otype {
	PushPull=0, OpenDrain=1
};

enum class gpio_pin_pull {
	None=0, Up=1, Down=2
};

enum class gpio_pin_ospeed : uint32_t {
	Low=0, Medium=1, High=2, VeryHigh=3
};

struct gpio_def {
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

	//Configures the pin mode for a gpio pin
	void conf_pin_mode(gpio_pin pin, gpio_pin_mode mode);

	//Configures the pin output type for a gpio pin
	void conf_pin_otype(gpio_pin pin, gpio_pin_otype otype);

	//Configures the pin pull for a gpio pin
	void conf_pin_pull(gpio_pin pin, gpio_pin_pull pull);

	//Configures the pin alt func for a gpio pin
	void conf_pin_af(gpio_pin pin, gpio_pin_af af);

	//Configures the pin output speed for a gpio pin
	void conf_pin_ospeed(gpio_pin pin, gpio_pin_ospeed ospeed);

	//Sets the value of the pin to 1
	void set_pin(gpio_pin pin);

	//Sets the value of the pin to 0
	void reset_pin(gpio_pin pin);

	//Returns the state of the pin
	bool read_pin(gpio_pin pin);

	//Writes the given state, 1 or 0, to the pin.
	void write_pin(gpio_pin pin, bool state);
};

#define GPIO_BASE		(0x58020000)
#define GPIO_OFFSET		(0x00000400)

#define GPIOA			(*(dd::gpio_def *) (GPIO_BASE + 0*GPIO_OFFSET))
#define GPIOB			(*(dd::gpio_def *) (GPIO_BASE + 1*GPIO_OFFSET))
#define GPIOC			(*(dd::gpio_def *) (GPIO_BASE + 2*GPIO_OFFSET))
#define GPIOD			(*(dd::gpio_def *) (GPIO_BASE + 3*GPIO_OFFSET))
#define GPIOE			(*(dd::gpio_def *) (GPIO_BASE + 4*GPIO_OFFSET))
#define GPIOF			(*(dd::gpio_def *) (GPIO_BASE + 5*GPIO_OFFSET))
#define GPIOG			(*(dd::gpio_def *) (GPIO_BASE + 6*GPIO_OFFSET))
#define GPIOH			(*(dd::gpio_def *) (GPIO_BASE + 7*GPIO_OFFSET))
#define GPIOI			(*(dd::gpio_def *) (GPIO_BASE + 8*GPIO_OFFSET))
#define GPIOJ			(*(dd::gpio_def *) (GPIO_BASE + 9*GPIO_OFFSET))
#define GPIOK			(*(dd::gpio_def *) (GPIO_BASE + 10*GPIO_OFFSET))

};

