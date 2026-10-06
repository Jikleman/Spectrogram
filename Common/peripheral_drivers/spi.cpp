#include "spi.h"
#include "./utils/bitwise_functions.h"

using namespace dd;

#define IOLOCK 		16
#define TCRINI 		15
#define RCRINI 		14
#define CRC33_17 	13
#define SSI			12
#define HDDIR		11
#define CSUSP		10
#define CSTART		9
#define MASRX		8
#define SPE			0

#define TSER		16
#define TSIZE 		15

#define MBR 		28
#define CRCEN		22
#define CRCSIZE		16
#define TXDMAEN		15
#define RXDMAEN		14
#define UDRDET		11
#define UDRCFG		9
#define FTHLV		5
#define DSIZE		0

#define AFCNTR		31
#define SSOM		30
#define SSOE		29
#define SSIOP		28
#define SSM			26
#define CPOL		25
#define CPHA		24
#define LSBFRST		23
#define MASTER		22
#define SP			19
#define COMM		17
#define IOSWP		15
#define MIDI		4
#define MSSI		0

#define TSERFIE		10
#define MODFIE		9
#define TIFREIE		8
#define CRCEIE		7
#define OVRIE		6
#define UDRIE		5
#define TXTFIE		4
#define EOTIE		3
#define DXPIE		2
#define TXPIE		1
#define RXPIE		0

#define CTSIZE		16
#define RXWNE		15
#define RXPLVL		13
#define TXC			12
#define SUSP		11
#define TSERF		10
#define MODF		9
#define TIFRE		8
#define CRCE		7
#define OVR			6
#define UDR			5
#define TXTF		4
#define EOT			3
#define DXP			2
#define TXP			1
#define RXP			0

#define SUSPC		11
#define TSERFC		10
#define MODFC		9
#define TIFREC		8
#define CRCEC		7
#define OVRC		6
#define UDRC		5
#define TXTFC		4
#define EOTC		3

#define MCKOE		25
#define ODD			24
#define I2SDIV		16
#define DATFMT		14
#define WSINV		13
#define FIXCH		12
#define CKPOL		11
#define CHLEN		10
#define DATLEN		8
#define PCMSYNC		7
#define I2SSTD		4
#define I2SCFG		1
#define I2SMOD		0

/// \cond
/**
 * Control register 1
 */
/// \endcond

void spi_def::lock_io_conf() {
	set_bit(CR1, IOLOCK);
}

void spi_def::unlock_io_conf() {
	reset_bit(CR1, IOLOCK);
}

bool spi_def::is_io_locked() {
	return read_bit(CR1, IOLOCK);
}

void spi_def::set_tx_crc_init_pattern(spi_crc_pattern pattern) {
	write_bit(CR1, TCRINI, (bool) pattern);
}

void spi_def::set_rx_crc_init_pattern(spi_crc_pattern pattern) {
	write_bit(CR1, RCRINI, (bool) pattern);
}

spi_crc_pattern spi_def::get_tx_crc_init_pattern() {
	return (spi_crc_pattern) read_bit(CR1, TCRINI);
}

spi_crc_pattern spi_def::get_rx_crc_init_pattern() {
	return (spi_crc_pattern) read_bit(CR1, RCRINI);
}

void spi_def::use_full_crc_poly() {
	set_bit(CR1, CRC33_17);
}

void spi_def::dont_use_full_crc_poly() {
	reset_bit(CR1, CRC33_17);
}

bool spi_def::is_using_full_crc_poly() {
	return read_bit(CR1, CRC33_17);
}

void spi_def::set_internal_ss() {
	set_bit(CR1, SSI);
}

void spi_def::reset_internal_ss() {
	reset_bit(CR1, SSI);
}

bool spi_def::is_internal_ss_active() {
	return read_bit(CR1, SSI);
}

void spi_def::set_half_duplex_dir(spi_direction direction) {
	write_bit(CR1, HDDIR, (bool) direction);
}

