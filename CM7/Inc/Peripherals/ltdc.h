#pragma once
#include <stdint.h>

#define LTDC_BASE		0x50001000

struct LTDCStruct {
	volatile uint32_t Reserved0[2];		//Not explicitly labeled as reserved, but the first register is offset by 2 words
	volatile uint32_t SSCR;				//Synchronization size configuration register
	volatile uint32_t BPCR;				//Back porch configuration register
	volatile uint32_t AWCR;				//Active width configuration register
	volatile uint32_t TWCR;				//Total width configuration register
	volatile uint32_t GCR;				//Global control register
	volatile uint32_t SRCR;				//Shadow reload configuration register
	volatile uint32_t BCCR;				//Background color configuration register
	volatile uint32_t Reserved1;		//Reserved
	volatile uint32_t IER;				//Interrupt enable register
	volatile uint32_t ISR;				//Interrupt status register
	volatile uint32_t ICR;				//Interrupt clear register
	volatile uint32_t LIPCR;			//Line interrupt position configuration register
	volatile uint32_t CPSR;				//Current position status register
	volatile uint32_t CDSR;				//Current display status register
	volatile uint32_t Reserved2[14];	//Reserved
	volatile uint32_t L1CR;				//L1 control register
	volatile uint32_t L1WHPCR;			//L1 window horizontal position configuration register
	volatile uint32_t L1WVPCR;			//L1 window vertical position configuration register
	volatile uint32_t L1CKCR;			//L1 color keying configuration register
	volatile uint32_t L1CACR;			//L1 pixel format configuration register
	volatile uint32_t L1DCCR;			//L1 constant alpha configuration register
	volatile uint32_t L1BFCR;			//L1 default color configuration register
	volatile uint32_t Reserved3[2];		//Reserved
	volatile uint32_t L1CFBAR;			//L1 blending factors configuration register
	volatile uint32_t L1CFBLR;			//L1 color frame buffer address register
	volatile uint32_t L1CFBNLR;			//L1 color frame buffer length register
	volatile uint32_t Reserved4[3];		//Reserved
	volatile uint32_t L1CLUTWR;			//L1 CLUT write register
	volatile uint32_t Reserved5[15];	//Reserved
	volatile uint32_t L2CR;				//L2 control register
	volatile uint32_t L2WHPCR;			//L2 window horizontal position configuration register
	volatile uint32_t L2WVPCR;			//L2 window vertical position configuration register
	volatile uint32_t L2CKCR;			//L2 color keying configuration register
	volatile uint32_t L2CACR;			//L2 pixel format configuration register
	volatile uint32_t L2DCCR;			//L2 constant alpha configuration register
	volatile uint32_t L2BFCR;			//L2 default color configuration register
	volatile uint32_t Reserved6[2];		//Reserved
	volatile uint32_t L2CFBAR;			//L2 blending factors configuration register
	volatile uint32_t L2CFBLR;			//L2 color frame buffer address register
	volatile uint32_t L2CFBNLR;			//L2 color frame buffer length register
	volatile uint32_t Reserved7[3];		//Reserved
	volatile uint32_t L2CLUTWR;			//L2 CLUT write register
};
