#include <stdint.h>
#include <cmath>
#include <st7789v3.h>
#include <color.h>

#include <rcc.h>
#include <hsem.h>
#include <nvic.h>
#include <dma2d.h>

#define shared __attribute__ ((section(".shared_memory")))

//Shared data
//Make sure both cores have the same buffSize
//4096 words (16Kb) per buffer
volatile bool M7_Signal;
const uint32_t buffCol = 128;
const uint32_t buffRow = 32;
const uint32_t buffSize = buffCol * buffRow;
template <typename T, uint32_t buffSize>
struct dBuff{
	volatile bool M7_Done;
	volatile bool M4_Done;
	volatile T buff0[buffSize];
	volatile T buff1[buffSize];
};
volatile shared dBuff<float, buffSize> sharedBuff;
uint32_t currentBuff;

//Global Variables

st7789v3 display;
uint16_t displayBuffer[display.MAX_COLS * display.MAX_ROWS];

//Function prototypes

void sendDisplayBuffer();
void dma2dTest();
void displayColorMap(Msh lowMsh, Msh highMsh);
void dma2dTest();
float clampThenNormalize(float x, const float min, const float max);
rgb565 genColor(Msh lowMsh, Msh highMsh, float x);
uint16_t _REV16(const uint16_t x);

void signal_M7();
void swapBuffers();

//IRQ handlers

extern "C"{
void HSEM1_IRQHandler(void){
	M7_Signal = true;
	HSEM_clearC2Flag(0);
}
}

int main(void)
{
	reallocPeripheral(AHB4_Peripheral::hsem);
	nvic_enableItr(H755_itrPos::hsem1);
	HSEM_enableC2Itr(0);

	//Wait for M7 to be ready
	while(!HSEM_isC1ItrEnabled(0));

	M7_Signal = false;
	currentBuff = 0;
	sharedBuff.M4_Done = false;
	while(true){
		//Wait for both CPUs to be ready
		if(!(sharedBuff.M7_Done && sharedBuff.M4_Done)) {
			//Switch buffers and do synchronization tasks
			swapBuffers();
			sharedBuff.M4_Done = false;
			signal_M7();

			//Wait for M4 to be ready
			while(!M7_Signal) {};
			M7_Signal = false;

			//Do task
		}


	}
}

void init_st7789v3(){
	//Start up sequence for the display
	display.enableBacklight();
	display.sendCommand(st7789v3::commands::SWRESET, false);
	display.sendCommand(st7789v3::commands::SLPOUT);
	display.sendCommand(st7789v3::commands::NORON);
	display.sendCommand(st7789v3::commands::DISPON);
	display.sendCommand(st7789v3::commands::INVON);

	//Set RGB and Control interface color format to 16-bit
	display.sendCommand(st7789v3::commands::COLMOD);
	uint8_t colmod = 0x55;
	display.sendData(&colmod, 1);

	//Set to BGR order and default data direction
	//Bits: X MY MX MV ML RGB MH X X
	display.sendCommand(st7789v3::commands::MADCTL);
	uint8_t madctl = 0b00001000;
	display.sendData(&madctl, 1);
}

//Display buffer is whole screen.
void sendDisplayBuffer(){
	display.sendCommand(st7789v3::commands::NOP);
	display.setColAddr(0, display.MAX_COLS - 1);
	display.setRowAddr(0, display.MAX_ROWS - 1);
	display.sendCommand(st7789v3::commands::RAMWR);
	display.sendData(&((uint8_t *)&displayBuffer[0])[0], 65535);
	display.sendData(&((uint8_t *)&displayBuffer[0])[65535], 44545);
	display.sendCommand(st7789v3::commands::NOP);
}

