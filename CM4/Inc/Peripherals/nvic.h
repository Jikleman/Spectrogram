#pragma once

#include <stdint.h>

#define NVIC_BASE			(0xE000E100)

struct NVICStruct {
	volatile uint32_t ISERx[8];		//Interrupt set enable
	volatile uint32_t ICERx[8];		//Interrupt clear enable
	volatile uint32_t ISPRx[8];		//Interrupt set pending
	volatile uint32_t ICPRx[8];		//Interrupt clear pending
	volatile uint32_t IABRx[8];		//Interrupt active bit
	volatile uint32_t IPRx[8];		//Interrupt priority
	volatile uint32_t STIR;			//Software trigger interrupt
};

#define NVIC				(*(NVICStruct *) NVIC_BASE)

enum class H755_itrPos : uint8_t {
	wwdg1 = 0,				//Use with M7
	wwdg2 = 0,				//Use with M4
	pvd_pvm = 1,
	rtc_tamp_stamp_css_lse = 2,
	rtc_wkup = 3,
	flash = 4,
	rcc = 5,
	exti0 = 6,
	exti1 = 7,
	exti2 = 8,
	exti3 = 9,
	exti4 = 10,
	dma_str0 = 11,
	dma_str1 = 12,
	dma_str2 = 13,
	dma_str3 = 14,
	dma_str4 = 15,
	dma_str5 = 16,
	dma_str6 = 17,
	adc1_2 = 18,
	fdcan1_it0 = 19,
	fdcan2_it0 = 20,
	fdcan1_it1 = 21,
	fdcan2_it1 = 22,
	exti9_5 = 23,
	tim1_brk = 24,
	tim1_up = 25,
	tim1_trg_com = 26,
	tim_cc = 27,
	tim2 = 28,
	tim3 = 29,
	tim4 = 30,
	i2c1_ev = 31,
	i2c1_er = 32,
	i2c2_ev = 33,
	i2c2_er = 34,
	spi1 = 35,
	spi2 = 36,
	usart1 = 37,
	usart2 = 38,
	usart3 = 39,
	exti15_10 = 40,
	rtc_alarm = 42,
	tim8_brk_tim12 = 43,
	tim8_up_tim13 = 44,
	tim8_trg_com_tim14 = 45,
	tim8_cc = 46,
	dma1_str7 = 47,
	fmc = 48,
	sdmmc1 = 49,
	tim5 = 50,
	spi3 = 51,
	uart4 = 52,
	uart5 = 53,
	tim6_dac = 54,
	tim7 = 55,
	dma2_str0 = 56,
	dma2_str1 = 57,
	dma2_str2 = 58,
	dma2_str3 = 59,
	dma2_str4 = 60,
	eth = 61,
	eth_wkup = 62,
	fdcan_cal = 63,
	cm7_sev = 64,			//Use with M4
	cm4_sev = 65,			//Use with M7
	dma2_str5 = 68,
	dma2_str6 = 69,
	dma2_str7 = 70,
	usart6 = 71,
	i2c3_ev = 72,
	i2c3_er = 73,
	otg_hs_ep1_out = 74,
	otg_hs_ep1_in = 75,
	otg_hs_wkup = 76,
	otg_hs = 77,
	dcmi = 78,
	cryp = 79,
	hash_rng = 80,
	fpu = 81,
	uart7 = 82,
	uart8 = 83,
	spi4 = 84,
	spi5 = 85,
	spi6 = 86,
	sai1 = 87,
	ltdc = 88,
	ltdc_er = 89,
	dma2d = 90,
	sai2 = 91,
	quadspi = 92,
	lptim1 = 93,
	cec = 94,
	i2c4_ev = 95,
	i2c4_er = 96,
	spdif = 97,
	otg_fs_ep1_out = 98,
	otg_fs_ep1_in = 99,
	otg_fs_wkup = 100,
	otg_fs = 101,
	dmamux1_ov = 102,
	hrtim1_mst = 103,
	hrtim1_tima = 104,
	hrtim1_timb = 105,
	hrtim1_timc = 106,
	hrtim1_timd = 107,
	hrtim_time = 108,
	hrtim1_flt = 109,
	dfsdm1_flt0 = 110,
	dfsdm1_flt1 = 111,
	dfsdm1_flt2 = 112,
	dfsdm1_flt3 = 113,
	sai3 = 114,
	swpmi1 = 115,
	tim15 = 116,
	tim16 = 117,
	tim17 = 118,
	mdios_wkup = 119,
	mdios = 120,
	jpeg = 121,
	mdma = 122,
	dsi = 123,
	dsi_wkup = 123,
	sdmmc2 = 124,
	hsem0 = 125, 		//Use with M7
	hsem1 = 126,		//Use with M4
	adc3 = 127,
	dmamux2_ovr = 128,
	bdma_ch0 = 129,
	bdma_ch1 = 130,
	bdma_ch2 = 131,
	bdma_ch3 = 132,
	bdma_ch4 = 133,
	bdma_ch5 = 134,
	bdma_ch6 = 135,
	bdma_ch7 = 136,
	comp = 137,
	lptim2 = 138,
	lptim3 = 139,
	lptim4 = 140,
	lptim5 = 141,
	lpuart = 142,
	wwdg2_rst = 143,	//Use with M7
	wwdg1_rst = 143,	//Use with M4
	crs = 144,
	ecc = 145,
	sai4 = 146,
	hold_core = 148,	//Also known as CPU1_HOLD in CPU1 and CPU2_HOLD in CPU2
	wkup = 149
};

//Enables an interrupt
void inline nvic_enableItr(H755_itrPos itr){
	NVIC.ISERx[(uint8_t) itr / 32] = (1 << ((uint8_t) itr%32) );
}

//Disables an interrupt
void inline nvic_clearItr(H755_itrPos itr){
	NVIC.ICERx[(uint8_t) itr / 32] = (1 << ((uint8_t) itr%32) );
}

//Forces an interrupt into a pending state
void inline nvic_setItrPending(H755_itrPos itr){
	NVIC.ISPRx[(uint8_t) itr / 32] = (1 << ((uint8_t) itr%32) );
}

//Removes an interrupt from a pending state
void inline nvic_clearItrPending(H755_itrPos itr){
	NVIC.ICPRx[(uint8_t) itr / 32] = (1 << ((uint8_t) itr%32) );
}

//Returns true if interrupt is active or active and pending
bool inline nvic_isItrActive(H755_itrPos itr){
	return NVIC.IABRx[(uint8_t) itr / 32] & (1 << (uint8_t) itr%32);
}

//Changes the priority of an interrupt
void inline nvic_setItrPriority(H755_itrPos itr, uint8_t priority){
	NVIC.IPRx[(uint8_t) itr / 4] |= (priority<<( ((uint8_t) itr % 4) * 8) );
}

//Generates an interrupt for an interrupt through software.
//This needs privileged access or USERSETMPEND enabled in CCR.
void inline nvic_softTrigItr(H755_itrPos itr){
	NVIC.STIR &= ~0xFF;
	NVIC.STIR |= (uint8_t) itr;
}

//Returns true if the interrupt is enabled.
bool inline nvic_isItrEnabled(H755_itrPos itr){
	return NVIC.ISERx[(uint8_t) itr / 32] & (1 << (uint8_t) itr%32);
}
