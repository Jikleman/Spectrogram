#include <dma2d.h>

//CR register field start bits
#define CR_MODE_BIT			16
#define CR_CEIE_BIT			13
#define CR_CTCIE_BIT		12
#define CR_CAEIE_BIT		11
#define CR_TWIE_BIT			10
#define CR_TCIE_BIT			9
#define CR_TEIE_BIT			8
#define CR_LOM_BIT			6
#define CR_ABORT_BIT		2
#define CR_SUSP_BIT			1
#define CR_START_BIT		0

//FG and BG PFC registers field start bits
#define PFC_ALPHA_BIT		24
#define PFC_RBS_BIT			21
#define PFC_AI_BIT			20
#define PFC_CSS_BIT			18
#define PFC_AM_BIT			16
#define PFC_CS_BIT			8
#define PFC_START_BIT		5
#define PFC_CCM_BIT			4
#define PFC_CM_BIT			0

//Output PFC register field start bits
#define O_PFC_RBS_BIT		21
#define O_PFC_AL_BIT		20
#define O_PFC_SB_BIT		8
#define O_PFC_CM_BIT		0

//Start bit for PL field in NLR
#define NLR_PL_BIT			16

//Start bits for AMTCR fields
#define AMTCR_DT_BIT		8
#define AMTCR_EN_BIT		0

//Sets the DMA2D control fields
void dma2d_config(DMA2D_CR_CFG cfg){
	uint32_t field = 0;
	field |= ((uint32_t)cfg.MODE)<<CR_MODE_BIT;
	field |= cfg.CEIE<<CR_CEIE_BIT;
	field |= cfg.CTCIE<<CR_CTCIE_BIT;
	field |= cfg.CAEIE<<CR_CAEIE_BIT;
	field |= cfg.TWIE<<CR_TWIE_BIT;
	field |= cfg.TCIE<<CR_TCIE_BIT;
	field |= cfg.TEIE<<CR_TEIE_BIT;
	field |= ((uint32_t)cfg.LOM)<<CR_LOM_BIT;
	DMA2D->CR |= field;
}
//Aborts the DMA2D Transfer
void dma2d_abort(){
	uint32_t field = DMA2D->CR | (1<<CR_ABORT_BIT);
	field &= (~1<<CR_START_BIT);
	field &= (~1<<CR_ABORT_BIT);
	DMA2D->CR = field;
}
//Suspends the DMA2D Transfer
void dma2d_susp(){
	uint32_t field = DMA2D->CR | (1<<CR_SUSP_BIT);
	field &= (~1<<CR_START_BIT);
	field &= (~1<<CR_ABORT_BIT);
	DMA2D->CR = field;
}
//Starts the DMA2D Transfer
void dma2d_start(){
//	uint32_t field = DMA2D->CR | (1<<CR_START_BIT);
//	field &= (~1<<CR_SUSP_BIT);
//	field &= (~1<<CR_ABORT_BIT);
	DMA2D->CR |= 1;
}

//Returns the status of the related DMA2D ISR
bool dma2d_isr_status(DMA2D_ISR isr){
	return DMA2D->ISR |= (1<<(uint32_t)isr);
}
//Clears the status of the related DMA2D ISR
void dma2d_isr_clear(DMA2D_ISR isr){
	DMA2D->IFCR = (1<<(uint32_t)isr);
}

//Sets the FG pixel color format controls
void dma2d_fg_cfgPFC(DMA2D_PFC_CFG cfg){
	uint32_t field = 0;
	field |= cfg.ALPHA<<PFC_ALPHA_BIT;
	field |= cfg.RBS<<PFC_RBS_BIT;
	field |= cfg.AI<<PFC_AI_BIT;
	field |= (uint32_t) cfg.CSS<<PFC_CSS_BIT;
	field |= (uint32_t) cfg.AM<<PFC_AM_BIT;
	field |= cfg.CS<<PFC_CS_BIT;
	field |= (uint32_t) cfg.CCM<<PFC_CCM_BIT;
	field |= (uint32_t) cfg.CM<<PFC_CM_BIT;
	DMA2D->FGPFCCR = field;
}
//Sets the address for the foreground image data
//Address alignment needs to match pixel format.
//32-bit pixel means 32-bit aligned. 16-bit pixel means 16-bit aligned.
void dma2d_fg_setMemAddr(uint32_t addr){
	DMA2D->FGMAR = addr;
}
//Sets the line offset for the FG.
//If the image format is 4-bit per pixel, line offset must be even.
void dma2d_fg_setOffset(uint16_t offset){
	DMA2D->FGOR = offset;
}
//Sets the CLUT address for the FG
void dma2d_fg_setCLUTAddr(uint32_t addr){
	DMA2D->FGCMAR = addr;
}
//Sets the field in the FG color register. color is a 24-bit field. The other 8 bits are ignored.
void dma2d_fg_setColorReg(uint32_t color){
	DMA2D->FGCOLR = color & 0x00FFFFFF;
}
//Starts automatic loading of the CLUT for the FG
void dma2d_fg_start(){
	DMA2D->FGPFCCR |= (1<<PFC_START_BIT);
}