void displayColorMap(Msh lowMsh, Msh highMsh){
	uint16_t buff[display.MAX_COLS];

	display.setColAddr(0, display.MAX_COLS - 1);
	display.setRowAddr(0, display.MAX_ROWS - 1);

	display.sendCommand(st7789v3::commands::RAMWR);
	for (uint32_t i = 0; i < display.MAX_ROWS; i++){
		float interp = i / (float) (display.MAX_ROWS - 1);
		rgb565 color = sRGBCompanding(Msh2RGB(interpolateColor(lowMsh, highMsh, interp)));
		uint16_t data = _REV16(color.data);
		for (uint32_t j = 0; j < display.MAX_COLS; j++){
			buff[j] = data;
		}
		display.sendData((uint8_t *) &buff, display.MAX_COLS * 2);
	}
}

void dma2dTest(){

	sendDisplayBuffer();
	while(display.ongoingTx());

	uint16_t buff[display.MAX_COLS * display.MAX_ROWS] = {0};
	uint16_t red = _REV16(rgb565(31,0,0).data);
	uint16_t blue = _REV16(rgb565(0,0,31).data);
	for (uint32_t x = 0; x < display.MAX_COLS; x++){
		for (uint32_t y = 0; y < display.MAX_ROWS; y++){
			if (x < display.MAX_COLS / 2)
				buff[y*display.MAX_COLS + x] = red;
			else
				buff[y*display.MAX_COLS + x] = blue;
		}
	}

	sendDisplayBuffer();
	while(display.ongoingTx());

	reallocPeripheral(AHB3_Peripheral::dma2d);
	DMA2D_CR_CFG crcfg;
	crcfg.MODE = DMA2D_Mode::Mem2Mem_FGFetchOnly;
	dma2d_config(crcfg);

	DMA2D_PFC_CFG pfccfg;
	pfccfg.CM = DMA2D_PFC_ColorMode::RGB565;
	dma2d_fg_cfgPFC(pfccfg);

	dma2d_fg_setMemAddr((uint32_t) buff);
	dma2d_out_setMemAddr((uint32_t) displayBuffer);
	dma2d_fg_setOffset(0);
	dma2d_out_setOffset(0);
	dma2d_setNumLines(display.MAX_ROWS, display.MAX_COLS);

	dma2d_start();
	while(!dma2d_isr_status(DMA2D_ISR::TCIF));
	dma2d_isr_clear(DMA2D_ISR::TCIF);
	sendDisplayBuffer();
	while(display.ongoingTx());

	dma2d_fg_setMemAddr((uint32_t) buff);
	dma2d_out_setMemAddr((uint32_t) &displayBuffer[30]);
	dma2d_fg_setOffset(30);
	dma2d_out_setOffset(30);
	dma2d_setNumLines(display.MAX_ROWS, display.MAX_COLS - 30);

	dma2d_start();
	while(!dma2d_isr_status(DMA2D_ISR::TCIF));
	dma2d_isr_clear(DMA2D_ISR::TCIF);
	sendDisplayBuffer();
	while(display.ongoingTx());
}

//Returns x normalized and clamped within range parameters specified in the function body
//Output is between 0.0 and 1.0
float clampThenNormalize(float x, const float min, const float max){
	const float range = (max - min) > 0 ? (max - min) : (min - max);

	//Clamp x between max/min
	x = (x > max) ? max : x;
	x = (x < min) ? min : x;

	//Add to x absolute value of min
	x = (min < 0) ? x - min : x + min;

	return x / range;
}

rgb565 genColor(Msh lowMsh, Msh highMsh, float x){
	return sRGBCompanding(Msh2RGB(interpolateColor(lowMsh, highMsh, x)));
}

uint16_t _REV16(const uint16_t x){
	uint16_t res;
	__asm(
		"REV16 %[result], %[input_x]"
		: [result] "=r" (res)
		: [input_x] "r" (x)
	);
	return res;
}

void signal_M7(){
	HSEM_readLock(HSEM_CoreID::MASTER1, 1);
	HSEM_unlock(HSEM_CoreID::MASTER1, 0, 1);
}

void swapBuffers(){
	if (currentBuff == 0)
		currentBuff = 1;
	else if (currentBuff == 1)
		currentBuff = 0;
}
