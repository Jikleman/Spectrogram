#include "dma.h"

constexpr uint8_t flagStartbit [4] {0, 6, 16, 22};

//Returns the status of a particular interrupt status flag for a DMA1 stream.
bool dma1_readItrFlag(DMA_Stream stream, DMA_ItrStatus flag){
    volatile uint32_t& ISR = ((int)stream / 4) ? DMA1->HISR : DMA1->LISR;
    return ISR & (1<<(flagStartbit[(int)stream % 4] + (int)flag));
}

//Returns the status of a particular interrupt status flag for a DMA2 stream.
bool dma2_readItrFlag(DMA_Stream stream, DMA_ItrStatus flag){
    volatile uint32_t& ISR = ((int)stream / 4) ? DMA2->HISR : DMA2->LISR;
    return ISR & (1<<(flagStartbit[(int)stream % 4] + (int)flag));
}

//Clears the status of a particular interrupt status flag for a DMA1 stream.
void dma1_clearItrFlag(DMA_Stream stream, DMA_ItrStatus flag){
    volatile uint32_t& IFCR = ((int)stream / 4) ? DMA1->HIFCR : DMA1->LIFCR;
    IFCR = (1<<(flagStartbit[(int)stream % 4] + (int)flag));
}

//Clears the status of a particular interrupt status flag for a DMA2 stream.
void dma2_clearItrFlag(DMA_Stream stream, DMA_ItrStatus flag){
    volatile uint32_t& IFCR = ((int)stream / 4) ? DMA1->HIFCR : DMA1->LIFCR;
    IFCR = (1<<(flagStartbit[(int)stream % 4] + (int)flag));
}

void dma1_clearAllFlags(DMA_Stream stream){
	volatile uint32_t& IFCR = ((int)stream / 4) ? DMA1->HIFCR : DMA1->LIFCR;
	IFCR = (0b111101<<(flagStartbit[(int)stream % 4]));
}
void dma2_clearAllFlags(DMA_Stream stream){
	volatile uint32_t& IFCR = ((int)stream / 4) ? DMA1->HIFCR : DMA1->LIFCR;
	IFCR = (1<<(flagStartbit[(int)stream % 4]));
}

struct DMAStreamStruct{
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
};

enum DMA_TransferDirection{
    PeriphToMem = 0,
    MemToPeriph = 1,
    MemToMem = 2
};

//Configures a DMA stream for memory to memory. Direct mode is disabled by hardware.
void dma_configStream(DMA_StreamCfg_MemToMem cfg, DMAStruct* DMA){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA->S0CR))[(int)cfg.stream];
    S.FCR &= ~(0x87); 	//clear FEIE, DMDIS, FTH[1:0]
    S.FCR |= ((uint32_t)cfg.fifoThreshold) | ((uint32_t)cfg.enableFIFOErrorItr<<7);
    S.M0AR = cfg.destinMemoryAddr;
    S.PAR = cfg.sourceMemoryAddr;
    S.NDTR = cfg.numberOfDataToTransfer;
    S.CR = 0;			//SxCR reset value is 0x0000 0000
    S.CR |= ((uint32_t)cfg.destinBurst<<23) | ((uint32_t)cfg.sourceBurst<<21) | (cfg.allowBufferableTransfers<<20) | ((uint32_t)cfg.priority<<16) | ((uint32_t)cfg.isSourceIncrFixedOffset<<15) | ((uint32_t)cfg.destinDataSize<<13) | ((uint32_t)cfg.sourceDataSize<<11)
            | (cfg.enableDestinMemIncr<<10) | (cfg.enableSourceMemIncr<<9) | (DMA_TransferDirection::MemToMem<<6) | (cfg.enableTransferCompleteItr<<4) | (cfg.enableHalftransferItr<<3) | (cfg.enableTransferErrorItr<<2);
}

//Helper function that actually writes the configuration for any transfer between peripheral and memory.
void dma_configstream(DMA_StreamCfg_PeripheralAndMemory& cfg, DMA_TransferDirection direction, DMAStruct* DMA){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA->S0CR))[(int)cfg.stream];
    S.FCR &= ~(0b10000111); 	//clear FEIE, DMDIS, FTH[1:0]
    S.FCR |= ((uint32_t)cfg.fifoThreshold) | (cfg.isNotDirectMode<<2)  | ((uint32_t)cfg.enableFIFOErrorItr<<7);
    S.M0AR = cfg.memory0Addr;
    S.M1AR = cfg.memory1Addr;
    S.PAR = cfg.periphMemoryAddr;
    S.NDTR = cfg.numberOfDataToTransfer;
    S.CR = 0;			//SxCR reset value is 0x0000 0000
    S.CR |= ((uint32_t)cfg.memoryBurst<<23) | ((uint32_t)cfg.periphBurst<<21) | (cfg.allowBufferableTransfers<<20) | ((uint32_t)cfg.startMemoryTarget<<19) | (cfg.isDoubleBuffer<<18) | ((uint32_t)cfg.priority<<16) | ((uint32_t)cfg.isPeriIncrFixedOffset<<15)
    		| ((uint32_t)cfg.memoryDataSize<<13) | ((uint32_t)cfg.periphDataSize<<11) | (cfg.enableMemIncr<<10) | (cfg.enablePeriphMemIncr<<9) | (cfg.isCircularMode<<8) | ((uint32_t)direction<<6) | (cfg.isPeriphFlowController<<5)
			| (cfg.enableTransferCompleteItr<<4) | (cfg.enableHalftransferItr<<3) | (cfg.enableTransferErrorItr<<2) | (cfg.enableDirectModeErrorItr<<1);
}

//Configures a DMA stream for peripheral to memory.
void dma_configStream(DMA_StreamCfg_PerToMem& cfg, DMAStruct* DMA){
    dma_configstream(cfg, DMA_TransferDirection::PeriphToMem, DMA);
}

//Configures a DMA stream for memory to peripheral.
void dma_configStream(DMA_StreamCfg_MemToPer& cfg, DMAStruct* DMA){
	dma_configstream(cfg, DMA_TransferDirection::MemToPeriph, DMA);
}

//Enables a DMA1 Stream.
void dma1_enableStream(DMA_Stream stream){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA1->S0CR))[(int)stream];
    S.CR |= (1<<0);
}
//Enables a DMA2 Stream.
void dma2_enableStream(DMA_Stream stream){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA2->S0CR))[(int)stream];
    S.CR |= (1<<0);
}
//Disables a DMA1 Stream and waits until the stream is disabled.
void dma1_disableStream(DMA_Stream stream){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA1->S0CR))[(int)stream];
    S.CR &= ~(1<<0);
    while(S.CR & 1);
}
//Disables a DMA2 Stream and waits until the stream is disabled.
void dma2_disableStream(DMA_Stream stream){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA2->S0CR))[(int)stream];
    S.CR &= ~(1<<0);
    while(S.CR & 1);
}

void dma1_setDataTransferSize(DMA_Stream stream, uint16_t items){
    volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA1->S0CR))[(int)stream];
    S.NDTR = items;
}
void dma2_setDataTransferSize(DMA_Stream stream, uint16_t items){
	volatile DMAStreamStruct& S = ((volatile DMAStreamStruct *) (&DMA2->S0CR))[(int)stream];
	S.NDTR = items;
}
