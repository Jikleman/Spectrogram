#pragma once

#include <stdint.h>

#define DMA1_BASE				(0x40020000)
#define DMA2_BASE				(0x40020400)

struct DMAStruct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    //Stream 0
    volatile uint32_t S0CR;
    volatile uint32_t S0NDTR;
    volatile uint32_t S0PAR;
    volatile uint32_t S0M0AR;
    volatile uint32_t S0M1AR;
    volatile uint32_t S0FCR;
    //Stream 1
    volatile uint32_t S1CR;
    volatile uint32_t S1NDTR;
    volatile uint32_t S1PAR;
    volatile uint32_t S1M0AR;
    volatile uint32_t S1M1AR;
    volatile uint32_t S1FCR;
    //Stream 2
    volatile uint32_t S2CR;
    volatile uint32_t S2NDTR;
    volatile uint32_t S2PAR;
    volatile uint32_t S2M0AR;
    volatile uint32_t S2M1AR;
    volatile uint32_t S2FCR;
    //Stream 3
    volatile uint32_t S3CR;
    volatile uint32_t S3NDTR;
    volatile uint32_t S3PAR;
    volatile uint32_t S3M0AR;
    volatile uint32_t S3M1AR;
    volatile uint32_t S3FCR;
    //Stream 4
    volatile uint32_t S4CR;
    volatile uint32_t S4NDTR;
    volatile uint32_t S4PAR;
    volatile uint32_t S4M0AR;
    volatile uint32_t S4M1AR;
    volatile uint32_t S4FCR;
    //Stream 5
    volatile uint32_t S5CR;
    volatile uint32_t S5NDTR;
    volatile uint32_t S5PAR;
    volatile uint32_t S5M0AR;
    volatile uint32_t S5M1AR;
    volatile uint32_t S5FCR;
    //Stream 6
    volatile uint32_t S6CR;
    volatile uint32_t S6NDTR;
    volatile uint32_t S6PAR;
    volatile uint32_t S6M0AR;
    volatile uint32_t S6M1AR;
    volatile uint32_t S6FCR;
    //Stream 7
    volatile uint32_t S7CR;
    volatile uint32_t S7NDTR;
    volatile uint32_t S7PAR;
    volatile uint32_t S7M0AR;
    volatile uint32_t S7M1AR;
    volatile uint32_t S7FCR;
};

#define DMA1                ((DMAStruct *) DMA1_BASE)
#define DMA2                ((DMAStruct *) DMA1_BASE)

enum class DMA_Stream{
    Stream0 = 0,
    Stream1 = 1,
    Stream2 = 2,
    Stream3 = 3,
    Stream4 = 4,
    Stream5 = 5,
    Stream6 = 6,
    Stream7 = 7
};

enum class DMA_ItrStatus{
    FIFOError = 0,
    DirectModeError = 2,
    TransferError = 3,
    HalfTransfer = 4,
    TransferComplete = 5
};

bool dma1_readItrFlag(DMA_Stream stream, DMA_ItrStatus flag);
bool dma2_readItrFlag(DMA_Stream stream, DMA_ItrStatus flag);
void dma1_clearItrFlag(DMA_Stream stream, DMA_ItrStatus flag);
void dma2_clearItrFlag(DMA_Stream stream, DMA_ItrStatus flag);
void dma1_clearAllFlags(DMA_Stream stream);
void dma2_clearAllFlags(DMA_Stream stream);

enum class DMA_PriorityLevel{
    Low = 0,
    Medium = 1,
    High = 2,
    VeryHigh = 3
};

//The FIFO can hold up to 4 words.
enum class DMA_FifoThreshold{
    FourthFull = 0,
    HalfFull = 1,
    ThreeFourthFull = 2,
    Full = 3
};

enum class DMA_BurstType{
	SingleTransfer = 0,
	INCR4 = 1,
	INCR8 = 2,
	INCR16 = 3
};

enum class DMA_DataSize{
	Byte = 0,
	HalfWord = 1,
	Word = 2
};

enum class DMA_MemoryStart{
    Memory0 = 0,
    Memory1 = 1
};

struct DMA_StreamCfg_MemToMem {
    DMA_Stream stream;
    DMA_FifoThreshold fifoThreshold;
    DMA_PriorityLevel priority;
    uint16_t numberOfDataToTransfer;
    uint32_t sourceMemoryAddr;
    uint32_t destinMemoryAddr;
    bool enableSourceMemIncr;
    bool enableDestinMemIncr;
    bool isSourceIncrFixedOffset;             //True means offset is fixed to 4 bytes. False means it depends on the source size.
    DMA_BurstType sourceBurst;
    DMA_BurstType destinBurst;
    DMA_DataSize sourceDataSize;
    DMA_DataSize destinDataSize;

