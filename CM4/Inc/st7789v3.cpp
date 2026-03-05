#include <st7789v3.h>
#include <rcc.h>
#include <gpio.h>
#include <spi.h>
#include <dma.h>
#include <dmamux.h>

#define SCK_PORT		GPIOC
#define SCK_PIN			GPIO_Pin::P10
#define MOSI_PORT		GPIOC
#define MOSI_PIN		GPIO_Pin::P11
#define NSS_PORT		GPIOA
#define NSS_PIN			GPIO_Pin::P15

#define DC_PORT			GPIOB
#define DC_PIN			GPIO_Pin::P12
#define RESET_PORT		GPIOC
#define RESET_PIN		GPIO_Pin::P12
#define BLK_PORT		GPIOC
#define BLK_PIN			GPIO_Pin::P7

void configureGPIO(){
    //PinsControlled by SPI Peripheral
    GPIO_PinCFG pinCfg;
    pinCfg.AF = GPIO_AltFunc::AF6;
    pinCfg.mode = GPIO_PinMode::AltFunc;
    pinCfg.ospeed = GPIO_OutputSpeed::Low;
    pinCfg.otype = GPIO_OutputType::PushPull;
    pinCfg.pull = GPIO_PinPull::None;

	gpio_configPin(SCK_PORT, SCK_PIN, pinCfg);   	//SCK
    gpio_configPin(MOSI_PORT, MOSI_PIN, pinCfg);   	//MISO (Swapped to MOSI)
    gpio_configPin(NSS_PORT, NSS_PIN, pinCfg);   	//NSS

    //Pins Controlled by software
    pinCfg.mode = GPIO_PinMode::Output;
    pinCfg.AF = GPIO_AltFunc::AF0;

    gpio_configPin(DC_PORT, DC_PIN, pinCfg);   		//DC
    gpio_configPin(RESET_PORT, RESET_PIN, pinCfg);  //Reset
    gpio_configPin(BLK_PORT, BLK_PIN, pinCfg);    	//BLK  (Back light enable)
}

void configureSPI(){
	RCC->D2CCIP1R |= (0b100<<12);	//Select Per_CK for SPI123 Ker clock
	SPI_MasterCFG SPIcfg;
	SPIcfg.spiMode = SPI_Mode::Mode0;
	SPIcfg.ssPol = SPI_SSPolarity::Low;
	SPIcfg.commMode = SPI_CommMode::Simplex;
	SPIcfg.masterPresc = SPI_MasterBaudRatePresc::Div32;
	SPIcfg.enableSSOutput = true;
	SPIcfg.periphControlsGPIOAltFunc = true;
	SPIcfg.enableTxDMA = true;
	SPIcfg.swapMOSI_MISO = true;
	SPIcfg.interDataIdleness = 0;
	SPIcfg.SSIdleness = 0;
	SPIcfg.bitsPerDataFrame = 8 - 1;
	spi_config(SPIcfg, SPI3);
}

void configureDMA(){
	//DMA configurations
	DMA_StreamCfg_MemToPer DMAcfg;
	DMAcfg.stream = DMA_Stream::Stream0;
	DMAcfg.fifoThreshold = DMA_FifoThreshold::Full;
	DMAcfg.numberOfDataToTransfer = 0;
	DMAcfg.enableMemIncr = true;
	DMAcfg.enablePeriphMemIncr = false;
	DMAcfg.isCircularMode = false;
	DMAcfg.isDoubleBuffer = false;
	DMAcfg.isNotDirectMode = true;
	DMAcfg.memory0Addr = 0;
	DMAcfg.startMemoryTarget = DMA_MemoryStart::Memory0;
	DMAcfg.periphMemoryAddr = (uint32_t) &SPI3->TXDR;
	DMAcfg.memoryDataSize = DMA_DataSize::Byte;
	DMAcfg.periphDataSize = DMA_DataSize::Byte;
	DMAcfg.memoryBurst = DMA_BurstType::SingleTransfer;
	DMAcfg.periphBurst = DMA_BurstType::SingleTransfer;
	dma_configStream(DMAcfg, DMA1);
}

