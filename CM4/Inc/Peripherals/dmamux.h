#pragma once

#include "dmamux_inputs.h"
#include <stdint.h>

#define DMAMUX1_BASE			(0x40020800)
#define DMAMUX2_BASE            (0x58025800)

struct DMAMUXStruct {
    volatile uint32_t C0CR;
    volatile uint32_t C1CR;
    volatile uint32_t C2CR;
    volatile uint32_t C3CR;
    volatile uint32_t C4CR;
    volatile uint32_t C5CR;
    volatile uint32_t C6CR;
    volatile uint32_t C7CR;
    volatile uint32_t C8CR;             // Not defined for DMAMUX2
    volatile uint32_t C9CR;             // Not defined for DMAMUX2
    volatile uint32_t C10CR;            // Not defined for DMAMUX2
    volatile uint32_t C11CR;            // Not defined for DMAMUX2
    volatile uint32_t C12CR;            // Not defined for DMAMUX2
    volatile uint32_t C13CR;            // Not defined for DMAMUX2
    volatile uint32_t C14CR;            // Not defined for DMAMUX2
    volatile uint32_t C15CR;            // Not defined for DMAMUX2
    volatile uint32_t Reserved0 [16];
    volatile uint32_t CSR;
    volatile uint32_t CFR;
    volatile uint32_t Reserved1 [30];
    volatile uint32_t RG0CR;
    volatile uint32_t RG1CR;
    volatile uint32_t RG2CR;
    volatile uint32_t RG3CR;
    volatile uint32_t RG4CR;            // Not explicity defined in the DMAMUX register map, but the request generator register information says there are 8 of them.
    volatile uint32_t RG5CR;            //
    volatile uint32_t RG6CR;            //
    volatile uint32_t RG7CR;            //
    volatile uint32_t Reserved2[8];     //
    volatile uint32_t RGSR;
    volatile uint32_t RGCFR;
};

#define DMAMUX1					(*(volatile DMAMUXStruct *) DMAMUX1_BASE)
#define DMAMUX2            		(*(volatile DMAMUXStruct *) DMAMUX2_BASE)

enum class DMAMUX_Polarity{
	NoEvent = 0,
	RisingEdge = 1,
	FallingEdge = 2,
	RisingAndFallingEdge = 3
};

enum class DMAMUX1_Channel{
	//DMA1 Channels
	Ch0 = 0,
	Ch1 = 1,
	Ch2 = 2,
	Ch3 = 3,
	Ch4 = 4,
	Ch5 = 5,
	Ch6 = 6,
	Ch7 = 7,
	//DMA2 Channels
	Ch8 = 8,
	Ch9 = 9,
	Ch10 = 10,
	Ch11 = 11,
	Ch12 = 12,
	Ch13 = 13,
	Ch14 = 14,
	Ch15 = 15
};

enum class DMAMUX2_Channel{
	//BDMA Channels
	Ch0 = 0,
	Ch1 = 0,
	Ch2 = 1,
	Ch3 = 2,
	Ch4 = 3,
	Ch5 = 4,
	Ch6 = 5,
	Ch7 = 6
};

struct DMAMUX1_ChannelCfg {
	DMAMUX1_Channel channel;
    DMAMUX1_MultiplexerInput inputRequest;
    DMAMUX1_SynchInput syncInput;
    DMAMUX_Polarity syncPolarity;
    uint8_t enableSynchronization : 1;
    uint8_t enableEventGeneration : 1;
    uint8_t enableSyncOverrunItr : 1;
    uint8_t numberForwardDMARequests : 5; //The number requests forwarded is +1, so a value of 0 forwards 1 request.

    //Initializes struct to CxCR default values
    DMAMUX1_ChannelCfg():
    	channel(DMAMUX1_Channel::Ch0), inputRequest(DMAMUX1_MultiplexerInput::null_input), syncInput(DMAMUX1_SynchInput::dmamux1_evt0), syncPolarity(DMAMUX_Polarity::NoEvent),
		enableSynchronization(false), enableEventGeneration(false), enableSyncOverrunItr(false), numberForwardDMARequests(0)
    {}
};

