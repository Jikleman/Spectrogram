#pragma once

#include <stdint.h>

enum class HSEM_CoreID{
	invalid = 4,
	MASTER0 = 3,			//CPU1 (M7)
	MASTER1 = 1				//CPU2 (M4)
};

#define HSEM_Base			(0x58026400)

struct HSEMStruct{
	volatile uint32_t Rx[32];
	volatile uint32_t RLRx[32];
	volatile uint32_t C1IER;
	volatile uint32_t C1ICR;
	volatile uint32_t C1ISR;
	volatile uint32_t C1MISR;
	volatile uint32_t C2IER;
	volatile uint32_t C2ICR;
	volatile uint32_t C2ISR;
	volatile uint32_t C2MISR;
	volatile uint32_t Reserved[8];		//Not labeled as reserved in manual, but there is a gap here
	volatile uint32_t CR;
	volatile uint32_t KEYR;
};

#define HSEM				((volatile HSEMStruct *) HSEM_Base)

bool HSEM_writeLock(HSEM_CoreID coreId, uint8_t procId, uint8_t semaphore);
bool HSEM_readLock(HSEM_CoreID coreId, uint8_t semaphore);
void HSEM_unlock(HSEM_CoreID coreId, uint8_t procId, uint8_t semaphore);

void HSEM_unlockAllSemaphores(HSEM_CoreID coreId, uint16_t key);
void HSEM_setKey(uint16_t key);

void HSEM_enableC1Itr(uint8_t semaphore);
void HSEM_enableC2Itr(uint8_t semaphore);
void HSEM_disableC1Itr(uint8_t semaphore);
void HSEM_disableC2Itr(uint8_t semaphore);
bool HSEM_isC1ItrEnabled(uint8_t semaphore);
bool HSEM_isC2ItrEnabled(uint8_t semaphore);
bool HSEM_readC1Flag(uint8_t semaphore);
bool HSEM_readC2Flag(uint8_t semaphore);
bool HSEM_readC1MaskedFlag(uint8_t semaphore);
bool HSEM_readC2MaskedFlag(uint8_t semaphore);
void HSEM_clearC1Flag(uint8_t semaphore);
void HSEM_clearC2Flag(uint8_t semaphore);

HSEM_CoreID HSEM_getCoreID();