//Initializes the peripherals needed for the peripheral.
void st7789v3::initPeripherals(){
	//Reset (if not already allocated) and allocate peripherals used for the display
	reallocPeripheral(APB1L_Peripheral::spi3);
	reallocPeripheral(AHB1_Peripheral::dma1);
	reallocPeripheral(AHB4_Peripheral::gpioa);
	reallocPeripheral(AHB4_Peripheral::gpiob);
	reallocPeripheral(AHB4_Peripheral::gpioc);

	configureGPIO();
	configureSPI();
	configureDMA();

	//DMAMUX configurations
	DMAMUX1_ChannelCfg MUXcfg;
	MUXcfg.channel = DMAMUX1_Channel::Ch0;
	MUXcfg.inputRequest = DMAMUX1_MultiplexerInput::spi3_tx_dma;
	MUXcfg.numberForwardDMARequests = 1 - 1;
	dmamux_configChannel(MUXcfg);
}

//Sets the DC line to indicate incoming data frames are a command
void st7789v3::setDCLine(){
    gpio_setPin(DC_PORT, DC_PIN);
}

///Sets the DC line to indicate incoming data frames are data
void st7789v3::resetDCLine(){
    gpio_resetPin(DC_PORT, DC_PIN);
}

//Disables the reset pin
void st7789v3::setRstPin(){
    gpio_setPin(RESET_PORT, RESET_PIN);
}

//Enables the reset pin
void st7789v3::resetRstPin(){
    gpio_resetPin(RESET_PORT, RESET_PIN);
}

void st7789v3::enableBacklight(){
    gpio_setPin(BLK_PORT, BLK_PIN);
}

void st7789v3::disableBacklight(){
    gpio_resetPin(BLK_PORT, BLK_PIN);
}

void inline disableSPI_DMA(){
	dma1_disableStream(DMA_Stream::Stream0);
	spi_disable(SPI3);
	SPI3->CFG1 &= ~(1<<15);	//disable TXDAMEN
}

void inline enableSPI_DMA(){
	SPI3->CFG1 |= (1<<15); //enable TXDAMEN
	spi_enable(SPI3);
	spi_masterStart(SPI3);
}

void st7789v3::sendCommand(st7789v3::commands command, bool waitForTx){
	if (waitForTx){
		waitTxComplete();
	}
	resetDCLine();
	spi_clearFlag(SPI3, SPI_Event::TXTF);
	spi_clearFlag(SPI3, SPI_Event::EOT);

	uint8_t commandBuff = (uint8_t) command;

	disableSPI_DMA();

	DMA1->S0M0AR = (uint32_t) &commandBuff;
	dma1_clearAllFlags(DMA_Stream::Stream0);
	dma1_setDataTransferSize(DMA_Stream::Stream0, 1);
	dma1_enableStream(DMA_Stream::Stream0);
	spi_setTSize(SPI3, 1);

	enableSPI_DMA();
}

//Can only send up to a maximum value of 65535 data frames per call. Any data chunking should be done by the user.
void st7789v3::sendData(uint8_t* buff, uint32_t dataLength, bool waitForTx){
	if (waitForTx){
		waitTxComplete();
	}
	setDCLine();
	spi_clearFlag(SPI3, SPI_Event::TXTF);
	spi_clearFlag(SPI3, SPI_Event::EOT);

	disableSPI_DMA();

	DMA1->S0M0AR = (uint32_t) buff;
	dma1_clearAllFlags(DMA_Stream::Stream0);
	dma1_setDataTransferSize(DMA_Stream::Stream0, dataLength);
	dma1_enableStream(DMA_Stream::Stream0);
	spi_setTSize(SPI3, dataLength);

	enableSPI_DMA();
}

void st7789v3::waitTxComplete(){
	while (!spi_readFlag(SPI3, SPI_Event::EOT));
	while (!spi_readFlag(SPI3, SPI_Event::TXTF));
}

bool st7789v3::ongoingTx(){
	return !(spi_readFlag(SPI3, SPI_Event::EOT) && spi_readFlag(SPI3, SPI_Event::TXTF));
}
