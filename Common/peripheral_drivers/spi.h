#pragma once

//SPI 1-3 fifo can hold 16 bytes, max data and crc is 32 bits
//SPI 4-6 fifo can hold 8 bytes, max data and crc size is 16 bits
#include <stdint.h>

namespace dd {

enum class spi_crc_pattern {
	all_ones = 0,
	all_zeroes = 1
};

enum class spi_direction {
	receiver = 0,
	transmitter = 1,
};

enum class spi_baud_presc {
	div2 = 0,
	div4 = 1,
	div8 = 2,
	div16 = 3,
	div32 = 4,
	div64 = 5,
	div128 = 6,
	div256 = 7
};

enum class spi_frame_size {
	frame_4bit = 3, frame_5bit  = 4, frame_6bit  = 5, frame_7bit  = 6, frame_8bit  = 7,
	frame_9bit  = 8, frame_10bit = 9, frame_11bit = 10, frame_12bit = 11, frame_13bit = 12,
	frame_14bit = 13, frame_15bit = 14, frame_16bit = 15, frame_17bit = 16,
	frame_18bit = 17, frame_19bit = 18, frame_20bit = 19, frame_21bit = 20, frame_22bit = 21,
	frame_23bit = 22, frame_24bit = 23, frame_25bit = 24, frame_26bit = 25, frame_27bit = 26,
	frame_28bit = 27, frame_29bit = 28, frame_30bit = 29, frame_31bit = 30, frame_32bit = 31,
};

enum class spi_udr_detection {
	data_frame_begin = 0,
	data_frame_end = 1,
	active_ss_begin = 2,
};

enum class spi_udr_behavior {
	constant_pattern = 0,
	last_rx_data_frame = 1,
	last_tx_data_frame = 2,
};

enum class spi_fifo_threshold {
	size_1frame = 0, size_2frames = 1, size_3frames = 2, size_4frames = 3,
	size_5frames = 4, size_6frames = 5, size_7frames = 6, size_8frames = 7,
	size_9frames = 8, size_10frames = 9, size_11frames = 10, size_12frames = 11,
	size_13frames = 12, size_14frames = 13, size_15frames = 14, size_16frames = 15
};

enum class spi_ss_out_management {
	active_until_tx_done = 0,
	interleave_between_data_frames = 1,
};

enum class spi_ss_input_management {
	ss_input_is_SS_PAD = 0,
	ss_input_is_SSI = 1,
};

enum class spi_cpol {
	idle_low = 0,
	idle_high = 1,
};

enum class spi_cpha {
	rising_edge = 0,
	falling_edge = 1,
};

enum class spi_data_frame_format {
	tx_msb_first = 0,
	tx_lsb_first = 1,
};

enum class spi_mode {
	slave = 0,
	master = 1,
};

enum class spi_protocol {
	motorola = 0,
	ti = 1,
};

enum class spi_comm_mode {
	full_duplex = 0,
	simplex_tx = 1,
	simplex_rx = 2,
	half_duplex = 3,
};

enum class spi_idle_delay {
	no_delay = 0, one_cycle = 1, two_cycles = 2, three_cycles = 3,
	four_cycles = 4, five_cycles = 5, six_cycles = 6, seven_cycles = 7,
	eight_cycles = 8, nine_cycles = 9, ten_cycles = 10, eleven_cycles = 11,
	twelve_cycles = 12, thirteen_cycles = 13, fourteen_cycles = 14, fifteen_cycles = 15,

};

enum class spi_RxFIFO_packing_lvl {
	no_frames=0,
	one_frames=1,
	two_frames=2,
	three_frames=3,
};

enum class spi_data_alignment {
	right=0,
	left=1,
};

enum class spi_clock_polarity {
	tx_fall_rx_rise=0,
	tx_rise_rx_fall=1,
};

enum class spi_ch_length {
	length_16_bit=0,
	length_32_bit=1,
};

enum class spi_data_length {
	length_16_bit=0,
	length_24_bit=1,
	length_32_bit=2,
};

enum class i2s_standard {
	i2s_phillips=0,
	msb_justified=1,
	lsb_justified=2,
	pcm=3,
};

enum class i2s_sync {
	short_frame_sync=0,
	long_frame_sync=1,
};

enum class i2s_mode {
	slave_tx=0,
	slave_rx=1,
	master_tx=2,
	master_rx=3,
	slave_full_duplex=4,
	master_full_duplex=5,
};

struct spi_def {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CFG1;
    volatile uint32_t CFG2;
    volatile uint32_t IER;
    volatile uint32_t SR;
    volatile uint32_t IFCR;
    volatile uint32_t Reserved0;
    volatile uint32_t TXDR;
    volatile uint32_t Reserved1[3];
    volatile uint32_t RXDR;
    volatile uint32_t Reserved2[3];
    volatile uint32_t CRCPOLY;
    volatile uint32_t TXCRC;
    volatile uint32_t RXCRC;
    volatile uint32_t UDRDR;
    volatile uint32_t I2SCFGR;

