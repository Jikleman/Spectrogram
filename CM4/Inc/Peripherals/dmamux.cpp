#include "dmamux.h"

//Helper function that returns the appropriate CxCR registers for a channel in DMAMUX1.
volatile uint32_t& dmamux1_chConfigReg(int channel){
	volatile uint32_t* C0CR = &DMAMUX1.C0CR;
	return C0CR[channel];
}

//Helper function that returns the appropriate CxCR registers for a channel in DMAMUX2.
volatile uint32_t& dmamux2_chConfigReg(int channel){
	volatile uint32_t* C0CR = &DMAMUX2.C0CR;
	return C0CR[channel];
}

//Helper functin that returns the appropriate RGxCR registers for a request generator chanenl in DMAMUX1.
volatile uint32_t& dmamux1_rgChConfigReg(int channel){
	volatile uint32_t* RG0CR = &DMAMUX1.RG0CR;
	return RG0CR[channel];
}

//Helper functin that returns the appropriate RGxCR registers for a request generator chanenl in DMAMUX2.
volatile uint32_t& dmamux2_rgChConfigReg(int channel){
	volatile uint32_t* RG0CR = &DMAMUX2.RG0CR;
	return RG0CR[channel];	
}

//Configures a particular DMAMUX1 channel
void dmamux_configChannel(DMAMUX1_ChannelCfg cfg){
	volatile uint32_t& CxCR = dmamux1_chConfigReg((int)cfg.channel);
	CxCR = 0; 	//Reset value of CxCR is 0x0000 0000
	CxCR |= ((uint32_t)cfg.syncInput<<24) | (cfg.numberForwardDMARequests<<19) | ((uint32_t)cfg.syncPolarity<<17) | (cfg.enableSynchronization<<16) | (cfg.enableEventGeneration<<9) | (cfg.enableSyncOverrunItr<<8) | ((uint32_t)cfg.inputRequest<<0);
}

//Configures a particular DMAMUX2 channel
void dmamux_configChannel(DMAMUX2_ChannelCfg cfg){
	volatile uint32_t& CxCR = dmamux2_chConfigReg((int)cfg.channel);
	CxCR = 0; 	//Reset value of CxCR is 0x0000 0000
	CxCR |= ((uint32_t)cfg.syncInput<<24) | (cfg.numberForwardDMARequests<<19) | ((uint32_t)cfg.syncPolarity<<17) | (cfg.enableSynchronization<<16) | (cfg.enableEventGeneration<<9) | (cfg.enableSyncOverrunItr<<8) | ((uint32_t)cfg.inputRequest<<0);
}

//Reads the status of the Synchronization overrun event flag for DMAMUX1 channels.
bool dmamux_readSyncOverrunEventflag(DMAMUX1_Channel channel){
	return DMAMUX1.CSR & (1<<(uint32_t)channel);
}

//Reads the status of the Synchronization overrun event flag for DMAMUX2 channels.
bool dmamux_readSyncOverrunEventflag(DMAMUX2_Channel channel){
	return DMAMUX2.CSR & (1<<(uint32_t)channel);
}

//Clears the status of the Synchronization overrun event flag for DMAMUX2 channels.
void dmamux_clearSyncOverrunEventFlag(DMAMUX1_Channel channel){
	DMAMUX1.CFR = (1<<(uint32_t)channel);
}

//Clears the status of the Synchronization overrun event flag for DMAMUX2 channels.
void dmamux_clearSyncOverrunEventFlag(DMAMUX2_Channel channel){
	DMAMUX1.CFR = (1<<(uint32_t)channel);
}

//Configures a particular DMAMUX1 request generator channel
void dmamux_configReqGenChannel(DMAMUX1_ReqGenCfg cfg){
	volatile uint32_t& RGxCR = dmamux1_rgChConfigReg((int)cfg.channel);
	RGxCR = 0;	//Reset value of RGxCR is 0x0000 0000
	RGxCR |= ((uint32_t)cfg.numberGenDMARequests<<19) | ((uint32_t)cfg.trigPolarity<<17) | (cfg.enableReqGenChannel<<16) | (cfg.enableTrigOverrunItr<<8) | ((uint32_t)cfg.trigInput<<0);
}
//Configures a particular DMAMUX2 request generator channel
void dmamux_configReqGenChannel(DMAMUX2_ReqGenCfg cfg){
	volatile uint32_t& RGxCR = dmamux2_rgChConfigReg((int)cfg.channel);
	RGxCR = 0;	//Reset value of RGxCR is 0x0000 0000
	RGxCR |= ((uint32_t)cfg.numberGenDMARequests<<19) | ((uint32_t)cfg.trigPolarity<<17) | (cfg.enableReqGenChannel<<16) | (cfg.enableTrigOverrunItr<<8) | ((uint32_t)cfg.trigInput<<0);
}

//Reads the status of the Trigger overrun event flag for DMAMUX1 channels.
bool dmamux_readSTrigOverrunEventflag(DMAMUX1_ReqGenChannel channel){
	return DMAMUX1.RGSR & (1<<(uint32_t)channel);
}

//Reads the status of the Trigger overrun event flag for DMAMUX1 channels.
bool dmamux_readSTrigOverrunEventflag(DMAMUX2_ReqGenChannel channel){
	return DMAMUX2.RGSR & (1<<(uint32_t)channel);
}

//Clears the status of the Trigger overrun event flag for DMAMUX2 channels.
void dmamux_clearTrigOverrunEventFlag(DMAMUX1_ReqGenChannel channel){
	DMAMUX1.RGCFR = (1<<(uint32_t)channel);
}

//Clears the status of the Trigger overrun event flag for DMAMUX2channels.
void dmamux_clearTrigOverrunEventFlag(DMAMUX2_ReqGenChannel channel){
	DMAMUX2.RGCFR = (1<<(uint32_t)channel);
}