spi_direction spi_def::get_half_duplex_dir() {
	return (spi_direction) read_bit(CR1, HDDIR);
}

void spi_def::master_request_suspend_tx() {
	set_bit(CR1, CSUSP);
}

void spi_def::master_start_tx() {
	set_bit(CR1, CSTART);
}

bool spi_def::is_master_tx_idle() {
	return !read_bit(CR1, CSTART);
}

void spi_def::enable_master_rx_auto_suspend() {
	set_bit(CR1, MASRX);
}

void spi_def::disable_master_rx_auto_suspend() {
	reset_bit(CR1, MASRX);
}

bool spi_def::is_master_rx_auto_suspend_enabled() {
	return read_bit(CR1, MASRX);
}

void spi_def::enable_spi_transfer() {
	set_bit(CR1, SPE);
}

void spi_def::disable_spi_transfer() {
	reset_bit(CR1, SPE);
}

bool spi_def::is_spi_enabled() {
	return read_bit(CR1, SPE);
}

/// \cond
/**
 * Control register 2
 */
/// \endcond

void spi_def::set_next_tx_data_frame_amount(uint16_t amount) {
	write_bits(CR2, TSER, amount, 16);
}

uint16_t spi_def::get_next_tx_data_frame_amount() {
	return read_bits(CR2, TSER, 16);
}

void spi_def::set_current_tx_data_frame_amount(uint16_t amount) {
	write_bits_nomask(CR2, TSIZE, amount, 16);
}

uint16_t spi_def::get_current_tx_data_frame_amount() {
	return read_bits(CR2, TSIZE, 16);
}


/// \cond
/**
 * Configuration register 1
 */
/// \endcond

void spi_def::set_baud_rate_prescale(spi_baud_presc prescale){
	write_bits(CFG1, MBR, (uint32_t) prescale, 3);
}

spi_baud_presc spi_def::get_baud_rate_prescale(){
	return (spi_baud_presc) read_bits(CFG1, MBR, 3);
}

void spi_def::disable_crc(){
	reset_bit(CFG1, CRCEN);
}

void spi_def::enable_crc(){
	set_bit(CFG1, CRCEN);
}

bool spi_def::is_crc_enabled(){
	return read_bit(CFG1, CRCEN);
}

void spi_def::set_crc_frame_bit_size(spi_frame_size size){
	write_bits(CFG1, CRCSIZE, (uint32_t) size, 5);
}

spi_frame_size spi_def::get_crc_frame_bit_size() {
	return (spi_frame_size) read_bits(CFG1, CRCSIZE, 5);
}

void spi_def::enable_tx_dma() {
	set_bit(CFG1, TXDMAEN);
}

void spi_def::disable_tx_dma() {
	reset_bit(CFG1, TXDMAEN);
}

bool spi_def::is_tx_dma_enabled() {
	return read_bit(CFG1, TXDMAEN);
}

void spi_def::enable_rx_dma() {
	set_bit(CFG1, RXDMAEN);
}

void spi_def::disable_rx_dma() {
	reset_bit(CFG1, RXDMAEN);
}

bool spi_def::is_rx_dma_enabled() {
	return read_bit(CFG1, RXDMAEN);
}

void spi_def::set_slave_tx_udr_detection(spi_udr_detection det) {
	write_bits(CFG1, UDRDET, (uint32_t) det, 2);
}

spi_udr_detection spi_def::get_slave_tx_udr_detection() {
	return (spi_udr_detection) read_bits(CFG1, UDRDET, 2);
}

void spi_def::set_slave_tx_udr_behavior(spi_udr_behavior behavior) {
	write_bits(CFG1, UDRCFG, (uint32_t) behavior, 2);
}

spi_udr_behavior spi_def::get_slave_tx_udr_behavior() {
	return (spi_udr_behavior) read_bits(CFG1, UDRCFG, 2);
}