    /*****
     * Control register 1
     */

    //Locks AF of related IOs
    void lock_io_conf();
    void unlock_io_conf();
    bool is_io_locked();

    void set_tx_crc_init_pattern(spi_crc_pattern pattern);
    void set_rx_crc_init_pattern(spi_crc_pattern pattern);
    spi_crc_pattern get_tx_crc_init_pattern();
    spi_crc_pattern get_rx_crc_init_pattern();

    void use_full_crc_poly();
    void dont_use_full_crc_poly();
    bool is_using_full_crc_poly();

    void set_internal_ss();
    void reset_internal_ss();
    bool is_internal_ss_active();

    void set_half_duplex_dir(spi_direction direction);
    spi_direction get_half_duplex_dir();

    void master_request_suspend_tx();
    void master_start_tx();
    bool is_master_tx_idle();

    void enable_master_rx_auto_suspend();
    void disable_master_rx_auto_suspend();
    bool is_master_rx_auto_suspend_enabled();

    void enable_spi_transfer();
    void disable_spi_transfer();
    bool is_spi_enabled();

    /*****
     * Control register 2
     */

    void set_next_tx_data_frame_amount(uint16_t amount);
    uint16_t get_next_tx_data_frame_amount();

    void set_current_tx_data_frame_amount(uint16_t amount);
    uint16_t get_current_tx_data_frame_amount();

    /*****
     * Configuration register 1
     */

    void set_baud_rate_prescale(spi_baud_presc prescale);
    spi_baud_presc get_baud_rate_prescale();

    void disable_crc();
    void enable_crc();
    bool is_crc_enabled();

    void set_crc_frame_bit_size(spi_frame_size size);
    spi_frame_size get_crc_frame_bit_size();

    void enable_tx_dma();
    void disable_tx_dma();
    bool is_tx_dma_enabled();

    void enable_rx_dma();
    void disable_rx_dma();
    bool is_rx_dma_enabled();

    //Configure when an underrun is detected when acting as slave tx
    void set_slave_tx_udr_detection(spi_udr_detection det);
    spi_udr_detection get_slave_tx_udr_detection();

    //Configure underrun behavior when acting as slave tx
    void set_slave_tx_udr_behavior(spi_udr_behavior behavior);
    spi_udr_behavior get_slave_tx_udr_behavior();

    //This is also setting the number of data frames in a single data packet. Should not be more than 1/2 FIFO space.
    void set_fifo_threshold(spi_fifo_threshold level);
    spi_fifo_threshold get_fifo_threshold();

    void set_data_frame_bit_size(spi_frame_size size);
    spi_frame_size get_data_frame_bit_size();

    /*****
     * Configuration register 2
     */

    void always_keep_gpio_control();
    void no_control_of_gpio();
    bool is_keep_gpio_control();

    void set_master_ss_out_management(spi_ss_out_management out_manage);
    spi_ss_out_management get_master_ss_out_management();

    void enable_ss_out();
    void disable_ss_out();
    bool is_ss_enabled();

    void set_ss_polarity_high();
    void set_ss_polarity_low();
    bool is_ss_polarity_high();

    void set_ss_input_software_management(spi_ss_input_management in_manage);
    spi_ss_input_management get_ss_input_software_management();

    void set_clock_polarity(spi_cpol polarity);
    spi_cpol get_clock_polarity();

    void set_clock_phase(spi_cpha phase);
    spi_cpha get_clock_phase();

    void set_data_frame_format(spi_data_frame_format format);
    spi_data_frame_format get_data_frame_format();

    void set_spi_mode(spi_mode mode);
    spi_mode get_spi_mode();

    void set_protocol(spi_protocol protocol);
    spi_protocol get_protocol();

    void set_comm_mode(spi_comm_mode mode);
    spi_comm_mode get_comm_mode();

    void swap_miso_mosi();
    void unswap_miso_mosi();
    bool is_miso_mosi_swapped();

    void set_master_inter_data_idleness(spi_idle_delay delay);
    spi_idle_delay get_master_inter_data_idleness();

    void set_master_ss_idleness(spi_idle_delay delay);
    spi_idle_delay get_master_ss_idleness();

    /*****
     * Interrupt register
     */