    bool enableTransferCompleteItr;
    bool enableHalftransferItr;
    bool enableTransferErrorItr;
    bool enableFIFOErrorItr;
    bool allowBufferableTransfers;  //Must be set to 1 if DMA manages any kind of USART/UART transfer

    //Default values for the SxCR and SxFCR. The stream doesn't have a default value.
    DMA_StreamCfg_MemToMem() :
        fifoThreshold(DMA_FifoThreshold::HalfFull), priority(DMA_PriorityLevel::Low), numberOfDataToTransfer(0), sourceMemoryAddr(0), destinMemoryAddr(0),
        enableSourceMemIncr(false), enableDestinMemIncr(false), isSourceIncrFixedOffset(false), sourceBurst(DMA_BurstType::SingleTransfer), destinBurst(DMA_BurstType::SingleTransfer),
        sourceDataSize(DMA_DataSize::Byte), destinDataSize(DMA_DataSize::Byte), enableTransferCompleteItr(false), enableHalftransferItr(false),
        enableTransferErrorItr(false), enableFIFOErrorItr(false), allowBufferableTransfers(false)
    {}
};

struct DMA_StreamCfg_PeripheralAndMemory {
    DMA_Stream stream;
    DMA_FifoThreshold fifoThreshold;		//This field has no effect in direct mode.
    DMA_PriorityLevel priority;
    uint16_t numberOfDataToTransfer;
    uint32_t periphMemoryAddr;
    uint32_t memory0Addr;
    uint32_t memory1Addr;
    bool enablePeriphMemIncr;
    bool enableMemIncr;
    bool isPeriIncrFixedOffset;             //True means offset is fixed to 4 bytes. False means it depends on the source size.
    DMA_BurstType periphBurst;
    DMA_BurstType memoryBurst;
    DMA_DataSize periphDataSize;
    DMA_DataSize memoryDataSize;
    bool isPeriphFlowController;
    bool isNotDirectMode;					//In direct mode only single transfers occur
    bool isCircularMode;					//Set to 0 by hardware if peripheral is flow controller. Set to 1 by hardware if double buffer mode is enabled.
    bool isDoubleBuffer;
    DMA_MemoryStart startMemoryTarget; 		//Matters for double buffer mode

    bool enableTransferCompleteItr;
    bool enableHalftransferItr;
    bool enableTransferErrorItr;
    bool enableFIFOErrorItr;
    bool enableDirectModeErrorItr;
    bool allowBufferableTransfers;          //Must be set to 1 if DMA manages any kind of USART/UART transfer

    //Default values for SxCR and SxFCR. The stream doesn't have a default value.
    DMA_StreamCfg_PeripheralAndMemory() :
        fifoThreshold(DMA_FifoThreshold::HalfFull), priority(DMA_PriorityLevel::Low), numberOfDataToTransfer(0), periphMemoryAddr(0), memory0Addr(0), memory1Addr(0),
        enablePeriphMemIncr(false), enableMemIncr(false), isPeriIncrFixedOffset(false), periphBurst(DMA_BurstType::SingleTransfer), memoryBurst(DMA_BurstType::SingleTransfer),
        periphDataSize(DMA_DataSize::Byte), memoryDataSize(DMA_DataSize::Byte), isPeriphFlowController(false), isNotDirectMode(true), isCircularMode(false),
        isDoubleBuffer(false), startMemoryTarget(DMA_MemoryStart::Memory0), enableTransferCompleteItr(false), enableHalftransferItr(false),
		enableTransferErrorItr(false), enableFIFOErrorItr(false), enableDirectModeErrorItr(false), allowBufferableTransfers(false)
    {}

};

struct DMA_StreamCfg_PerToMem : DMA_StreamCfg_PeripheralAndMemory {
	DMA_StreamCfg_PerToMem() :
		DMA_StreamCfg_PeripheralAndMemory()
	{}
};
struct DMA_StreamCfg_MemToPer : DMA_StreamCfg_PeripheralAndMemory {
	DMA_StreamCfg_MemToPer() :
		DMA_StreamCfg_PeripheralAndMemory()
	{}
};

void dma_configStream(DMA_StreamCfg_MemToMem cfg, DMAStruct* DMA);
void dma_configStream(DMA_StreamCfg_PerToMem& cfg, DMAStruct* DMA);
void dma_configStream(DMA_StreamCfg_MemToPer& cfg, DMAStruct* DMA);


void dma1_enableStream(DMA_Stream stream);
void dma2_enableStream(DMA_Stream stream);
void dma1_disableStream(DMA_Stream stream);
void dma2_disableStream(DMA_Stream stream);

void dma1_setDataTransferSize(DMA_Stream stream, uint16_t items);
void dma2_setDataTransferSize(DMA_Stream stream, uint16_t items);
