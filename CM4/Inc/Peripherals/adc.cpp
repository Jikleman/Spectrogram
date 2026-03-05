#include "adc.h"

//CR register bits
#define ADCAL       (1<<31)
#define ADCALDIF    (1<<30)
#define DEEPWD      (1<<29)
#define ADVREGEN    (1<<28)
#define ADCALLIN    (1<<16)
#define BOOST1		(9)
#define BOOST0      (8)
#define ADSTP       (1<<4)
#define ADSTART     (1<<2)
#define ADDIS       (1<<1)
#define ADEN        (1<<0)

//Clear a flag in the ISR
void adc_clearFlag(ADCStruct* adc, adc_flag flag){
    adc->ISR = (1 << flag);
}
//Read and clear a flag in the ISR
bool adc_readClearFlag(ADCStruct* adc, adc_flag flag){
    return (adc->ISR & (1 << flag));
}
//Wake ADC from deep-power down, enable voltage regulator, then wait for stable voltage regulator
void adc_wake(ADCStruct* adc){
    adc->CR &= ~DEEPWD;
    adc->CR |= ADVREGEN;
    while(!adc_readClearFlag(adc, LDORDY));
}
//Make sure a conversion isn't active before calling this.
void adc_calibrate(ADCStruct* adc, adc_calibrate_input_mode mode, bool cal_lin){
    (cal_lin) ? adc->CR |= ADCALLIN : adc->CR &= ~ADCALLIN; 
    (mode) ? adc->CR |= ADCALDIF : adc->CR &= ~ADCALDIF;
    
    //Start calibration and wait for calibration to finish
    adc->CR |= ADCAL;
    while (adc->CR & ADCAL);
}
/*00: used when ADC clock ≤ 6.25 MHz
 *01: used when 6.25 MHz < ADC clock frequency ≤ 12.5 MHz
 *10: used when 12.5 MHz < ADC clock ≤ 25.0 MHz
 *11: used when 25.0 MHz < ADC clock ≤ 50.0 MHz
*/
void adc_set_boost(ADCStruct* adc, adc_boost_mode mode){
    adc->CR |= (mode<<BOOST0);
}
//Make sure to wake up adc before enabling adc.
void adc_enable(ADCStruct* adc){
    adc_clearFlag(adc, ADRDY);
    adc->CR |= ADEN;
    while(!adc_readClearFlag(adc, ADRDY));
}
//Make sure that the adc isn't doing a conversion before calling this
void adc_disable(ADCStruct* adc){
    adc->CR |= ADDIS;
    while(adc->CR & ADDIS);
}

void adc_start(ADCStruct* adc){
    adc->CR |= ADSTART;
}
void adc_stop(ADCStruct* adc){
    adc->CR |= ADSTP;
}

void adc_set_configurations1(ADCStruct* adc, ADCSettingsCFGR s){
    uint32_t cfgr = 0;
    cfgr |= (s.JQDIS<<31) | (s.AWD1CH<<26) | (s.JAUTO<<25) | (s.JAWD1EN<<24) | (s.AWD1EN<<23) | (s.AWD1SGL<<22) | (s.JQM<<21) | (s.JDISCEN<<20) | (s.DISCNUM<<17) | (s.DISCEN<<16) | (s.AUTDLY<<14) | (s.CONT<<13)
         |  (s.OVRMOD<<12) | (s.EXTEN<<10) | (s.EXTSEL<<5) | (s.RES<<2) | (s.DMNGT<<0);
    cfgr |= adc->CFGR & (1<<15); //Mask in the reserved bit in the CFGR
    adc->CFGR = cfgr;
}

void adc_set_configurations2(ADCStruct* adc, ADCSettingsCFGR2 s){
    uint32_t cfgr2 = 0;
    cfgr2 |= (s.LSHIFT<<28) | (s.OSVR<<16) | (s.RSHIFT4<<14) | (s.RSHIFT3<<13) | (s.RSHIFT2<<12) | (s.RSHIFT1<<11) | (s.ROVSM<<10) | (s.TROVS<<9) | (s.OVSS<<5) | (s.JOVSE<<1) | (s.ROVSE<<0);
    uint32_t resMask = 0;
    resMask |= (3<<25) | (1<<15) | (7<2);
    cfgr2 |= adc->CFGR2 & resMask; //Mask in the reserved bit in the CFGR2
    adc->CFGR2 = cfgr2;
}

//Length between 0 and 16. Only uses the lowest four bits of the length parameter.
void adc_set_seqLen(ADCStruct* adc, uint8_t length){
    adc->SQR1 |= (length & 0xF);
}
/*Sets the sample time for each ch. Only uses the lowest 3 bits for the smplTime.
 *000: 1.5 ADC clock cycles
 *001: 2.5 ADC clock cycles
 *010: 8.5 ADC clock cycles
 *011: 16.5 ADC clock cycles
 *100: 32.5 ADC clock cycles
 *101: 64.5 ADC clock cycles
 *110: 387.5 ADC clock cycles
 *111: 810.5 ADC clock cycles
*/
void adc_set_chSampleTime(ADCStruct* adc, uint8_t smplTime, uint8_t ch){
    uint32_t SMPR = (((ch + 1) * 3) + 30 - 1) / 30; //Calculate which SMPR to use
	uint32_t startBit = (ch * 3) % 30;              //Calculate the bit in the SMPR to place the data
    uint32_t data = ((smplTime & 7) << startBit);
	switch (SMPR){
	case 1:
		adc->SMPR1 |= data;
		break;
	case 2:
		adc->SMPR2 |= data;
		break;
	default:
		return;
	}
}
//Before adding a channel to a sequence, the channel must be preselected.
void adc_preselectCh(ADCStruct* adc, uint8_t ch){
    adc->PCSEL |= (1<<ch);
}
//Configures the sequence of channel conversions
void adc_set_regSeq(ADCStruct* adc, uint8_t seq, uint8_t ch){
    uint32_t SQR = ((seq * 6 + 6) + 30 - 1) / 30;   //Calculate which SQR to use
	uint32_t startBit = (seq * 6) % 30;             //Calculate the bit in the SQR to place the data
	uint32_t data = (ch << startBit);
	switch (SQR){
	case 1:
		adc->SQR1 |= data;
		break;
	case 2:
		adc->SQR2 |= data;
		break;
	case 3:
		adc->SQR3 |= data;
		break;
	case 4:
		adc->SQR4 |= data;
		break;
	default:
		return;
	}
}

uint32_t adc_read(ADCStruct* adc){
    return adc->DR;
}

////////////////////////////////////////////////////

/*
 *00: CK_ADCx (x=1 to 23) (Asynchronous clock mode)
 *01: adc_sclk/1 (Synchronous clock mode).
 *10: adc_sclk/2 (Synchronous clock mode)
 *11: adc_sclk/4 (Synchronous clock mode)
 *
 * Uses only the lowest two bits of the ckmode parameter
*/
void adcCom_set_ckmode(ADCCommonStruct* adcCom, adcCom_ckmode_types ckmode){
    adcCom->CCR |= (ckmode<<16);
}