    void enable_inter_TSER();
    void enable_inter_MODF();
    void enable_inter_CRCE();
    void enable_inter_OVR();
    void enable_inter_UDR();
    void enable_inter_TXTF();
    void enable_inter_EOT();
    void enable_inter_DXP();
    void enable_inter_TXP();
    void enable_inter_RXP();

    void disable_inter_TSER();
    void disable_inter_MODF();
    void disable_inter_CRCE();
    void disable_inter_OVR();
    void disable_inter_UDR();
    void disable_inter_TXTF();
    void disable_inter_EOT();
    void disable_inter_RXP();

    bool is_inter_active_TSER();
    bool is_inter_active_MODF();
    bool is_inter_active_CRCE();
    bool is_inter_active_OVR();
    bool is_inter_active_UDR();
    bool is_inter_active_TXTF();
    bool is_inter_active_EOT();
    bool is_inter_active_DXP();
    bool is_inter_active_TXP();
    bool is_inter_active_RXP();

    /*****
     * Status register
     */

    uint16_t get_remaining_data_frames();

    bool is_active_RXWNE();
    spi_RxFIFO_packing_lvl get_rx_fifo_packing_level();
    bool is_active_TXC();
    bool is_active_SUSP();
    bool is_active_TSERF();
    bool is_active_MODF();
    bool is_active_TIFRE();
    bool is_active_CRCE();
    bool is_active_OVR();
    bool is_active_UDR();
    bool is_active_TXTF();
    bool is_active_EOT();
    bool is_active_DXP();
    bool is_active_TXP();
    bool is_active_RXP();

    /*****
     * Clear flag register
     */

    void clear_SUSP();
    void clear_TSERF();
    void clear_MODF();
    void clear_TIFRE();
    void clear_CRCE();
    void clear_OVR();
    void clear_UDR();
    void clear_TXTF();
    void clear_EOT();

    /*****
     * Transmit data register
     */

    void write_tx32(uint32_t word);
    void write_tx16(uint16_t word);
    void write_tx8(uint8_t word);

    /*****
     * Receive data register
     */

    uint32_t read_rx32();
    uint16_t read_rx16();
    uint8_t read_rx8();

    /*****
     * Polynomial register
     */

    void set_crc_polynomial(uint32_t polynomial);
    uint32_t get_crc_polynomial();

    /*****
     * Transmitter CRC register
     */

    uint32_t get_computed_tx_crc();

    /*****
     * Receiver CRC register
     */

    uint32_t get_computed_rx_crc();

    /*****
     * Underrun data register
     */

    void set_underrun_pattern(uint32_t pattern);
    uint32_t get_underrun_pattern();

    /*****
     * Configuration register
     */

    void enable_master_clock_out();
    void disable_master_clock_out();
    bool is_master_clock_out_enabled();

    void use_odd_prescale_factor();
    void use_even_prescale_factor();
    bool is_odd_prescale_factor();

    void set_i2s_linear_pescale(uint8_t prescale);
    uint8_t get_i2s_linear_presc();

    void set_data_alignment(spi_data_alignment alignment);
    spi_data_alignment get_data_alignment();

    void invert_word_select();
    void uninvert_word_select();
    bool is_word_select_inverted();

    void set_fixed_slave_ch_length();
    void set_unfixed_slave_ch_length();
    bool is_fixed_slave_ch_length();

    void set_serial_clock_polarity(spi_clock_polarity pol);
    spi_clock_polarity get_serial_clock_polarity();

    void set_ch_length(spi_ch_length length);
    spi_ch_length get_ch_length();

    void set_tx_data_length(spi_data_length length);
    spi_data_length get_tx_data_length();

    void set_pcm_frame_sync(i2s_sync sync);
    i2s_sync get_pcm_frame_sync();

    void set_i2s_standard(i2s_standard standard);
    i2s_standard get_i2s_standard();

    void set_i2s_configuration_mode(i2s_mode mode);
    i2s_mode get_i2s_mode();

    void use_spi();
    void use_i2s_pcm();
    bool is_using_i2s_pcm();
};

#define SPI1_BASE				(0x40013000)
#define SPI2_BASE			    (0x40003800)
#define SPI3_BASE              	(0x40003C00)
#define SPI4_BASE				(0x40013400)
#define SPI5_BASE				(0x40015000)
#define SPI6_BASE				(0x58001400)

#define SPI1                    (*(dd::spi_def*) SPI1_BASE)
#define SPI2                    (*(dd::spi_def*) SPI2_BASE)
#define SPI3                    (*(dd::spi_def*) SPI3_BASE)
#define SPI4                    (*(dd::spi_def*) SPI4_BASE)
#define SPI5                    (*(dd::spi_def*) SPI5_BASE)
#define SPI6                    (*(dd::spi_def*) SPI6_BASE)

};