//Sets the BG pixel color format controls
void dma2d_bg_cfgPFC(DMA2D_PFC_CFG cfg){
	uint32_t field = 0;
	field |= cfg.ALPHA<<PFC_ALPHA_BIT;
	field |= cfg.RBS<<PFC_RBS_BIT;
	field |= cfg.AI<<PFC_AI_BIT;
	field |= (uint32_t) cfg.CSS<<PFC_CSS_BIT;
	field |= (uint32_t) cfg.AM<<PFC_AM_BIT;
	field |= cfg.CS<<PFC_CS_BIT;
	field |= (uint32_t) cfg.CCM<<PFC_CCM_BIT;
	field |= (uint32_t) cfg.CM<<PFC_CM_BIT;
	DMA2D->BGPFCCR = field;
}
//Sets the address for the background image data
//Address alignment needs to match pixel format.
//32-bit pixel means 32-bit aligned. 16-bit pixel means 16-bit aligned.
void dma2d_bg_setMemAddr(uint32_t addr){
	DMA2D->BGMAR = addr;
}
//Sets the line offset for the BG.
//If the image format is 4-bit per pixel, line offset must be even.
void dma2d_bg_setOffset(uint16_t offset){
	DMA2D->BGOR = offset;
}
//Sets the CLUT address for the BG
void dma2d_bg_setCLUTAddr(uint32_t addr){
	DMA2D->BGCMAR = addr;
}
//Sets the field in the BG Color register. color is a 24-bit field. The other 8 bits are ignored.
void dma2d_bg_setColorReg(uint32_t color){
	DMA2D->BGCOLR = color & 0x00FFFFFF;
}
//Starts automatic loading of the CLUT for the BG
void dma2d_bg_start(){
	DMA2D->BGPFCCR |= (1<<PFC_START_BIT);
}


//Sets the PFC settings for register to memory mode
void dma2d_out_cfgPFC(DMA2D_PFC_OUT_CFG cfg){
	uint32_t field = 0;
	field |= cfg.RBS<<O_PFC_RBS_BIT;
	field |= cfg.AI<<O_PFC_AL_BIT;
	field |= cfg.SB<<O_PFC_SB_BIT;
	field |= (uint32_t) cfg.CM<<O_PFC_CM_BIT;
	DMA2D->OPFCCR = field;
}
//Sets the color to be used in register to memory mode
//Color format should match whatever format is selected
void dma2d_out_setColor(uint32_t color){
	DMA2D->OCOLR = color;
}

//Sets the dma2d output memory address
void dma2d_out_setMemAddr(uint32_t addr){
	DMA2D->OMAR = addr;
}
//Sets the dma2d output offset
void dma2d_out_setOffset(uint16_t offset){
	DMA2D->OOR = offset;
}
//Sets the number of lines and pixels per line
//PL is a 14 bit field. Extra bits will be ignored.
void dma2d_setNumLines(uint16_t NumLines, uint16_t PixelPerLine){
	uint32_t field = NumLines;
	field |= PixelPerLine << NLR_PL_BIT;
	DMA2D->NLR = field;
}
//Sets what line is used as the watermark line
void dma2d_setLineWatermark(uint16_t line){
	DMA2D->LWR = line;
}

//Sets the minimum number of guaranteed cycles between two AXI bus accesses
void dma2d_setAXIDeadTime(uint8_t deadCycles){
	uint32_t field = DMA2D->AMTCR & 1;
	field |= deadCycles<<AMTCR_DT_BIT;
	DMA2D->AMTCR &= field;	//Clear dead cycles field;
}
//Enables AXI dead time
void dma2d_enableAXIDeadtime(){
	DMA2D->AMTCR |= 1<<AMTCR_EN_BIT;
}
//Disable AXI dead time
void dma2d_disableAXIDeadTime(){
	DMA2D->AMTCR &= ~1<<AMTCR_EN_BIT;
}