struct DMAMUX2_ChannelCfg {
	DMAMUX2_Channel channel;
    DMAMUX2_MultiplexerInput inputRequest;
    DMAMUX2_SynchInput syncInput;
    DMAMUX_Polarity syncPolarity;
    uint8_t enableSynchronization : 1;
    uint8_t enableEventGeneration : 1;
    uint8_t enableSyncOverrunItr : 1;
    uint8_t numberForwardDMARequests : 5; //The number requests forwarded is +1, so a value of 0 forwards 1 request.

    //Initializes struct to CxCR default values
    DMAMUX2_ChannelCfg():
    	channel(DMAMUX2_Channel::Ch0), inputRequest(DMAMUX2_MultiplexerInput::null_input), syncInput(DMAMUX2_SynchInput::dmamux2_evt0), syncPolarity(DMAMUX_Polarity::NoEvent),
		enableSynchronization(false), enableEventGeneration(false), enableSyncOverrunItr(false), numberForwardDMARequests(0)
    {}
};

void dmamux_configChannel(DMAMUX1_ChannelCfg cfg);
void dmamux_configChannel(DMAMUX2_ChannelCfg cfg);
bool dmamux_readSyncOverrunEventflag(DMAMUX1_Channel channel);
bool dmamux_readSyncOverrunEventflag(DMAMUX2_Channel channel);
void dmamux_clearSyncOverrunEventFlag(DMAMUX1_Channel channel);
void dmamux_clearSyncOverrunEventFlag(DMAMUX2_Channel channel);

enum class DMAMUX1_ReqGenChannel{
    ReqGen0 = 0,
    ReqGen1 = 1,
    ReqGen2 = 2,
    ReqGen3 = 3,
    ReqGen4 = 4,
    ReqGen5 = 5,
    ReqGen6 = 6,
    ReqGen7 = 7
};

enum class DMAMUX2_ReqGenChannel{
    ReqGen0 = 0,
    ReqGen1 = 1,
    ReqGen2 = 2,
    ReqGen3 = 3,
    ReqGen4 = 4,
    ReqGen5 = 5,
    ReqGen6 = 6,
    ReqGen7 = 7
};

struct DMAMUX1_ReqGenCfg {
    DMAMUX1_ReqGenChannel channel;
    DMAMUX1_TriggerInput trigInput;
    DMAMUX_Polarity trigPolarity;
    uint8_t enableReqGenChannel : 1;
    uint8_t enableTrigOverrunItr : 1;
    uint8_t numberGenDMARequests : 5; //The number requests generator is +1, so a value of 0 generates 1 request.

    //Initializes struct to RGxCR default values
    DMAMUX1_ReqGenCfg():
        channel(DMAMUX1_ReqGenChannel::ReqGen0), trigInput(DMAMUX1_TriggerInput::dmamux1_evt0), trigPolarity(DMAMUX_Polarity::NoEvent), enableReqGenChannel(false),
        enableTrigOverrunItr(false), numberGenDMARequests(0)
    {}
};

struct DMAMUX2_ReqGenCfg {
    DMAMUX2_ReqGenChannel channel;
    DMAMUX2_TriggerInput trigInput;
    DMAMUX_Polarity trigPolarity;
    uint8_t enableReqGenChannel : 1;
    uint8_t enableTrigOverrunItr : 1;
    uint8_t numberGenDMARequests : 5; //The number requests generator is +1, so a value of 0 generates 1 request.

    //Initializes struct to RGxCR default values
    DMAMUX2_ReqGenCfg():
        channel(DMAMUX2_ReqGenChannel::ReqGen0), trigInput(DMAMUX2_TriggerInput::dmamux2_evt0), trigPolarity(DMAMUX_Polarity::NoEvent), enableReqGenChannel(false),
        enableTrigOverrunItr(false), numberGenDMARequests(0)
    {}
};

void dmamux_configReqGenChannel(DMAMUX1_ReqGenCfg cfg);
void dmamux_configReqGenChannel(DMAMUX2_ReqGenCfg cfg);
bool dmamux_readSTrigOverrunEventflag(DMAMUX1_ReqGenChannel channel);
bool dmamux_readSTrigOverrunEventflag(DMAMUX2_ReqGenChannel channel);
void dmamux_clearTrigOverrunEventFlag(DMAMUX1_ReqGenChannel channel);
void dmamux_clearTrigOverrunEventFlag(DMAMUX2_ReqGenChannel channel);
