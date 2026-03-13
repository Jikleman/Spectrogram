#include <stdint.h>
#include <nvic.h>
#include <rcc.h>
#include <adc.h>
#include <tim2345.h>
#include <gpio.h>
#include <dma.h>
#include <dmamux.h>
#include <hsem.h>

#include <fft.h>
#include <cmath>

#define shared	__attribute__ ((section(".shared_memory")))
#define dtcm	__attribute__ ((section(".dtcm_ram")))


//Shared data
//Make sure both cores have the same buffSize
//4096 words (16Kb) per buffer
volatile bool M4_Signal;
const uint32_t buffCol = 128;
const uint32_t buffRow = 4;
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

//const uint32_t N = 256;
//const uint32_t L = 128;
//const uint32_t O = 32;
//
//static_assert(N > L, "N cannot be less than L");
//static_assert(L > O, "FFT length (L) cannot be less than overlap (O)");

uint32_t adcBuff;

//Function Prototypes

void init_st7789v3();
void init_ADC();

void swapBuffers();
void signal_M4();

/*Attempts to swap between the shared buffers.
* If M4 is not ready for swap, swap fails and false is returned.
* True is returned on a successful swap.
*/
bool try_syncBufferSwap(){
	//Check if both CPUs are ready
	if(sharedBuff.M7_Done && sharedBuff.M4_Done) {
		//Switch buffers and do synchronization tasks
		swapBuffers();
		signal_M4();

		//Wait for M4 to be ready
		while(!M4_Signal) {};
		M4_Signal = false;
		sharedBuff.M7_Done = false;

		return true;
	}
	return false;
}

//Contains the initialization for shared buffer synchronization
void syncInit(){
	//Wait for M4 to be ready
	while(!HSEM_isC2ItrEnabled(0));
	allocatePeripheral(AHB4_Peripheral::hsem);
	nvic_enableItr(H755_itrPos::hsem0);
	HSEM_enableC1Itr(1);

	M4_Signal = false;
	currentBuff = 1;
	sharedBuff.M7_Done = false;
}

//IRQ handlers

extern "C" {
void ADC1_2_IRQHandler(void){

}
}

extern "C"{
void HSEM0_IRQHandler(void){
	M4_Signal = true;
	HSEM_clearC1Flag(1);
}
}

int main(void)
{
	//	reallocPeripheral(APB1L_Peripheral::tim3);
	//	tim_prescale(TIM3, 64); //Timer 3 set to 1MHz
	//	tim_enable(TIM3);
	//	init_ADC();
	//	adc_start(ADC1);

	uint32_t Fs = 10000;
	uint32_t freq = 1000;
	float theta = (2.0f * MATH_PI * freq) / Fs;

	constexpr uint32_t N = 512;
	constexpr uint32_t L = 128;
	constexpr uint32_t O = 64;
	constexpr uint32_t H = L - O;
	constexpr uint32_t frames = 1 + (N-L) / (H);
	constexpr uint32_t l = L/2 + 1; //Only the first half + 1 of each frame is needed for the spectrogram

	static dtcm float x[N];
	static dtcm float w[L];
	static dtcm complex v[L];
	static dtcm float X[frames][l] = {0};

	hanning(L, w);

	//Wait for M4 to be ready then signal to M4
	syncInit();

	uint32_t k = 0;
	for (uint32_t n = 0; n < N; n++){
		x[n] = cosf(theta * k);
		k++;
	}
	uint32_t sharedBuffIdx = 0;
	const static auto calcSpec = [&](){
		//Compute spectrogram
		for (uint32_t f = 0; f < frames; f++){
			for (uint32_t m = 0; m < L; m++){
				v[m].real = w[m] * x[m + f*H];
				v[m].imag = 0;
			}
			fft<L>(v);
			reversePermute(v, L, log2floor(L));
			magnitude(v, X[f], l);
		}
	};

	const static auto prepare_x = [&](){
		//Copy the elements from x needed for the overlap of the next frame to the start
		for (uint32_t n = 0; n < O; n++){
			x[n] = x[N - O + n];
		}
		//Write the elements for next frames
		for (uint32_t n = O; n < N; n++){
			x[n] = cosf(theta * k);
			k++;
		}
	};

	while(true){
		calcSpec();
		prepare_x();

		bool transferingData = true;
		uint32_t f = 0;
		uint32_t n = 0;

		struct excessDataInfo{
			uint32_t f;
			uint32_t n;
		};

		//Lambda for moving data to shared buffer. Used for return and capture functionality
		const static auto moveData = [&](volatile float* buff) -> excessDataInfo{
			for (;f < frames; f++){
				for (;n < l; n++){
					buff[sharedBuffIdx++] = X[f][n];
					if (sharedBuffIdx == buffSize){
						return excessDataInfo {f, n + 1};
					}
				}
			}
			return excessDataInfo {f,n};
		};

		while(transferingData){
			excessDataInfo info;
			if (currentBuff == 0){
				info = moveData(sharedBuff.buff0);
			}
			if (currentBuff == 1){
				info = moveData(sharedBuff.buff1);
			}

			//If buffer is full, wait to swap buffers with M4 then finish sending data.
			if (info.f < (frames-1) || info.n < (l-1)){
				sharedBuffIdx = 0;
				sharedBuff.M7_Done = true;
				while(!try_syncBufferSwap()) {}
			} else {
				transferingData = false;
			}
		}
	}
}

