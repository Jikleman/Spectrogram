#pragma once

#include <stdint.h>

#define RCC_HCLK1	(64000000) //64MHz

#define TIM_BASE	(0x40000000)
#define TIM_OFFSET 	(0x400)

struct TIMStruct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t Reserved0;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t Reserved1;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t AF1;
	volatile uint32_t TISEL;
	volatile uint32_t reserved[(TIM_OFFSET - ( 22*sizeof(volatile uint32_t) )) / sizeof(volatile uint32_t)]; //22 registers in TIM_Struct
};
static_assert(sizeof(TIMStruct) == 0x400, "TIM_Struct does not fill exactly 0x400 bytes");

#define TIM2			((TIMStruct *) (TIM_BASE + TIM_OFFSET*0))
#define TIM3            ((TIMStruct *) (TIM_BASE + TIM_OFFSET*1))
#define TIM4			((TIMStruct *) (TIM_BASE + TIM_OFFSET*2))
#define TIM5			((TIMStruct *) (TIM_BASE + TIM_OFFSET*3))

///////////////////////////////////////////////////////////////////////////

typedef enum {
	Reset,
	Enable,
	Update,
	Compare_Pulse,
	Compare_OC1REFC,
	Compare_OC2REFC,
	Compare_OC3REFC,
	Compare_OC4REFC
} tim_mms;

//Contains the bit fields in the TIM CR except CEN.
struct TIMSettingsCR {
	//CR1
	bool UIFREMAP;		//UIF status bit remapping
	uint8_t CDK: 2;		//Clock division
	bool ARPE;			//Auto-reload preload enable
	uint8_t CMS: 2;		//Center-aligned mode selection
	bool DIR;			//Direction
	bool OPM;			//One-pulse mode
	bool URS;			//Update request source
	bool UDIS;			//Update disable
	//CR2
	bool TI1S;			//TI1 selection
	tim_mms MMS: 3;		//Master mode selection
	bool CCDS;			//Capture/compare DMA selection

	TIMSettingsCR(): //The default values for each bit
		UIFREMAP(0), CDK(0), ARPE(0), CMS(0), DIR(0), OPM(0), URS(0), UDIS(0),
		TI1S(0), MMS(tim_mms::Reset), CCDS(0)
	{}
};

void tim_controls(TIMStruct* tim, TIMSettingsCR s);
void tim_prescale(TIMStruct* tim, uint16_t presc);
void tim_auto_reload(TIMStruct* tim, uint32_t ar);

void tim_enable(TIMStruct* tim);
void tim_disable(TIMStruct* tim);

struct TIMSettingsDIER {
	bool TDE;		//Trigger DMA request enable
	bool CC4DE;		//Compare/Compare 4 DMA request enable
	bool CC3DE;		//Compare/Compare 3 DMA request enable
	bool CC2DE;		//Compare/Compare 2 DMA request enable
	bool CC1DE;		//Compare/Compare 1 DMA request enable
	bool UDE;		//Update DMA request enable
	bool TIE;		//Trigger interrupt enable
	bool CC4IE;		//Compare/Compare 4 interrupt enable
	bool CC3IE;		//Compare/Compare 3 interrupt enable
	bool CC2IE;		//Compare/Compare 2 interrupt enable
	bool CC1IE;		//Compare/Compare 1 interrupt enable
	bool UIE;		//Update interrupt request enable
};
void tim_dma_ir_en(TIMStruct* tim, TIMSettingsDIER s);

typedef enum {
	UIF=0,				//Update interrupt flag
	CC1IF=1,			//Compare/Compare 1 interrupt flag
	CC2IF=2,			//Compare/Compare 2 interrupt flag
	CC3IF=3,			//Compare/Compare 3 interrupt flag
	CC4IF=4,			//Compare/Compare 4 interrupt flag
	TIF=6,				//Trigger interrupt flag
	CC1OF=9,			//Compare/Compare 1 overcapture flag
	CC2OF=10,			//Compare/Compare 2 overcapture flag
	CC3OF=11,			//Compare/Compare 3 overcapture flag
	CC4OF=12,			//Compare/Compare 4 overcapture flag
} tim_flag;

bool tim_read_flag(TIMStruct* tim, tim_flag flag);
void tim_clear_flag(TIMStruct* tim, tim_flag flag);