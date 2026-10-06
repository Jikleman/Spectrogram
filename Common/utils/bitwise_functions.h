#pragma once

#include <stdint.h>

/**
 * Writes to the register a 1 at the particular position.
 *
 * @param reg
 * @param position
 */
inline void set_bit_nomask(volatile uint32_t& reg, uint32_t position){
	reg = (1u << position);
}

/**
 * Masks a 1 to a particular bit in a register.
 *
 * @param reg A register to write a bit to.
 * @param position The position of the bit to write a 1 to.
 */
inline void set_bit(volatile uint32_t& reg, uint32_t position){
	reg |= (1u << position);
}

/**
 * Masks a 0 to a particular bit in a register.
 *
 * @param reg A register to write a bit to.
 * @param position The position of the bit to write a 0 to.
 */
inline void reset_bit(volatile uint32_t& reg, uint32_t position){
	reg &= ~(1u << position);
}

/**
 * Returns the value of a particular bit in a register.
 *
 * @param reg A register to read a bit from.
 * @param position The position of the bit to read from in the register.
 * @return The value of the bit to be read.
 */
inline bool read_bit(volatile uint32_t& reg, uint32_t position){
	return reg & (1u<<position);
}

/**
 * Masks a 0 or a 1 to a bit in a register.
 *
 * @param reg A register to write a bit to.
 * @param position The position of the bit to write in the register.
 * @param data The value to write to the bit.
 */
inline void write_bit(volatile uint32_t& reg, uint32_t position, bool data){
	data ? set_bit(reg, position) : reset_bit(reg, position);
}

/**
 * Rewrites just a sequence of bits at a given position in a register.
 *
 * @param reg A register to write the bits to.
 * @param position A position to offset the data to.
 * @param bits The data to write into the register.
 * @param num_bits The sequence length from the data, from bit 0 to bit n, to write into the register.
 */
inline void write_bits(volatile uint32_t& reg, uint32_t position, uint32_t data, uint32_t num_bits){
	const uint32_t mask = ((1u<<num_bits) - 1) << position;
	reg = (reg & ~mask) | ((data<<position) & mask);
}

/**
 * Writes to the register a sequence of bits at a given position.
 *
 * @param reg A register to write the bits to.
 * @param position A position to offset the data to.
 * @param bits The data to write into the register.
 * @param num_bits The sequence length from the data, from bit 0 to bit n, to write into the register.
 */
inline void write_bits_nomask(volatile uint32_t& reg, uint32_t position, uint32_t data, uint32_t num_bits){
	const uint32_t data_mask = ((1u<<num_bits) - 1) << position;
	reg = ((data<<position) & data_mask);
}

/**
 * Reads a sequence of bits from a register.
 * The selected bits are shifted so the first bit of the sequence is returned at bit position 0.
 *
 * @param reg A register to read the bits from.
 * @param position The starting position to read bits from.
 * @param num_bits The number of bits to read from the register.
 * @return The selected bits, right-aligned starting from position 0.
 */
inline uint32_t read_bits(volatile uint32_t& reg, uint32_t position, uint32_t num_bits){
	const uint32_t mask = ((1u<<num_bits) - 1);
	return (reg>>position) & mask;
}