//Function Implementations

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

void swapBuffers(){
	if (currentBuff == 0)
		currentBuff = 1;
	else if (currentBuff == 1)
		currentBuff = 0;
}

void signal_M4(){
	HSEM_readLock(HSEM_CoreID::MASTER0, 0);
	HSEM_unlock(HSEM_CoreID::MASTER0, 0, 0);
}

//void spectrogramTest(){
//	const int N = 512;
//	const int L = 128;
//	const int O = 115;
//
//	uint32_t freq = 1000;
//	uint32_t Fs = 16000;
//	float theta = (2.0 * MATH_PI * freq) / Fs;
//
//	complex x[N];
//	for (uint32_t n = 0; n < N; n++){
//		if (n % 100 < 50)
//			x[n].real = 1;
//		else
//			x[n].real = -1;
//		x[n].imag = 0;
//	}
//
//    float w[L];
//    uint32_t H = L - O;
//    uint32_t frames = 1 + (N-L) / (H);
//    uint32_t l = L/2 + 1; //Only the first half + 1 of each frame is needed for the spectrogram
//
//    complex v[L] = {0};
//    hanning(L,w);
//    float X[frames][l] = {0};
//
//    for (uint32_t f = 0; f < frames; f++){
//        for (int m = 0; m < L; m++){
//            v[m].real = w[m] * x[m + f*H].real;
//            v[m].imag = 0;
//        }
//        fft<L>(v);
//        reversePermute(v, L, log2floor(L));
//        magnitude(v, X[f], l);
//    }
//
//
//    const Msh highMsh = RGB2Msh(invsRGBCompanding(rgb565(27,5,2)));
//    const Msh lowMsh = RGB2Msh(invsRGBCompanding(rgb565(3,7,24)));
//
//    rgb565 specRGB[frames][l];
//
//	const float max = 10.0;
//	const float min = 0.0;
//	for (uint32_t  f = 0; f < frames; f++){
//		for (uint32_t i = 0; i < l; i++){
//			X[f][i] = clampThenNormalize(X[f][i], min, max);
//			specRGB[f][i] = genColor(lowMsh, highMsh, X[f][i]);
//		}
//	}
//
//	//Black out screen
//	display.fill(0x0000);
//
//	const uint32_t sizeX = 4;
//	const uint32_t sizeY = 4;
//	for (uint32_t f = 0; f < frames; f++){
//		uint32_t xIdx = f * sizeX;
//		for (uint32_t i = 0; i < l; i++){
//			uint32_t yIdx = i * sizeY;
//			uint16_t data = _REV16(specRGB[f][i].data);
//			for (uint32_t y = yIdx; y < sizeY + yIdx; y++){
//				for (uint32_t x = xIdx; x < sizeX + xIdx; x++){
//					displayBuffer[y*display.MAX_COLS + x] = data;
//				}
//			}
//		}
//	}
//
//	sendDisplayBuffer();
//}
