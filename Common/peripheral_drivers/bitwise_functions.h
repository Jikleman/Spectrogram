#pragma once

#include <stdint.h>

//Writes a 1 to a certain position in a register. All other bits are 0s
inline void set_bit_nomask(volatile uint32_t& reg, uint32_t position){
	reg = (1u << position);
}

//Masks a 1 to a certain position in a register
inline void set_bit(volatile uint32_t& reg, uint32_t position){
	reg |= (1u << position);
}

//Masks a 0 to a certain position in a register
inline void reset_bit(volatile uint32_t& reg, uint32_t position){
	reg &= ~(1u << position);
}

//Returns if the bit at the position is true or false.
inline bool read_bit(volatile uint32_t& reg, uint32_t position){
	return reg & (1u<<position);
}

//Only writes the bit if the bit at the position isn't already the desired value.
inline void write_bit(volatile uint32_t& reg, uint32_t position, bool state){
	state ? set_bit(reg, position) : reset_bit(reg, position);
}

//Masks in a group of bits
inline void mask_bits(volatile uint32_t& reg, uint32_t position, uint32_t bits, uint32_t num_bits){
	const uint32_t mask = ((1u<<num_bits) - 1) << position;
	reg = (reg & ~mask) | ((bits<<position) & mask);
}
