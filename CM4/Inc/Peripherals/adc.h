#pragma once

#include <stdint.h>

#define ADC_MASTER_OFFSET               (0x000)
#define ADC_SLAVE_OFFSET                (0x100)
#define ADC_COMMON_OFFSET               (0x300)

#define ADC1_BASE                       (0x40022000)
#define ADC2_BASE                       (0x40022000)
#define ADC3_BASE                       (0x58026000)

struct ADCStruct{
    volatile uint32_t ISR;
    volatile uint32_t IER;
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CFGR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t PCSEL;
    volatile uint32_t LTR1;
    volatile uint32_t HTR1;
    volatile uint32_t Reserved0;
    volatile uint32_t Reserved1;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t SQR4;
    volatile uint32_t DR;
    volatile uint32_t Reserved2[2];
    volatile uint32_t JSQR;
    volatile uint32_t Reserved3[4];
    volatile uint32_t OFR1;
    volatile uint32_t OFR2;
    volatile uint32_t OFR3;
    volatile uint32_t OFR4;
    volatile uint32_t Reserved4[4];
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t Reserved5[4];
    volatile uint32_t AWD2CR;
    volatile uint32_t AWD3CR;
    volatile uint32_t Reserved6[2];
    volatile uint32_t LTR2;
    volatile uint32_t HTR2;
    volatile uint32_t LTR3;
    volatile uint32_t HTR3;
    volatile uint32_t DIFSEL;
    volatile uint32_t CALFACT;
    volatile uint32_t CALFACT2;
    volatile uint32_t Reserved[2];
};

#define ADC1                            ((ADCStruct *)(ADC1_BASE + ADC_MASTER_OFFSET))
#define ADC2                            ((ADCStruct *)(ADC2_BASE + ADC_SLAVE_OFFSET))
#define ADC3                            ((ADCStruct *)(ADC3_BASE + ADC_MASTER_OFFSET))

struct ADCCommonStruct{
    volatile uint32_t CSR;
    volatile uint32_t Reserved0;
    volatile uint32_t CCR;
    volatile uint32_t CDR;
    volatile uint32_t CDR2;
};

#define ADC12_COMMON                    ((ADCCommonStruct *)(ADC1_BASE + ADC_COMMON_OFFSET))
#define ADC3_COMMON                     ((ADCCommonStruct *)(ADC3_BASE + ADC_COMMON_OFFSET))

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
	ADRDY,				//ADC ready
	EOSMP,				//End of sampling flag
	EOC,				//End of conversion flag
	EOS,				//End of regular sequence flag
	OVR,				//ADC overrun flag
	JEOC,				//Injected channel EOC flag
	JEOS,				//Injected channel EOS flag
	AWD1, AWD2, AWD3,	//Analog watchdog flags
	JQOVF,				//Injected context queue overflow flag
	LDORDY=12			//ADC LDO output voltage ready bit
} adc_flag;
typedef enum {
    SingleEnded=0,
    Differential=1
} adc_calibrate_input_mode;
typedef enum {
	boost00,	//used when ADC clock ≤ 6.25 MHz
	boost01,	//used when 6.25 MHz < ADC clock frequency ≤ 12.5 MHz
	boost10,	//used when 12.5 MHz < ADC clock ≤ 25.0 MHz
	boost11		//used when 25.0 MHz < ADC clock ≤ 50.0 MHz
} adc_boost_mode;

void adc_clearFlag(ADCStruct* adc, adc_flag flag);
bool adc_readClearFlag(ADCStruct* adc, adc_flag flag);

void adc_wake(ADCStruct* adc);
void adc_calibrate(ADCStruct* adc, adc_calibrate_input_mode mode, bool cal_lin);
void adc_set_boost(ADCStruct* adc, adc_boost_mode mode);
void adc_enable(ADCStruct* adc);
void adc_disable(ADCStruct* adc);

void adc_start(ADCStruct* adc);
void adc_stop(ADCStruct* adc);

struct ADCSettingsCFGR {
    bool JQDIS;         //Injected Queue disable
    uint8_t AWD1CH:  5; //Analog watchdog 1 channel selection
    bool JAUTO;         //Automatic injected group conversion
    bool JAWD1EN;       //Analog watchdog 1 enable on injected channel
    bool AWD1EN;        //Analog watchdog 1 enable on regular channels
    bool AWD1SGL;       //Enable the watchdog 1 on a single channel or on all channels
    bool JQM;           //JSQR queue mode
    bool JDISCEN;       //Discontinuous mode on injected channels
    uint8_t DISCNUM: 3; //Discontinuous mode channel count
    bool DISCEN;        //Discontinuous mode for regular channels
    bool AUTDLY;        //Delayed conversion mode
    bool CONT;          //Single / continuous conversion mode for regular conversions
    bool OVRMOD;        //Overrun Mode
    uint8_t EXTEN:   2; //External trigger enable and polarily selection for regular channels
    uint8_t EXTSEL:  5; //External trigger selection for regular group
    uint8_t RES:     3; //Data resolution
    uint8_t DMNGT:   2; //Data Management configuration

    ADCSettingsCFGR(): //Has the default values for each field
        JQDIS(1), AWD1CH(0), JAUTO(0), JAWD1EN(0), AWD1EN(0), AWD1SGL(0), JQM(0), JDISCEN(0), DISCNUM(0),
        DISCEN(0), AUTDLY(0), CONT(0), OVRMOD(0), EXTEN(0), EXTSEL(0), RES(0), DMNGT(0)
    {}
};
void adc_set_configurations1(ADCStruct* adc, ADCSettingsCFGR s);

struct ADCSettingsCFGR2{
    uint8_t LSHIFT: 4;  //Left shift factor
    uint16_t OSVR:  10;  //Oversampling ratio
    bool RSHIFT4;       //Right-shift data after Offset 4 correction
    bool RSHIFT3;       //Right-shift data after Offset 3 correction
    bool RSHIFT2;       //Right-shift data after Offset 2 correction
    bool RSHIFT1;       //Right-shift data after Offset 1 correction
    bool ROVSM;         //Regular Oversampling mode
    bool TROVS;         //Triggered Regular Oversampling
    uint8_t OVSS:   4;  //Oversampling right shift
    bool JOVSE;         //Injected Oversampling enable
    bool ROVSE;         //Regular Oversampling enable

    ADCSettingsCFGR2(): //Has the default values for each field
        LSHIFT(0), OSVR(0), RSHIFT4(0), RSHIFT3(0), RSHIFT2(0), RSHIFT1(0),
        ROVSM(0), TROVS(0), OVSS(0), JOVSE(0), ROVSE(0)
    {}
};
void adc_set_configurations2(ADCStruct* adc, ADCSettingsCFGR2 s);

void adc_set_seqLen(ADCStruct* adc, uint32_t length);
void adc_set_chSampleTime(ADCStruct* adc, uint8_t smplTime, uint8_t ch);
void adc_preselectCh(ADCStruct* adc, uint8_t ch);
void adc_set_regSeq(ADCStruct* adc, uint8_t seq, uint8_t ch);

uint32_t adc_read(ADCStruct* adc);

/////////////////////////////////////////////////////////////////////
typedef enum {
	ASYNC=0,
	SYNC_DIV1=1,
	SYNC_DIV2=2,
	SYNC_DIV4=3
} adcCom_ckmode_types;

void adcCom_set_ckmode(ADCCommonStruct* adcCom, adcCom_ckmode_types ckmode);
