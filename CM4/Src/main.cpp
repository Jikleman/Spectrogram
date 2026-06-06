#include <dma2d.h>
#include <hsem.h>
#include <nvic.h>
#include <rcc.h>

#include <double_buffer.h>
#include <cmath>
#include <color.h>
#include <st7789v3.h>
#include <stdint.h>

	constexpr uint32_t L = 256;
	constexpr uint32_t l = L/2 + 1; //Only the first half + 1 of each frame is needed for the spectrogram

	const uint32_t size_x = 5;
	const uint32_t size_y = 2;


//Global Variables
st7789v3 display;
uint16_t displayBuffer[display.MAX_COLS * display.MAX_ROWS];

const uint32_t tx_hsem = 0;
const uint32_t rx_hsem = 1;
const uint32_t dbuff_len = 512;
shared double_buffer<float, dbuff_len> dbuff(tx_hsem, rx_hsem);

//Function prototypes
void sendDisplayBuffer();
void dma2dTest();
void displayColorMap(Msh lowMsh, Msh highMsh);

float clampThenNormalize(float x, const float min, const float max);
rgb565 genColor(Msh lowMsh, Msh highMsh, float x);
uint16_t _REV16(const uint16_t x);

void init_st7789v3();
void M4_startup_sync();

//IRQ handlers
extern "C"{
void HSEM1_IRQHandler(void){

}
}

#define CPACR				(*(volatile unsigned int *) 0xE000ED88)

int main(void)
{
	M4_startup_sync();

	//Set full access privilege to enable FPU
	CPACR |= (0xF << 20);

	init_st7789v3();

	display.fill(0x0000);

	const Msh highMsh = RGB2Msh(invsRGBCompanding(rgb565(27,5,2)));
	const Msh lowMsh = RGB2Msh(invsRGBCompanding(rgb565(3,7,24)));

	const float max = 5.0f;
	const float min = 0.0f;

	float X[dbuff_len];
	uint16_t spec_frame[l];
	uint32_t n = 0;
	uint32_t x_cur = 0;
	while(true){
		dbuff.rx_swap_ready();
		dbuff.rx_read(X, 0, dbuff_len);

		for (uint32_t i = 0; i < dbuff_len; i++){
			float normalized_data = clampThenNormalize(X[i], min, max);
			uint16_t frame_data = genColor(lowMsh, highMsh, normalized_data).data;
			spec_frame[n++] = _REV16(frame_data);

			if (n == l){
				for (uint32_t j = 0; j < l; j++){
					uint32_t y_cur = j * size_y;
					for (uint32_t y = y_cur; y < size_y + y_cur; y++){
						for (uint32_t x = x_cur; x < size_x + x_cur; x++){
							displayBuffer[y*display.MAX_COLS + (x % display.MAX_COLS)] = spec_frame[j];
						}
					}
				}
				n = 0;
				x_cur += size_x;
				x_cur %= display.MAX_COLS;
				sendDisplayBuffer();
			}
		}
	}


//	uint32_t dispBuffX = 0;
//	while(true){
//		//double buffer swap goes here
//		dbuff.rx_swap_ready();
//		dbuff.rx_read(X, 0, 10);
//
//		const float max = 10.0;
//		const float min = 0.0;
//
//		for (uint32_t  f = 0; f < frames; f++){
//			for (uint32_t i = 0; i < l; i++){
//				X[f][i] = clampThenNormalize(X[f][i], min, max);
//				specRGB[f][i] = genColor(lowMsh, highMsh, X[f][i]);
//			}
//		}
//		const uint32_t sizeX = 4;
//		const uint32_t sizeY = 4;
//		for (uint32_t f = 0; f < frames; f++){
//			uint32_t xIdx = f * sizeX + dispBuffX;
//			for (uint32_t i = 0; i < l; i++){
//				uint32_t yIdx = i * sizeY;
//				uint16_t data = _REV16(specRGB[f][i].data);
//				for (uint32_t y = yIdx; y < sizeY + yIdx; y++){
//					for (uint32_t x = xIdx; x < sizeX + xIdx; x++){
//						displayBuffer[y*display.MAX_COLS + x] = data;
//					}
//				}
//			}
//		}
//		dispBuffX = dispBuffX + frames * sizeX;
//		dispBuffX = dispBuffX % display.MAX_COLS;
//
//		sendDisplayBuffer();
//	}
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

void M4_startup_sync(){
	//Wait for M7 to be ready
	while(!HSEM_isC1ItrEnabled(0));
	allocatePeripheral(AHB4_Peripheral::hsem);
	HSEM_disableC1Itr(0);

	//Signal M7
	HSEM_enableC2Itr(0);
}
