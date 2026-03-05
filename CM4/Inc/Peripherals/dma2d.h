#pragma once

#include <stdint.h>

#define DMA2D_BASE 		(0x52001000)

struct DMA2DStruct {
	volatile uint32_t CR;				//Control register
	volatile uint32_t ISR;				//Interrupt status register
	volatile uint32_t IFCR;				//Interrupt flag clear register
	volatile uint32_t FGMAR;			//Foreground memory address register
	volatile uint32_t FGOR;				//Foreground offset register
	volatile uint32_t BGMAR;			//Background memory address register
	volatile uint32_t BGOR;				//Background offset register
	volatile uint32_t FGPFCCR;			//Foreground PFC control register
	volatile uint32_t FGCOLR;			//Foreground color register
	volatile uint32_t BGPFCCR;			//Background PFC control register
	volatile uint32_t BGCOLR;			//Background color register
	volatile uint32_t FGCMAR;			//Foreground CLUT memory address register
	volatile uint32_t BGCMAR;			//Background CLUT memory address register
	volatile uint32_t OPFCCR;			//Output PFC control registers
	volatile uint32_t OCOLR;			//Output color register
	volatile uint32_t OMAR;				//Output memory address register
	volatile uint32_t OOR;				//Output offset register
	volatile uint32_t NLR;				//Number of line register
	volatile uint32_t LWR;				//Line watermark register
	volatile uint32_t AMTCR;			//AXI master time configuration register
	volatile uint32_t Reserved0[236];	//Reserved field
	volatile uint32_t FGCLUT[256];		//Foreground CLUT
	volatile uint32_t BGCLUT[256];		//Background CLUT
};

#define DMA2D		((DMA2DStruct *) DMA2D_BASE)

enum class DMA2D_Mode {
	Mem2Mem_FGFetchOnly = 0,
	Mem2Mem_WithPFC = 1,
	Mem2Mem_WithBlending = 2,
	Register2Mem = 3,
	Mem2Mem_WithBlendingFixedColorFG = 4,
	Mem2Mem_withBlendingFixedColorBG = 5
};

enum class DMA2D_LineOffsetMode {
	Pixels = 0,		//Offsets will be in pixels. The last 2 bits of the offset fields are ignored.
	Bytes = 1		//Offsets will be in bits.
};


//Configurations for the DMA2D CR register
struct DMA2D_CR_CFG{
	DMA2D_Mode MODE;				//DMA2D mode
	bool CEIE;						//Configuration Error interrupt enable
	bool CTCIE;						//CLUT transfer complete interrupt enable
	bool CAEIE;						//CLUT access error interrupt enable
	bool TWIE;						//Transfer Watermark interrupt enable
	bool TCIE;						//Transfer Complete interrupt enable
	bool TEIE;						//Transfer Error interrupt enable
	DMA2D_LineOffsetMode LOM;		//Line offset Mode

	DMA2D_CR_CFG():
		MODE(DMA2D_Mode::Mem2Mem_FGFetchOnly), CEIE(0),
		CTCIE(0), CAEIE(0), TWIE(0), TCIE(0), TEIE(0),
		LOM(DMA2D_LineOffsetMode::Pixels)
	{}
};

void dma2d_config(DMA2D_CR_CFG cfg);
void dma2d_abort();
void dma2d_susp();
void dma2d_start();

enum class DMA2D_ISR {
	CEIF = 5,
	CTCIF = 4,
	CAEIF = 3,
	TWIF = 2,
	TCIF = 1,
	TEIF = 0
};

bool dma2d_isr_status(DMA2D_ISR isr);
void dma2d_isr_clear(DMA2D_ISR isr);

//Chroma SubSampling mode for YCbCr color mode in pixel format conversion
enum class DMA2D_PFC_CSS {
	ss_444 = 0,		//No chroma subsampling
	ss_422 = 1,
	ss_420 = 2
};