void spi_def::set_fifo_threshold(spi_fifo_threshold level) {
	write_bits(CFG1, FTHLV, (uint32_t) level, 2);
}

spi_fifo_threshold spi_def::get_fifo_threshold() {
	return (spi_fifo_threshold) read_bits(CFG1, FTHLV, 2);
}

void spi_def::set_data_frame_bit_size(spi_frame_size size) {
	write_bits(CFG1, DSIZE, (uint32_t) size, 5);
}

spi_frame_size spi_def::get_data_frame_bit_size(){
	return (spi_frame_size) read_bits(CFG1, DSIZE, 5);
}

/// \cond
/**
 * Configuration register 2
 */
/// \endcond

void spi_def::always_keep_gpio_control() {
	set_bit(CFG2, AFCNTR);
}

void spi_def::no_control_of_gpio() {
	reset_bit(CFG2, AFCNTR);
}

bool spi_def::is_keep_gpio_control() {
	return read_bit(CFG2, AFCNTR);
}

void spi_def::set_master_ss_out_management(spi_ss_out_management out_manage) {
	write_bit(CFG1, AFCNTR, (bool) out_manage);
}

spi_ss_out_management spi_def::get_master_ss_out_management() {
	return (spi_ss_out_management) read_bit(CFG1, AFCNTR);
}
void spi_def::enable_ss_out() {
	set_bit(CFG2, SSOE);
}

void spi_def::disable_ss_out() {
	reset_bit(CFG2, SSOE);
}

bool spi_def::is_ss_enabled() {
	return read_bit(CFG2, SSOE);
}

void spi_def::set_ss_polarity_high() {
	set_bit(CFG2, SSIOP);
}

void spi_def::set_ss_polarity_low() {
	reset_bit(CFG2, SSIOP);
}

bool spi_def::is_ss_polarity_high() {
	return read_bit(CFG2, SSIOP);
}


void spi_def::set_ss_input_software_management(spi_ss_input_management in_manage) {
	write_bit(CFG2, SSM, (bool) in_manage);
}

spi_ss_input_management spi_def::get_ss_input_software_management() {
	return (spi_ss_input_management) read_bit(CFG2, SSM);
}

void spi_def::set_clock_polarity(spi_cpol polarity) {
	write_bit(CFG2, CPOL, (bool) polarity);
}

spi_cpol spi_def::get_clock_polarity() {
	return (spi_cpol) read_bit(CFG2, CPOL);
}

void spi_def::set_clock_phase(spi_cpha phase){
	write_bit(CFG2, CPHA, (bool) phase);
}

spi_cpha spi_def::get_clock_phase(){
	return (spi_cpha) read_bit(CFG2, CPHA);
}

void spi_def::set_data_frame_format(spi_data_frame_format format){
	write_bit(CFG2, LSBFRST, (bool) format);
}

spi_data_frame_format spi_def::get_data_frame_format() {
	return (spi_data_frame_format) read_bit(CFG2, LSBFRST);
}

void spi_def::set_spi_mode(spi_mode mode) {
	write_bit(CFG2, MASTER, (bool) mode);
}

spi_mode spi_def::get_spi_mode() {
	return (spi_mode) read_bit(CFG2, MASTER);
}

void spi_def::set_protocol(spi_protocol protocol) {
	write_bits(CFG2, SP, (uint32_t) protocol, 2);
}

spi_protocol spi_def::get_protocol() {
	return (spi_protocol) read_bits(CFG2, SP, 2);
}

void spi_def::set_comm_mode(spi_comm_mode mode) {
	write_bits(CFG2, COMM, (uint32_t) mode, 2);
}

spi_comm_mode spi_def::get_comm_mode() {
	return (spi_comm_mode) read_bits(CFG2, COMM, 2);
}

void spi_def::swap_miso_mosi() {
	set_bit(CFG2, IOSWP);
}

void spi_def::unswap_miso_mosi() {
	reset_bit(CFG2, IOSWP);
}

