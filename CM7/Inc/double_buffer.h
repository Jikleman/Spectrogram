#pragma once

#include <stdint.h>
#define shared __attribute__ ((section(".shared_memory")))

#include <hsem.h>

//Please put explicit templates in dBuffMan.cpp

//Double_buffer class intended for sharing between two cores
//Is not guaranteed to be thread-safe on the same core because of read lock proc id on hsem
template <typename T, uint32_t size>
class double_buffer {
private:
	//Array related variables
	volatile uint32_t tx_buff_idx;	//tx_buff is 0 upon init
	volatile uint32_t rx_buff_idx;	//rx_buff is 1 upon init
	volatile T shbuffers[2][size];	//0 refers to buff[0], and 1 refers to buff[1] obviously

	//Array helper functions
	bool len_in_bounds(uint32_t len) const;
	void copy_to_tx(T (&buff)[], uint32_t buff_start, uint32_t len);
	void copy_from_rx(T (&buff)[], uint32_t buff_start, uint32_t len);

	//Synchronization related variables
	volatile uint32_t tx_hsem_id;
	volatile uint32_t rx_hsem_id;
	volatile bool tx_ready_for_swap;
	volatile bool rx_ready_for_swap;

	//Synchronization helper functions
	void try_swap();
	void swap_buffer_idx();

	//HSEM related functions
	bool tx_try_lock();
	bool rx_try_lock();
	void tx_unlock();
	void rx_unlock();
public:
	double_buffer(uint32_t tx_hsem_id, uint32_t rx_hsem_id) :
		tx_buff_idx(0), rx_buff_idx(1),
		tx_hsem_id(tx_hsem_id), rx_hsem_id(rx_hsem_id),
		tx_ready_for_swap(false), rx_ready_for_swap(false)
	{}

	bool tx_try_write(T (&buff)[], uint32_t buff_start, uint32_t len);
	bool tx_write(T (&buff)[], uint32_t buff_start, uint32_t len);
	bool rx_try_read(T (&buff)[], uint32_t buff_start, uint32_t len);
	bool rx_read(T (&buff)[], uint32_t buff_start, uint32_t len);

	void tx_swap_ready();
	void rx_swap_ready();
};


//Returns true if 0 < len < size
template<typename T, uint32_t size>
bool double_buffer<T,size>::len_in_bounds(uint32_t len) const{
	return (0 < len || len < size);
}


template<typename T, uint32_t size>
void double_buffer<T,size>::copy_to_tx(T (&buff)[], uint32_t buff_start, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		shbuffers[tx_buff_idx][i] = buff[i + buff_start];
	}
}

template<typename T, uint32_t size>
void double_buffer<T,size>::copy_from_rx(T (&buff)[], uint32_t buff_start, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		buff[i + buff_start] = shbuffers[rx_buff_idx][i];
	}
}

template <typename T, uint32_t size>
void double_buffer<T,size>::swap_buffer_idx(){
	if (tx_buff_idx == 0){
		tx_buff_idx = 1;
		rx_buff_idx = 0;
	} else {
		tx_buff_idx = 0;
		rx_buff_idx = 1;
	}
}

//Returns true if able to lock tx hsem.
//Also returns true if core already locked the hsem.
template<typename T, uint32_t size>
bool double_buffer<T,size>::tx_try_lock(){
	const HSEM_CoreID core_id = HSEM_getCoreID();
	if (!HSEM_readLock(core_id, tx_hsem_id))
		return false;
	return true;
}

//Returns true if able to lock rx hsem.
//Also returns true if core already locked the hsem.
template<typename T, uint32_t size>
bool double_buffer<T,size>::rx_try_lock(){
	const HSEM_CoreID core_id = HSEM_getCoreID();
	if (!HSEM_readLock(core_id, rx_hsem_id))
		return false;
	return true;
}

