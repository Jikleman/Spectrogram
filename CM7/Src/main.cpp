#include <stdint.h>
#include <st7789v3.h>
#include <nvic.h>
#include <rcc.h>
#include <adc.h>
#include <tim2345.h>
#include <gpio.h>
#include <dma.h>
#include <dmamux.h>

#include <fft.h>
#include <color.h>

#include <cmath>

#include <dma2d.h>

extern "C" {
void ADC1_2_IRQHandler(void){

}
}

//Global Variables

st7789v3 display;
uint16_t displayBuffer [display.MAX_ROWS * display.MAX_COLS] = {0};
uint32_t adcBuff[2*100];

//Function Prototypes

void init_st7789v3();
void init_ADC();

uint16_t _REV16(const uint16_t x);
float clampThenNormalize(float x, const float min, const float max);
rgb565 genColor(Msh lowMsh, Msh highMsh, float x);
void displayColorMap(Msh lowMsh, Msh highMsh);
void sendDisplayBuffer();
void spectrogramTest();

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


int main(void)
{
	//	reallocPeripheral(APB1L_Peripheral::tim3);
	//	tim_prescale(TIM3, 64); //Timer 3 set to 1MHz
	//	tim_enable(TIM3);
	//	init_ADC();
	//	adc_start(ADC1);
	init_st7789v3();
//	dma2dTest();
	while(true){

	}
}

//Function Implementations

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

void init_ADC(){
	reallocPeripheral(AHB1_Peripheral::adc1_adc2);
	reallocPeripheral(AHB1_Peripheral::dma1);
	reallocPeripheral(AHB4_Peripheral::gpioa);
	reallocPeripheral(APB1L_Peripheral::tim2);

	//AR value set to overrun one times a second;
	tim_auto_reload(TIM2, 64000000);
	TIMSettingsCR s;
	s.MMS = tim_mms::Update;
	tim_controls(TIM2, s);

	DMAMUX1_ChannelCfg muxC;
	muxC.channel = DMAMUX1_Channel::Ch1;
	muxC.inputRequest = DMAMUX1_MultiplexerInput::adc1_dma;
	muxC.numberForwardDMARequests = 1 - 1;
	dmamux_configChannel(muxC);

	DMA_StreamCfg_PerToMem dmaC;
	dmaC.stream = DMA_Stream::Stream1;
	dmaC.fifoThreshold = DMA_FifoThreshold::Full;
	dmaC.numberOfDataToTransfer = 4;
	dmaC.enableMemIncr = true;
	dmaC.enablePeriphMemIncr = false;
	dmaC.isCircularMode = true;
	dmaC.memory0Addr = (uint32_t) &adcBuff;
	dmaC.periphMemoryAddr = (uint32_t) &(ADC1->DR);
	dmaC.memoryDataSize = DMA_DataSize::Word;
	dmaC.periphDataSize = DMA_DataSize::Word;
	dmaC.memoryBurst = DMA_BurstType::SingleTransfer;
	dmaC.periphBurst = DMA_BurstType::SingleTransfer;
	dma_configStream(dmaC, DMA1);

	adcCom_set_ckmode(ADC12_COMMON, SYNC_DIV2);
	adc_preselectCh(ADC1, 15);
	adc_set_regSeq(ADC1, 1, 15);
	adc_set_chSampleTime(ADC1, 0b111, 15);
	adc_set_boost(ADC1, boost11);
	adc_wake(ADC1);
	adc_calibrate(ADC1, SingleEnded, true);
	ADCSettingsCFGR c;
	c.EXTEN = 1; //Hardware trigger on rising edge
	c.EXTSEL= 11; //Select external trigger event 11 (tim2_trgo)
	c.DMNGT = 0b11;
	adc_set_configurations1(ADC1, c);
	adc_enable(ADC1);

	tim_enable(TIM2);
	dma1_enableStream(DMA_Stream::Stream1);
}

//Returns the reverse byte order 16-bit data
uint16_t _REV16(const uint16_t x){
	uint16_t res;
	__asm(
		"REV16 %[result], %[input_x]"
		: [result] "=r" (res)
		: [input_x] "r" (x)
	);
	return res;
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

void spectrogramTest(){
	const int N = 512;
	const int L = 128;
	const int O = 115;

	uint32_t freq = 1000;
	uint32_t Fs = 16000;
	float theta = (2.0 * MATH_PI * freq) / Fs;

	complex x[N];
	for (uint32_t n = 0; n < N; n++){
		if (n % 100 < 50)
			x[n].real = 1;
		else
			x[n].real = -1;
		x[n].imag = 0;
	}

    float w[L];
    uint32_t H = L - O;
    uint32_t frames = 1 + (N-L) / (H);
    uint32_t l = L/2 + 1; //Only the first half + 1 of each frame is needed for the spectrogram

    complex v[L] = {0};
    hanning(L,w);
    float X[frames][l] = {0};

    for (uint32_t f = 0; f < frames; f++){
        for (int m = 0; m < L; m++){
            v[m].real = w[m] * x[m + f*H].real;
            v[m].imag = 0;
        }
        fft<L>(v);
        reversePermute(v, L, log2floor(L));
        magnitude(v, X[f], l);
    }


    const Msh highMsh = RGB2Msh(invsRGBCompanding(rgb565(27,5,2)));
    const Msh lowMsh = RGB2Msh(invsRGBCompanding(rgb565(3,7,24)));

    rgb565 specRGB[frames][l];

	const float max = 10.0;
	const float min = 0.0;
	for (uint32_t  f = 0; f < frames; f++){
		for (uint32_t i = 0; i < l; i++){
			X[f][i] = clampThenNormalize(X[f][i], min, max);
			specRGB[f][i] = genColor(lowMsh, highMsh, X[f][i]);
		}
	}

	//Black out screen
	display.fill(0x0000);

	const uint32_t sizeX = 4;
	const uint32_t sizeY = 4;
	for (uint32_t f = 0; f < frames; f++){
		uint32_t xIdx = f * sizeX;
		for (uint32_t i = 0; i < l; i++){
			uint32_t yIdx = i * sizeY;
			uint16_t data = _REV16(specRGB[f][i].data);
			for (uint32_t y = yIdx; y < sizeY + yIdx; y++){
				for (uint32_t x = xIdx; x < sizeX + xIdx; x++){
					displayBuffer[y*display.MAX_COLS + x] = data;
				}
			}
		}
	}

	sendDisplayBuffer();
}