bool spi_def::is_miso_mosi_swapped() {
	return read_bit(CFG2,  IOSWP);
}

void spi_def::set_master_inter_data_idleness(spi_idle_delay delay) {
	write_bits(CFG2, MIDI, (uint32_t) delay, 4);
}

spi_idle_delay spi_def::get_master_inter_data_idleness() {
	return (spi_idle_delay) read_bits(CFG2, MIDI, 4);
}

void spi_def::set_master_ss_idleness(spi_idle_delay delay) {
	write_bits(CFG2, MSSI, (uint32_t) delay, 4);
}

spi_idle_delay spi_def::get_master_ss_idleness() {
	return (spi_idle_delay) read_bits(CFG2, MSSI, 4);
}

/// \cond
/**
 *  Interrupt register
 */
/// \endcond

/// \cond
///	Interrupt enable
/// \endcond

void spi_def::enable_inter_TSER() {
	set_bit(IER, TSERFIE);
}

void spi_def::enable_inter_MODF() {
	set_bit(IER, MODFIE);
}

void spi_def::enable_inter_CRCE() {
	set_bit(IER, CRCEIE);
}

void spi_def::enable_inter_OVR() {
	set_bit(IER, OVRIE);
}

void spi_def::enable_inter_UDR() {
	set_bit(IER, UDRIE);
}

void spi_def::enable_inter_TXTF() {
	set_bit(IER, TXTFIE);
}

void spi_def::enable_inter_EOT() {
	set_bit(IER, EOTIE);
}

void spi_def::enable_inter_DXP() {
	set_bit(IER, DXPIE);
}

void spi_def::enable_inter_TXP() {
	set_bit(IER, TXPIE);
}

void spi_def::enable_inter_RXP() {
	set_bit(IER, RXPIE);
}

/// \cond
///	Interrupt disable
/// \endcond

void spi_def::disable_inter_TSER() {
	set_bit(IER, TSERFIE);
}

void spi_def::disable_inter_MODF() {
	reset_bit(IER, MODFIE);
}

void spi_def::disable_inter_CRCE() {
	reset_bit(IER, CRCEIE);
}

void spi_def::disable_inter_OVR() {
	reset_bit(IER, OVRIE);
}

void spi_def::disable_inter_UDR() {
	reset_bit(IER, UDRIE);
}

void spi_def::disable_inter_TXTF() {
	reset_bit(IER, TXTFIE);
}

void spi_def::disable_inter_EOT() {
	reset_bit(IER, EOTIE);
}

void spi_def::disable_inter_RXP() {
	reset_bit(IER, RXPIE);
}

/// \cond
///	Interrupt active
/// \endcond

bool spi_def::is_inter_active_TSER() {
	return read_bit(IER, TSERFIE);
}

bool spi_def::is_inter_active_MODF() {
	return read_bit(IER, MODFIE);
}

bool spi_def::is_inter_active_CRCE() {
	return read_bit(IER, CRCEIE);
}

bool spi_def::is_inter_active_OVR() {
	return read_bit(IER, OVRIE);
}

bool spi_def::is_inter_active_UDR() {
	return read_bit(IER, UDRIE);
}

bool spi_def::is_inter_active_TXTF() {
	return read_bit(IER, TXTFIE);
}

bool spi_def::is_inter_active_EOT() {
	return read_bit(IER, EOTIE);
}

bool spi_def::is_inter_active_DXP() {
	return read_bit(IER, DXPIE);
}

bool spi_def::is_inter_active_TXP() {
	return read_bit(IER, TXPIE);
}

bool spi_def::is_inter_active_RXP() {
	return read_bit(IER, RXPIE);
};

/// \cond
/**
 * Status register
 */
/// \endcond

uint16_t spi_def::get_remaining_data_frames() {
	return read_bits(SR, CTSIZE, 16);
}

bool spi_def::is_active_RXWNE() {
	return read_bit(SR, RXWNE);
}

