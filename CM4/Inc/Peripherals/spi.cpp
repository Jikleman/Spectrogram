#include <spi.h>

#define MASTER_BIT          22

void spi_config(SPI_MasterCFG cfg, SPIStruct* SPIx){
    SPIx->CFG1 = 0x00070000 | (cfg.bitsPerDataFrame<<0);      //Reset Value with the new DSize
    SPIx->CFG1 |= ((uint32_t)cfg.masterPresc<<28) | (cfg.enableTxDMA<<15) | (cfg.enableRxDMA<<14) | ((uint32_t)cfg.fifoThresholdLevel<<5);
    SPIx->CFG2 = 0;       //CFG2 reset value
    SPIx->CFG2 |= (cfg.periphControlsGPIOAltFunc<<31) | (cfg.enableSSOutput<<29) | ((uint32_t)cfg.ssPol<<28) | ((uint32_t)cfg.spiMode<<24)
                | ((uint32_t)cfg.format<<23) | (1<<MASTER_BIT) | ((uint32_t)cfg.protocol<<19) | ((uint32_t)cfg.commMode<<17)
                | (cfg.swapMOSI_MISO<<15) | (cfg.interDataIdleness<<4) | (cfg.SSIdleness<<0);
}

void spi_enableItr(SPIStruct* SPIx, SPI_Event event){
	if (event == SPI_Event::EOT || event == SPI_Event::SUSP || event == SPI_Event::TXC){
		SPIx->IER |= (1<<(uint32_t)SPI_Event::EOT);
		return;
	}

	SPIx->IER |= (1<<(uint32_t)event);
}
void spi_disableItr(SPIStruct* SPIx, SPI_Event event){
	if (event == SPI_Event::EOT || event == SPI_Event::SUSP || event == SPI_Event::TXC){
		SPIx->IER &= ~(1<<(uint32_t)SPI_Event::EOT);
		return;
	}

	SPIx->IER &= ~(1<<(uint32_t)event);
}
void spi_clearFlag(SPIStruct* SPIx, SPI_Event event){
	SPIx->IFCR = (1<<(uint32_t)event);
}
bool spi_readFlag(SPIStruct* SPIx, SPI_Event event){
	return (SPIx->SR & (1<<(uint32_t)event));
}

/* Can lock associated the AF of associated IOs and protects certain configurations (CFG2) from being modified.
 * When the spi is disabled, this lock and protection is removed.
*/
void spi_iolock(SPIStruct* SPIx){
    SPIx->CR1 |= (1<<16);
}

//When SPI is enabled, configurations are write protected. Same for IOLOCK, CRCPOLY, and UDRDR in CR1.
void spi_enable(SPIStruct* SPIx){
    SPIx->CR1 |= (1<<0);
}

void spi_disable(SPIStruct* SPIx){
    SPIx->CR1 &= ~(1<<0);
}

void spi_masterStart(SPIStruct* SPIx){
    SPIx->CR1 |= (1<<9);
}

void spi_masterSuspend(SPIStruct* SPIx){
    SPIx->CR1 |= (1<<10);
}

//SPI needs to be disabled to change the TSize value
void spi_setTSize(SPIStruct* SPIx, uint16_t tsize){
	SPIx->CR2 &= ~0xFFFF;
	SPIx->CR2 |= tsize;
}

void spi_setTSER(SPIStruct* SPIx, uint16_t tser){
	SPIx->CR2 &= ~0xFFFF0000;
	SPIx->CR2 |= ((uint32_t)tser<<16);
}

void spi_transmitData(SPIStruct* SPIx, uint32_t data){
    SPIx->TXDR = data;
}

//If SPI is in half-duplex mode, the SPI is set as a transmitter. The SPI must be disabled to change direction.
void spi_halfDuplex_transmitter(SPIStruct* SPIx){
    SPIx->CR1 |= (1<<11);
}

//If SPI is in half-duplex mode, the SPI is set as a receiver. The SPI must be disabled to change direction.
void spi_halfDuplex_receiver(SPIStruct* SPIx){
    SPIx->CR1 &= ~(1<<11);
}