//Alpha modes for pixel format conversion
enum class DMA2D_PFC_AlphaMode{
	NoMod = 0,			//No change to image alpha value
	Replace = 1,		//Replace image alpha value by ALPHA field
	Multiply = 2,		//Replace image alpha value by ALPHA field multiplied by image's original alpha value.
};

//Color modes for pixel format conversion
enum class DMA2D_PFC_ColorMode {
	ARGB8888 = 0,
	RGB888 = 1,
	RGB565 = 2,
	ARGB1555 = 3,
	ARGB4444 = 4,
	L8 = 5,
	AL44 = 6,
	AL88 = 7,
	L4 = 8,
	A8 = 9,
	A4 = 10,
	YCbCr = 11
};

enum class DMA2D_PFC_CLUTColorMode {
	ARGB8888 = 0,
	RGB888 = 1
};

struct DMA2D_PFC_CFG {
	uint8_t ALPHA;				//Alpha value used with Alpha mode
	bool RBS;					//True swaps red and blue for BGR or ABGR format
	bool AI;					//True inverts the alpha value
	DMA2D_PFC_CSS CSS;			//Sets the chroma subsampling mode for YCbCr color mode
	DMA2D_PFC_AlphaMode AM;		//Sets the alpha mode
	uint8_t CS;					//Sets the size of the CLUT. CLUT entries is the CLUT field + 1
	DMA2D_PFC_CLUTColorMode CCM;//Sets the color format of the CLUT
	DMA2D_PFC_ColorMode CM;		//Sets the color format of the image

	DMA2D_PFC_CFG():
		ALPHA(0), RBS(0), AI(0), CSS(DMA2D_PFC_CSS::ss_444),
		AM(DMA2D_PFC_AlphaMode::NoMod), CS(0),
		CCM(DMA2D_PFC_CLUTColorMode::ARGB8888),
		CM(DMA2D_PFC_ColorMode::ARGB8888)
	{}
};

void dma2d_fg_cfgPFC(DMA2D_PFC_CFG cfg);
void dma2d_fg_setMemAddr(uint32_t addr);
void dma2d_fg_setOffset(uint16_t offset);
void dma2d_fg_setCLUTAddr(uint32_t addr);
void dma2d_fg_setColorReg(uint32_t color);
void dma2d_fg_start();

void dma2d_bg_cfgPFC(DMA2D_PFC_CFG cfg);
void dma2d_bg_setMemAddr(uint32_t addr);
void dma2d_bg_setOffset(uint16_t offset);
void dma2d_bg_setCLUTAddr(uint32_t addr);
void dma2d_bg_setColorReg(uint32_t color);
void dma2d_bg_start();

enum class DMA2D_PFC_OUT_ColorMode {
	ARGB888 = 0,
	RGB888 = 1,
	RGB565 = 2,
	ARGB1555 = 3,
	ARGB4444 = 4,
};

struct DMA2D_PFC_OUT_CFG {
	bool RBS;					//True swaps red and blue for BGR or ABGR format
	bool AI;					//True inverts the alpha value
	bool SB;					//Swap bytes
	DMA2D_PFC_OUT_ColorMode CM;	//Sets the color format of the output image

	DMA2D_PFC_OUT_CFG():
		RBS(0), AI(0), SB(0), CM(DMA2D_PFC_OUT_ColorMode::ARGB888)
	{}
};


void dma2d_out_setColor(uint32_t color);
void dma2d_out_setOffset(uint16_t offset);

void dma2d_out_setMemAddr(uint32_t addr);
void dma2d_out_cfgPFC(DMA2D_PFC_OUT_CFG cfg);
void dma2d_setNumLines(uint16_t NumLines, uint16_t PixelPerLine);
void dma2d_setLineWatermark(uint16_t line);

void dma2d_setAXIDeadTime(uint8_t deadCycles);
void dma2d_enableAXIDeadtime();
void dma2d_disableAXIDeadTime();