spi_RxFIFO_packing_lvl spi_def::get_rx_fifo_packing_level() {
	return (spi_RxFIFO_packing_lvl) read_bits(SR, RXPLVL, 2);
}

bool spi_def::is_active_TXC() {
    return read_bit(SR, TXC);
}
bool spi_def::is_active_SUSP() {
    return read_bit(SR, SUSP);
}
bool spi_def::is_active_TSERF() {
    return read_bit(SR, TSERF);
}
bool spi_def::is_active_MODF() {
    return read_bit(SR, MODF);
}
bool spi_def::is_active_TIFRE() {
    return read_bit(SR, TIFRE);
}
bool spi_def::is_active_CRCE() {
    return read_bit(SR, CRCE);
}
bool spi_def::is_active_OVR() {
    return read_bit(SR, OVR);
}
bool spi_def::is_active_UDR() {
    return read_bit(SR, UDR);
}
bool spi_def::is_active_TXTF() {
    return read_bit(SR, TXTF);
}
bool spi_def::is_active_EOT() {
    return read_bit(SR, EOT);
}
bool spi_def::is_active_DXP() {
    return read_bit(SR, DXP);
}
bool spi_def::is_active_TXP() {
    return read_bit(SR, TXP);
}
bool spi_def::is_active_RXP() {
    return read_bit(SR, RXP);
}

/// \cond
/**
 * Clear flag register
 */
/// \endcond

void spi_def::clear_SUSP() {
	set_bit_nomask(IFCR, SUSPC);
}

void spi_def::clear_TSERF() {
	set_bit_nomask(IFCR, TSERFC);
}

void spi_def::clear_MODF() {
	set_bit_nomask(IFCR, MODFC);
}

void spi_def::clear_TIFRE() {
	set_bit_nomask(IFCR, TIFREC);
}

void spi_def::clear_CRCE() {
	set_bit_nomask(IFCR, CRCEC);
}

void spi_def::clear_OVR() {
	set_bit_nomask(IFCR, OVRC);
}

void spi_def::clear_UDR() {
	set_bit_nomask(IFCR, UDRC);
}

void spi_def::clear_TXTF() {
	set_bit_nomask(IFCR, TXTFC);
}

void spi_def::clear_EOT() {
	set_bit_nomask(IFCR, EOTC);
}

/// \cond
/**
 * Transmit data register
 */
/// \endcond

void spi_def::write_tx32(uint32_t word) {
	TXDR = word;
}

void spi_def::write_tx16(uint16_t word) {
	(*(volatile uint16_t*) &TXDR) = word;
}

void spi_def::write_tx8(uint8_t word) {
	(*(volatile uint8_t*) &TXDR) = word;
}

/// \cond
/**
 * Receive data register
 */
/// \endcond

uint32_t spi_def::read_rx32() {
	return TXDR;
}

uint16_t spi_def::read_rx16() {
	return (*(volatile uint16_t*) &TXDR);
}

uint8_t spi_def::read_rx8() {
	return (*(volatile uint8_t*) &TXDR);
}

/// \cond
/**
 * Polynomial register
 */
/// \endcond

void spi_def::set_crc_polynomial(uint32_t polynomial) {
	CRCPOLY = polynomial;
}

uint32_t spi_def::get_crc_polynomial() {
	return CRCPOLY;
}

/// \cond
/**
 * Transmitter CRC register
 */
/// \endcond

uint32_t spi_def::get_computed_tx_crc() {
	return TXCRC;
}

/// \cond
/**
 * Receiver CRC register
 */
/// \endcond

uint32_t spi_def::get_computed_rx_crc() {
	return RXCRC;
}

/// \cond
/**
 * Underrun data register
 */
/// \endcond

void spi_def::set_underrun_pattern(uint32_t pattern) {
	UDRDR = pattern;
}

uint32_t spi_def::get_underrun_pattern() {
	return UDRDR;
}