//Unlocks the tx hsem
template<typename T, uint32_t size>
void double_buffer<T,size>::tx_unlock(){
	const HSEM_CoreID core_id = HSEM_getCoreID();
	constexpr uint32_t readlock_proc_id = 0;
	HSEM_unlock(core_id, readlock_proc_id, tx_hsem_id);
}

//Unlocks the rx hsem
template<typename T, uint32_t size>
void double_buffer<T,size>::rx_unlock(){
	const HSEM_CoreID core_id = HSEM_getCoreID();
	constexpr uint32_t readlock_proc_id = 0;
	HSEM_unlock(core_id, readlock_proc_id, rx_hsem_id);
}

//If tx_hsem is locked, no write is done and false is returned. True if successfully completed.
template<typename T, uint32_t size>
bool double_buffer<T,size>::tx_try_write(T (&buff)[], uint32_t buff_start, uint32_t len){
	if (!len_in_bounds(len)){
		return false;
	}
	if (tx_ready_for_swap){
		return false;
	}
	if (!tx_try_lock()){
		tx_unlock();
		return false;
	}

	copy_to_tx(buff, buff_start, len);

	tx_unlock();
	return true;
}

//Returns false if len > size. True if successfully completed.
//Waits for tx hsem lock and writes before returning true.
template<typename T, uint32_t size>
bool double_buffer<T,size>::tx_write(T (&buff)[], uint32_t buff_start, uint32_t len){
	if (!len_in_bounds(len)){
		return false;
	}

	while(tx_ready_for_swap) {}
	while(!tx_try_lock()) {}

	copy_to_tx(buff, buff_start, len);

	tx_unlock();
	return true;
}

//If rx_hsem is locked, no write is done and false is returned. True if successfully completed
template<typename T, uint32_t size>
bool double_buffer<T,size>::rx_try_read(T (&buff)[], uint32_t buff_start, uint32_t len){
	if (!len_in_bounds(len)){
		return false;
	}
	if (rx_ready_for_swap){
		return false;
	}
	if (!rx_try_lock()){
		rx_unlock();
		return false;
	}

	copy_from_rx(buff, buff_start, len);

	rx_unlock();
	return true;
}

//Returns false if len > size. True if successfully completed.
template<typename T, uint32_t size>
bool double_buffer<T,size>::rx_read(T (&buff)[], uint32_t buff_start, uint32_t len){
	if (!len_in_bounds(len)){
		return false;
	}

	while(rx_ready_for_swap) {}
	while(!rx_try_lock()) {}

	copy_from_rx(buff, buff_start, len);

	rx_unlock();
	return true;
}

//Attempts to lock tx and rx hsem before swapping.
//If either is locked, releases locks and does nothing
template <typename T, uint32_t size>
void double_buffer<T,size>::try_swap(){
	if(!tx_try_lock()){
		return;
	}
	if(!rx_try_lock()){
		tx_unlock();
		return;
	}
	if (!tx_ready_for_swap || !rx_ready_for_swap){
		tx_unlock();
		rx_unlock();
		return;
	}

	swap_buffer_idx();

	tx_ready_for_swap = false;
	rx_ready_for_swap = false;
	tx_unlock();
	rx_unlock();
}

//Signals that the tx buffer is ready to swap. Attempts exchange if rx is also ready.
//Blocking call, will wait until tx hsem is available
template<typename T, uint32_t size>
void double_buffer<T,size>::tx_swap_ready(){
	//Ideally, neither lock should be locked for long by either swap_ready()
	while(!tx_try_lock()){}
	tx_ready_for_swap = true;
	tx_unlock();

	try_swap();
}

//Signals that the rx buffer is ready to swap. Attempts exchange if tx is also ready.
//Blocking call, will wait until rx hsem is available.
template<typename T, uint32_t size>
void double_buffer<T,size>::rx_swap_ready(){
	//Ideally, neither lock should be locked for long by either swap_ready()
	while(!rx_try_lock()) {}
	rx_ready_for_swap = true;
	rx_unlock();

	try_swap();
}
