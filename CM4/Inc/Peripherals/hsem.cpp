#include <hsem.h>

#define KEY_BIT					16
#define COREID_BIT				8
#define LOCK_BIT				31
#define SEMAPHORE_MASK			0x1F		//Valid semaphore counts are from 0 to 31. Higher values wrap-around.

//Returns true if the semaphore was locked, and false if it failed because the semaphore was already locked. Locks a semaphore using the two-step write lock procedure.
bool HSEM_writeLock(HSEM_CoreID coreId, uint8_t procId, uint8_t semaphore){
	constexpr uint32_t setLock = (1<<LOCK_BIT);
	uint32_t sIdx = semaphore & SEMAPHORE_MASK;
	uint32_t mask = setLock | ((uint32_t)coreId<<COREID_BIT) | (uint32_t)procId;

	HSEM->Rx[sIdx] = mask;
	return (HSEM->Rx[sIdx] == mask);
}

//Returns true if the semaphore was locked, and false if it failed because the semaphore was already locked. Locks a semaphore using the one-step read lock procedure
//Semaphores locked by this method are unlocked using a procID of 0
bool HSEM_readLock(HSEM_CoreID coreId, uint8_t semaphore){
	constexpr uint32_t lock = (1<<LOCK_BIT);
	uint32_t sIdx = semaphore & SEMAPHORE_MASK;
	uint32_t mask = lock | ((uint32_t)coreId<<COREID_BIT);

	return (HSEM->RLRx[sIdx] == mask);
}

void HSEM_unlock(HSEM_CoreID coreId, uint8_t procId, uint8_t semaphore){
	uint32_t sIdx = semaphore & SEMAPHORE_MASK;
	uint32_t mask = (uint32_t)coreId<<COREID_BIT | (uint32_t)procId;
	HSEM->Rx[sIdx] = mask;
}

void HSEM_enableC1Itr(uint8_t semaphore){
    HSEM->C1IER |= (1<<(semaphore & SEMAPHORE_MASK));
}

void HSEM_enableC2Itr(uint8_t semaphore){
    HSEM->C2IER |= (1<<(semaphore & SEMAPHORE_MASK));
}

void HSEM_disableC1Itr(uint8_t semaphore){
    HSEM->C1IER &= ~(1<<(semaphore & SEMAPHORE_MASK));
}

void HSEM_disableC2Itr(uint8_t semaphore){
    HSEM->C2IER &= ~(1<<(semaphore & SEMAPHORE_MASK));
}

bool HSEM_isC1ItrEnabled(uint8_t semaphore){
	return HSEM->C1IER & (1<<semaphore);
}
bool HSEM_isC2ItrEnabled(uint8_t semaphore){
	return HSEM->C2IER & (1<<semaphore);
}

bool HSEM_readC1Flag(uint8_t semaphore){
	return HSEM->C1ISR & (1<<(semaphore & SEMAPHORE_MASK));
}

bool HSEM_readC2Flag(uint8_t semaphore){
	return HSEM->C2ISR & (1<<(semaphore & SEMAPHORE_MASK));
}

bool HSEM_readC1MaskedFlag(uint8_t semaphore){
	return HSEM->C1MISR & (1<<(semaphore & SEMAPHORE_MASK));
}

bool HSEM_readC2MaskedFlag(uint8_t semaphore){
	return HSEM->C2MISR & (1<<(semaphore & SEMAPHORE_MASK));
}

void HSEM_clearC1Flag(uint8_t semaphore){
    HSEM->C1ICR = 1<<(semaphore & SEMAPHORE_MASK);
}

void HSEM_clearC2Flag(uint8_t semaphore){
    HSEM->C2ICR = 1<<(semaphore & SEMAPHORE_MASK);
}


void HSEM_unlockAllSemaphores(HSEM_CoreID coreId, uint16_t key){
	HSEM->CR = (key<<KEY_BIT) | ((uint32_t)coreId<<COREID_BIT);
}

void HSEM_setKey(uint16_t key){
	HSEM->KEYR = (key<<KEY_BIT);
}
