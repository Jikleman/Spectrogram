#include "rcc.h"
#include "./utils/bitwise_functions.h"

volatile uint32_t* C2_ENR = &(RCC.C2_AHB3ENR);
volatile uint32_t* C1_ENR = &(RCC.C1_AHB3ENR);
volatile uint32_t* ENR = &(RCC.AHB3ENR);
volatile uint32_t* RSTR = &(RCC.AHB3RSTR);

template<typename T>
void dd::rcc_def::allocate_peripheral_M4(T peripheral){
	set_bit(C2_ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
void dd::rcc_def::allocate_peripheral_M7(T peripheral){
	set_bit(C1_ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
void dd::rcc_def::allocate_peripheral(T peripheral) {
	set_bit(ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
void dd::rcc_def::deallocate_peripheral_M4(T peripheral){
	reset_bit(C2_ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
void dd::rcc_def::deallocate_peripheral_M7(T peripheral){
	reset_bit(C1_ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
void dd::rcc_def::deallocate_peripheral(T peripheral){
	reset_bit(ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
void dd::rcc_def::reset_peripheral(T peripheral){
	volatile int x = 0;
	set_bit(RSTR[peripheral_bus<T>::index], (uint32_t)peripheral);
	x++;
	reset_bit(RSTR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

template<typename T>
bool dd::rcc_def::is_peripheral_allocated(T peripheral){
	return read_bit(ENR[peripheral_bus<T>::index], (uint32_t)peripheral);
}

#define EXPLICIT_INSTANTIATE(T) \
	template void dd::rcc_def::allocate_peripheral_M4<T>(T); \
	template void dd::rcc_def::allocate_peripheral_M7<T>(T); \
	template void dd::rcc_def::allocate_peripheral<T>(T); \
	template void dd::rcc_def::deallocate_peripheral_M4<T>(T); \
	template void dd::rcc_def::deallocate_peripheral_M7<T>(T); \
	template void dd::rcc_def::deallocate_peripheral<T>(T); \
	template void dd::rcc_def::reset_peripheral<T>(T); \
	template bool dd::rcc_def::is_peripheral_allocated<T>(T);

EXPLICIT_INSTANTIATE(AHB3);
EXPLICIT_INSTANTIATE(AHB1);
EXPLICIT_INSTANTIATE(AHB2);
EXPLICIT_INSTANTIATE(AHB4);
EXPLICIT_INSTANTIATE(APB3);
EXPLICIT_INSTANTIATE(APB1L);
EXPLICIT_INSTANTIATE(APB1H);
EXPLICIT_INSTANTIATE(APB2);
EXPLICIT_INSTANTIATE(APB4);

#undef EXPLICIT_INSTANTIATE
