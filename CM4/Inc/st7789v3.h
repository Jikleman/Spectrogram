#pragma once

#include <stdint.h>

#define SHARED_ARRAY_SIZE		10;

//For SPI 4-wire with DMA

class st7789v3 {
private:
	void setDCLine();
	void resetDCLine();
	void setRstPin();
	void resetRstPin();
	void initPeripherals();

	bool initialized = false;
public:
	static const uint32_t IC_MAX_ROWS = 320;	//For the IC, max rows is 320
	static const uint32_t IC_MAX_COLS = 240;	//For the IC, max cols is 240
	static const uint32_t MAX_ROWS = 320;		//For my display, max rows is 320
	static const uint32_t MAX_COLS = 172;		//For my display, max cols is 172
	static const uint32_t COL_OFFS = 34;		//The offset for this is 34
	enum class commands {
		//System Function Control Table 1
		NOP = 		0x00,		//NO OP
		SWRESET = 	0x01,		//Software reset
		RDDID = 	0x04,		//Read display
		RDDST = 	0x09,		//Read display status
		RDDPM = 	0x0A,		//Read display power mode
		RDDMADCTL = 0x0B,		//Read display MADCTL
		RDDCOLMOD = 0x0C,		//Read display pixel format
		RDDIM = 	0x0D,		//Read display image mode
		RDDSM = 	0x0E,		//Read display signal mode
		RDDSDR = 	0x0F,		//Read display self-diagnostic result
		SLPIN = 	0x10,		//Sleep in
		SLPOUT = 	0x11,		//Sleep out
		PTLON = 	0x12,		//Partial display mode on
		NORON = 	0x13,		//Normal display mode on
		INVOFF = 	0x20,		//Display inversion off
		INVON = 	0x21,		//Display inversion on
		GAMSET = 	0x26,		//Gamma set
		DISPOFF = 	0x28,		//Display off
		DISPON = 	0x29,		//Display on
		CASET = 	0x2A,		//Column address set
		RASET = 	0x2B,		//Row address set
		RAMWR = 	0x2C,		//Memory write
		RAMRD = 	0x2E,		//Memory read
		PTLAR = 	0x30,		//Partial area
		VSCRDEF = 	0x33,		//Vertical scrolling definition
		TEOFF = 	0x34,		//Tearing effect line off
		TEON = 		0x35,		//Tearing effect line on
		MADCTL = 	0x36,		//Memory data access control
		VSCSAD = 	0x37,		//Vertical scroll start address of RAM
		IDMOFF = 	0x38,		//Idle mode off
		IDMON = 	0x39,		//Idle mode on
		COLMOD = 	0x3A,		//Interface pixel format
		WRMEMC = 	0x3C,		//Write memory continue
		RDMEMC = 	0x3E,		//Read memory continue
		STE = 		0x44,		//Set tear scanline
		GSCAN = 	0x45,		//Get scanline
		WRDISBV = 	0x51,		//Write display brightness
		RDDISBV = 	0x52,		//Read display brightness value
		WRCTRLD = 	0x53,		//Write CTRL display
		RDCTRLD = 	0x54,		//Read CTRL value display
		WRCACE = 	0x55,		//Write content adaptive brightness control and color enhancement
		RDCABC = 	0x56,		//Read content adaptive brightness control
		WRCABCMB = 	0x5E,		//Write CABC minimum brightness
		RDCABCMD = 	0x5F,		//Read CABC minimum brightness
		RDABCSDR = 	0x68,		//Read automatic brightness control self-diagnostic result
		RDID1 = 	0xDA,		//Read ID1
		RDID2 = 	0xDB,		//Read ID2
		RDID3 = 	0xDC,		//Read ID3
		//System Function Control Table 2
		RAMCTRL = 	0xB0,		//Ram control
		RGBCTRL = 	0xB1,		//RGB interface control
		PORCTRL = 	0xB2,		//Porch setting
		FRCTRL1 = 	0xB3,		//Frame rate control 1
		PARCTRL = 	0xB5,		//Partial mode control
		GCTRL = 	0xB7,		//Gate control
		GTADJ = 	0xB8,		//Gate on timing adjustment
		DGMEN = 	0xBA,		//Digital gamma enable
		VCOMS = 	0xBB,		//VCOMS setting
		LCMCTRL =	0xC0,		//LCM Control
		IDSET =		0xC1,		//ID Code setting
		VDVVRHEN = 	0xC2,		//VDV and VRH command enable
		VRHS = 		0xC3,		//VRH set
		VDVS = 		0xC4,		//VDV set
		VCMOSFET = 	0xC5,		//COMS offset set
		FRCTRL2 = 	0xC6,		//Frame rate control in normal mode
		CABCCTRL = 	0xC7,		//CABC control
		REGSEL1 = 	0xC8,		//Register value selection 1
		REGSEL2 = 	0xCA,		//Register value selection 2
		PWMFRSEL = 	0xCC,		//PWM Frequency selection
		PWCTRL1 = 	0xD0,		//Power control 1
		VAPVANEN = 	0xD2,		//Enable VAP/VAN signal output
		GATESEL = 	0xD6,		//Gate output selection in sleep mode
		CMD2EN = 	0xDF,		//Command 2 enable
		PVGAMCTRL = 0xE0,		//Positive voltage gamma control
		NVGAMCTRL = 0xE1,		//Negative voltage gamma control
		DGMLUTR = 	0xE2,		//Digital gamma look-up table for red
		DGMLUTB = 	0xE3,		//Digital gamma look-up table for blue
		GATECTRL = 	0xE4,		//Gate control
		SPI2EN = 	0xE7,		//SPI2 enable
		PWCTRL2 = 	0xE8,		//Power control 2
		EQCTRL = 	0xE9,		//Equalize time control
		PROMCTRL = 	0xEC,		//Program mode control
		PROMEN = 	0xFA,		//Program mode enable
		NVMSET = 	0xFC,		//NVM setting
		PROMACT = 	0xFE		//Program action
	};
	void enableBacklight();
	void disableBacklight();

