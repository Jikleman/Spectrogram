#pragma once

//Only has configurations for Master right now, and CRC configurations are ignored.

#include <stdint.h>

#define SPI1_BASE				(0x40013000)
#define SPI2_BASE			    (0x40003800)
#define SPI3_BASE              	(0x40003C00)
#define SPI4_BASE				(0x40013400)
#define SPI5_BASE				(0x40015000)
#define SPI6_BASE				(0x58001400)

struct SPIStruct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CFG1;
    volatile uint32_t CFG2;
    volatile uint32_t IER;
    volatile uint32_t SR;
    volatile uint32_t IFCR;
    volatile uint32_t Reserved0;	//Not documented specifically, but there is a gap between these two registers
    volatile uint32_t TXDR;
    volatile uint32_t Reserved1[3];
    volatile uint32_t RXDR;
    volatile uint32_t Reserved2[3];
    volatile uint32_t CRCPOLY;
    volatile uint32_t TXCRC;
    volatile uint32_t RXCRC;
    volatile uint32_t UDRDR;
    volatile uint32_t I2SCFGR;
};

#define SPI1                    ((SPIStruct *) SPI1_BASE)
#define SPI2                    ((SPIStruct *) SPI2_BASE)
#define SPI3                    ((SPIStruct *) SPI3_BASE)
#define SPI4                    ((SPIStruct *) SPI4_BASE)
#define SPI5                    ((SPIStruct *) SPI5_BASE)
#define SPI6                    ((SPIStruct *) SPI6_BASE)

/* SPI123:
 *  fifo can hold 16 bytes
 *  max data and crc size is 32 bits
 *  can be configured as I2S
 * SPI456:
 *  fifo can hold 8 bytes
 *  max data and crc size is 16 bits
*/
enum class SPI_Mode{
    Mode0 = 0,
    Mode1 = 1,
    Mode2 = 2,
    Mode3 = 3,
};

enum class SPI_CommMode{
    FullDuplex = 0,
    Simplex,
    HalfDuplex = 3
};

enum class SPI_Protocol{
    Motorola = 0,
    TI = 1
};

enum class SPI_SSPolarity{
    Low = 0,
    High = 1
};

enum class SPI_DataFrameFormat{
    MSBFirst = 0,
    LSBFirst = 1
};

enum class SPI_MasterBaudRatePresc{
    Div2 = 0,
    Div4 = 1,
    Div8 = 2,
    Div16 = 3,
    Div32 = 4,
    Div64 = 5,
    Div128 = 6,
    Div256 = 7,
};

//EOT, SUSP, and TXC share the same interrupt.
enum class SPI_Event {
	RXP = 0,            //Data packet available in RxFIFO
	TXP = 1,            //Space available for data packet in TxFIFO
	DXP = 2,            //Both TXP and RXP are active
	EOT = 3,            //End of transfer
	TXTF = 4,           //Transmission transfer filled. (When all data to be transmitted has been written by DMA or user)
	UDR = 5,            //Underrun
	OVR = 6,            //Overrun
	CRCE = 7,           //CRC error
	TIFRE = 8,          //TI frame format error
	MODF = 9,           //Mode fault
	TSERF = 10,         //When TSER value loaded to TSIZE. (TSER is ready to take a new value)
	SUSP = 11,          //Master mode suspended
	TXC = 12            //TxFIFO transmission complete (When the TxFIFO is empty)
};

struct SPI_MasterCFG{
    SPI_Mode spiMode;                   //CPOL and CPHA configurations
    SPI_SSPolarity ssPol;               //Decides if the active SS signal is defined as high or low
    SPI_DataFrameFormat format;
    SPI_CommMode commMode;
    SPI_Protocol protocol;
    SPI_MasterBaudRatePresc masterPresc;
    bool enableSSOutput;                //Disable for multi-master configuration. Enable so that SS is made active while transmitting data.
    bool periphControlsGPIOAltFunc;     //Peripheral controls the relevant GPIOs even if the SPI is disabled.
    bool swapMOSI_MISO;                 //Swaps the MOSI and MISO Pins
    bool enableTxDMA;
    bool enableRxDMA;
    uint32_t interDataIdleness : 4;         //Number of clock cycles as extra delay between data frames
    uint32_t SSIdleness : 4;                //Number of clock cycles as extra delay between active edge of SS and first data frame
    uint32_t fifoThresholdLevel : 4;    //Number of data frames per data packet plus 1. Packet size cannot be more than half of FIFO space.
    uint32_t bitsPerDataFrame : 5;      //Numbers of bits + 1 per data frame. Cannot have less than 4 bits per frame.

    SPI_MasterCFG() :
        spiMode(SPI_Mode::Mode0), ssPol(SPI_SSPolarity::Low), format(SPI_DataFrameFormat::MSBFirst), commMode(SPI_CommMode::FullDuplex), protocol(SPI_Protocol::Motorola), masterPresc(SPI_MasterBaudRatePresc::Div2),
        enableSSOutput(false), periphControlsGPIOAltFunc(false), swapMOSI_MISO(false), enableTxDMA(false), enableRxDMA(false), interDataIdleness(0), SSIdleness(0), fifoThresholdLevel(1 - 1),
        bitsPerDataFrame(8 - 1)
    {}
};

void spi_config(SPI_MasterCFG cfg, SPIStruct* SPI);

void spi_enableItr(SPIStruct* SPIx, SPI_Event event);
void spi_disableItr(SPIStruct* SPIx, SPI_Event event);
void spi_clearFlag(SPIStruct* SPIx, SPI_Event event);
bool spi_readFlag(SPIStruct* SPIx, SPI_Event event);

void spi_iolock(SPIStruct* SPIx);
void spi_enable(SPIStruct* SPIx);
void spi_disable(SPIStruct* SPIx);
void spi_masterStart(SPIStruct* SPIx);
void spi_masterSuspend(SPIStruct* SPIx);
void spi_setTSize(SPIStruct* SPIx, uint16_t tsize);
void spi_setTSER(SPIStruct* SPIx, uint16_t tser);
void spi_transmitData(SPIStruct* SPIx, uint32_t data);

void spi_halfDuplex_transmitter(SPIStruct* SPIx);
void spi_halfDuplex_receiver(SPIStruct* SPIx);

void spi_disableTXDMA();
void spi_enableTXDMA();

