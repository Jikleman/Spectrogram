#include "rcc.h"

enum BUS {AHB3=0, AHB1=1, AHB2=2, AHB4=3, APB3=4, APB1L=5, APB1H=6, APB2=7, APB4=8};

//Allocates a peripheral from a bus to a particular CPU
void allocatePeripheral(BUS bus, int peripheral, int CPU){
	volatile uint32_t* ENR = &(RCC->AHB3ENR);
    switch (CPU){
		case 2:
			ENR = &(RCC->C2_AHB3ENR);
			break;
		case 1:
			ENR = &(RCC->C1_AHB3ENR);
			break;
		default:
			ENR = &(RCC->AHB3ENR);
    }
    ENR[bus] |= (1 << peripheral);
}

void allocatePeripheral(AHB3_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::AHB3, (int)peripheral, (int)CPU);
}
void allocatePeripheral(AHB1_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::AHB1, (int)peripheral, (int)CPU);
}
void allocatePeripheral(AHB2_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::AHB2, (int)peripheral, (int)CPU);
}
void allocatePeripheral(AHB4_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::AHB4, (int)peripheral, (int)CPU);
}
void allocatePeripheral(APB3_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::APB3, (int)peripheral, (int)CPU);
}
void allocatePeripheral(APB1L_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::APB1L, (int)peripheral, (int)CPU);
}
void allocatePeripheral(APB1H_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::APB1H, (int)peripheral, (int)CPU);
}
void allocatePeripheral(APB2_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::APB2, (int)peripheral, (int)CPU);
}
void allocatePeripheral(APB4_Peripheral peripheral, SysCPU CPU){
	allocatePeripheral(BUS::APB4, (int)peripheral, (int)CPU);
}

//Resets the related peripheral for a particular bus
void resetPeripheral(BUS bus, int peripheral){
    volatile uint32_t* RSTR = &(RCC->AHB3RSTR);
    RSTR[bus] |= (1 << peripheral);
    RSTR[bus] &= ~(1 << peripheral);
}

void resetPeripheral(AHB3_Peripheral peripheral){
	resetPeripheral(AHB3, (int)peripheral);
}
void resetPeripheral(AHB1_Peripheral peripheral){
	resetPeripheral(AHB1, (int)peripheral);
}
void resetPeripheral(AHB2_Peripheral peripheral){
	resetPeripheral(AHB2, (int)peripheral);
}
void resetPeripheral(AHB4_Peripheral peripheral){
	resetPeripheral(AHB4, (int)peripheral);
}
void resetPeripheral(APB3_Peripheral peripheral){
	resetPeripheral(APB3, (int)peripheral);
}
void resetPeripheral(APB1L_Peripheral peripheral){
	resetPeripheral(APB1L, (int)peripheral);
}
void resetPeripheral(APB1H_Peripheral peripheral){
	resetPeripheral(APB1H, (int)peripheral);
}
void resetPeripheral(APB2_Peripheral peripheral){
	resetPeripheral(APB2, (int)peripheral);
}
void resetPeripheral(APB4_Peripheral peripheral){
	resetPeripheral(APB4, (int)peripheral);
}

//Both resets and allocates a peripheral from a bus to a particular CPU
void freshAllocPeripheral(BUS bus, int peripheral, int CPU){
	resetPeripheral(bus, peripheral);
	allocatePeripheral(bus, peripheral, CPU);
}

void freshAllocPeripheral(AHB3_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(AHB3, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(AHB1_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(AHB1, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(AHB2_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(AHB2, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(AHB4_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(AHB4, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(APB3_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(APB3, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(APB1L_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(APB1L, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(APB1H_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(APB1H, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(APB2_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(APB2, (int)peripheral, (int)CPU);
}
void freshAllocPeripheral(APB4_Peripheral peripheral, SysCPU CPU){
	freshAllocPeripheral(APB4, (int)peripheral, (int)CPU);
}

bool isAllocated(BUS bus, int peripheral){
	volatile uint32_t* ENR = &(RCC->AHB3ENR);
	return (ENR[bus] & (1<<peripheral));
}

void reallocPeripheral(BUS bus, int peripheral, int CPU){
	if (!isAllocated(bus, peripheral))
		resetPeripheral(bus, peripheral);
	allocatePeripheral(bus, peripheral, CPU);
}

void reallocPeripheral(AHB3_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(AHB3, (int)peripheral, (int)CPU);
}
void reallocPeripheral(AHB1_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(AHB1, (int)peripheral, (int)CPU);
}
void reallocPeripheral(AHB2_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(AHB2, (int)peripheral, (int)CPU);
}
void reallocPeripheral(AHB4_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(AHB4, (int)peripheral, (int)CPU);
}
void reallocPeripheral(APB3_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(APB3, (int)peripheral, (int)CPU);
}
void reallocPeripheral(APB1L_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(APB1L, (int)peripheral, (int)CPU);
}
void reallocPeripheral(APB1H_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(APB1H, (int)peripheral, (int)CPU);
}
void reallocPeripheral(APB2_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(APB2, (int)peripheral, (int)CPU);
}
void reallocPeripheral(APB4_Peripheral peripheral, SysCPU CPU){
	reallocPeripheral(APB4, (int)peripheral, (int)CPU);
}