	st7789v3(){
		if (!initialized){
			initialized = true;
			initPeripherals();
			resetRstPin();
			setRstPin();
		}
	}
    void sendCommand(st7789v3::commands command, bool waitForTx=true);
    void sendData(uint8_t* buff, uint32_t dataLength, bool waitForTx=true);
    void waitTxComplete();
    bool ongoingTx();

    //By default, column address is the x-axis. (col,row)
    void setColAddr(uint16_t colStart, uint16_t colEnd){
    	colStart += COL_OFFS;
    	colEnd += COL_OFFS;
    	sendCommand(st7789v3::commands::CASET);
        uint8_t x[4] = {
            (uint8_t)((colStart >> 8) & 0xFF),  // MSB
            (uint8_t)( colStart        & 0xFF), // LSB
            (uint8_t)((colEnd   >> 8) & 0xFF),  // MSB
            (uint8_t)( colEnd          & 0xFF)  // LSB
        };
    	sendData(x, 4);
    }

    //By default, row address is the y-axis. (col,row)
    void setRowAddr(uint16_t rowStart, uint16_t rowEnd){
    	sendCommand(st7789v3::commands::RASET);
        uint8_t y[4] = {
            (uint8_t)((rowStart >> 8) & 0xFF),
            (uint8_t)( rowStart        & 0xFF),
            (uint8_t)((rowEnd   >> 8) & 0xFF),
            (uint8_t)( rowEnd          & 0xFF)
        };
    	sendData(y, 4);
    }
    //Fills the screen with the specified color.
    //The data for the color needs to already have its byte order reversed.
    void fill(const uint16_t colorData){
    	setColAddr(0, MAX_COLS - 1);
    	setRowAddr(0, MAX_ROWS - 1);
    	setColAddr(0, MAX_COLS - 1);
    	setRowAddr(0, MAX_ROWS - 1);
    	sendCommand(st7789v3::commands::RAMWR);
    	uint16_t buff[MAX_COLS] = {0};
    	for (unsigned int i = 0; i < MAX_COLS; i++){
    		buff[i] = colorData;
    	}
    	for (unsigned int i = 0; i < MAX_ROWS; i++){
    		sendData((uint8_t *) &buff, MAX_COLS*2);
    	}
    	sendCommand(st7789v3::commands::NOP);
    }
};