/// \cond
/**
 * Configuration register
 */
/// \endcond

void spi_def::enable_master_clock_out() {
	set_bit(I2SCFGR, MCKOE);
}

void spi_def::disable_master_clock_out() {
	reset_bit(I2SCFGR, MCKOE);
}

bool spi_def::is_master_clock_out_enabled() {
	return read_bit(I2SCFGR, MCKOE);
}

void spi_def::use_odd_prescale_factor() {
	set_bit(I2SCFGR, ODD);
}

void spi_def::use_even_prescale_factor() {
	set_bit(I2SCFGR, ODD);
}

bool spi_def::is_odd_prescale_factor() {
	return read_bit(I2SCFGR, ODD);
}

void spi_def::set_i2s_linear_pescale(uint8_t prescale) {
	write_bits(I2SCFGR, I2SDIV, (uint8_t) prescale, 8);
}

uint8_t spi_def::get_i2s_linear_presc() {
	return read_bits(I2SCFGR, I2SDIV, 8);
}

void spi_def::set_data_alignment(spi_data_alignment alignment) {
	write_bit(I2SCFGR, DATFMT, (bool) alignment);
}

void spi_def::invert_word_select() {
	set_bit(I2SCFGR, WSINV);
}

void spi_def::uninvert_word_select() {
	reset_bit(I2SCFGR, WSINV);
}

bool spi_def::is_word_select_inverted() {
	return read_bit(I2SCFGR, WSINV);
}

void spi_def::set_fixed_slave_ch_length() {
	set_bit(I2SCFGR, FIXCH);
}

void spi_def::set_unfixed_slave_ch_length() {
	reset_bit(I2SCFGR, FIXCH);
}

bool spi_def::is_fixed_slave_ch_length() {
	return read_bit(I2SCFGR, FIXCH);
}

void spi_def::set_serial_clock_polarity(spi_clock_polarity pol) {
	write_bit(I2SCFGR, CKPOL, (bool) pol);
}

spi_clock_polarity spi_def::get_serial_clock_polarity() {
	return (spi_clock_polarity) read_bit(I2SCFGR, CKPOL);
}

void spi_def::set_ch_length(spi_ch_length length) {
	write_bit(I2SCFGR, CHLEN, (bool) length);
}

spi_ch_length spi_def::get_ch_length() {
	return (spi_ch_length) read_bit(I2SCFGR, CHLEN);
}

void spi_def::set_tx_data_length(spi_data_length length) {
	write_bits(I2SCFGR, DATLEN, (uint32_t) length, 2);
}

spi_data_length spi_def::get_tx_data_length() {
	return (spi_data_length) read_bits(I2SCFGR, DATLEN, 2);
}

void spi_def::set_pcm_frame_sync(i2s_sync sync) {
	write_bit(I2SCFGR, PCMSYNC, (bool) sync);
}

i2s_sync spi_def::get_pcm_frame_sync() {
	return (i2s_sync) read_bit(I2SCFGR, PCMSYNC);
}

void spi_def::set_i2s_standard(i2s_standard standard) {
	write_bits(I2SCFGR, I2SSTD, (uint32_t) standard, 2);
}

i2s_standard spi_def::get_i2s_standard() {
	return (i2s_standard) read_bits(I2SCFGR, I2SSTD, 2);
}

void spi_def::set_i2s_configuration_mode(i2s_mode mode) {
	write_bits(I2SCFGR, I2SCFG, (uint32_t) mode, 3);
}

i2s_mode spi_def::get_i2s_mode() {
	return (i2s_mode) read_bits(I2SCFGR, I2SCFG, 3);
}

void spi_def::use_spi() {
	reset_bit(I2SCFGR, I2SMOD);
}

void spi_def::use_i2s_pcm() {
	set_bit(I2SCFGR, I2SMOD);
}

bool spi_def::is_using_i2s_pcm(){
	return read_bit(I2SCFGR, I2SMOD);
}
