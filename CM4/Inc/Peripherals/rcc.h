#pragma once

#include <stdint.h>

#define RCC_Base        (0x58024400)

struct RCCStruct{
    volatile uint32_t CR;                       // Source control
    volatile uint32_t HSICFGR;                  // HSI configuration
    volatile uint32_t CRRCR;                    // Clock recovery RC
    volatile uint32_t CSICFGR;                  // CSI configuration
    volatile uint32_t CFGR;                     // Clock configuration
    volatile uint32_t Reserved0;                         // Reserved field
    volatile uint32_t D1CFGR;                   // Domain 1 clock configuration
    volatile uint32_t D2CFGR;                   // Domain 2 clock configuration
    volatile uint32_t D3CFGR;                   // Domain 3 clock configuration
    volatile uint32_t Reserved1;                         // Reserved field
    volatile uint32_t PLLCKSELR;                // PLLs clock source selection
    volatile uint32_t PLLCFGR;                  // PLL configuration
    volatile uint32_t PLL1DIVR;                 // PLL1 dividers configuration
    volatile uint32_t PLL1FRACR;                // PLL1 fractional divider
    volatile uint32_t PLL2DIVR;                 // PLL2 dividers configuration
    volatile uint32_t PLL2FRACR;                // PLL2 fractional divider
    volatile uint32_t PLL3DIVR;                 // PLL3 dividers configuration
    volatile uint32_t PLL3FRACR;                // PLL3 fractional divider
    volatile uint32_t Reserved2;                         // Reserved field
    volatile uint32_t D1CCIPR;                  // Domain 1 kernel clock configuration
    volatile uint32_t D2CCIP1R;                 // Domain 2 kernel clock configuration 1
    volatile uint32_t D2CCIP2R;                 // Domain 2 kernel clock configuration 2
    volatile uint32_t D3CCIPR;                  // Domain 3 kernel clock configuration
    volatile uint32_t Reserved3;                         // Reserved field
    volatile uint32_t CIER;                     // Clock source interrupt enable
    volatile uint32_t CIFR;                     // Clock source interrupt flag
    volatile uint32_t CICR;                     // Clock source interrupt clear
    volatile uint32_t Reserved4;                         // Reserved field
    volatile uint32_t BDCR;                     // Backup domain control
    volatile uint32_t CSR;                      // Clock control and status
    volatile uint32_t Reserved5;                         // Reserved field
    volatile uint32_t AHB3RSTR;                 // AHB3 reset
    volatile uint32_t AHB1RSTR;                 // AHB1 peripheral reset
    volatile uint32_t AHB2RSTR;                 // AHB2 peripheral reset
    volatile uint32_t AHB4RSTR;                 // APB4 peripheral reset
    volatile uint32_t APB3RSTR;                 // APB3 peripheral reset
    volatile uint32_t APB1LRSTR;                // APB1 peripheral reset low
    volatile uint32_t APB1HRSTR;                // APB1 peripheral reset high
    volatile uint32_t APB2RSTR;                 // APB2 peripheral reset
    volatile uint32_t APB4RSTR;                 // APB4 peripheral reset
    volatile uint32_t GCR;                      // Global control
    volatile uint32_t Reserved6;                         // Reserved field
    volatile uint32_t D3AMR;                    // D3 Autonomous mode
    volatile uint32_t Reserved7[9];                      // Reserved field
    volatile uint32_t RSR;                      // Reset status
    volatile uint32_t AHB3ENR;                  // AHB3 clock
    volatile uint32_t AHB1ENR;                  // AHB1 clock
    volatile uint32_t AHB2ENR;                  // AHB2 clock
    volatile uint32_t AHB4ENR;                  // APB4 clock
    volatile uint32_t APB3ENR;                  // APB3 clock
    volatile uint32_t APB1LENR;                 // APB1 clock
    volatile uint32_t APB1HENR;                 // APB1 clock
    volatile uint32_t APB2ENR;                  // APB2 clock
    volatile uint32_t APB4ENR;                  // APB4 clock
    volatile uint32_t Reserved8;                         // Reserved field
    volatile uint32_t AHB3LPENR;                // AHB3 sleep clock
    volatile uint32_t AHB1LPENR;                // AHB1 sleep clock
    volatile uint32_t AHB2LPENR;                // AHB2 sleep clock
    volatile uint32_t AHB4LPENR;                // APB4 sleep clock
    volatile uint32_t APB3LPENR;                // APB3 sleep clock
    volatile uint32_t APB1LLPENR;               // APB1 sleep clock low
    volatile uint32_t APB1HLPENR;               // APB1 sleep clock high
    volatile uint32_t APB2LPENR;                // APB2 sleep clock
    volatile uint32_t APB4LPENR;                // APB4 sleep clock
    volatile uint32_t Reserved9[4];                      // Reserved field
    volatile uint32_t C1_RSR;                   // C1 Reset status
    volatile uint32_t C1_AHB3ENR;               // C1 AHB3 clock
    volatile uint32_t C1_AHB1ENR;               // C1 AHB1 clock
    volatile uint32_t C1_AHB2ENR;               // C1 AHB2 clock
    volatile uint32_t C1_AHB4ENR;               // C1 APB4 clock
    volatile uint32_t C1_APB3ENR;               // C1 APB3 clock
    volatile uint32_t C1_APB1LENR;              // C1 APB1 clock
    volatile uint32_t C1_APB1HENR;              // C1 APB1 clock
    volatile uint32_t C1_APB2ENR;               // C1 APB2 clock
    volatile uint32_t C1_APB4ENR;               // C1 APB4 clock
    volatile uint32_t Reserved10;                        // Reserved field
    volatile uint32_t C1_AHB3LPENR;             // C1 AHB3 sleep clock
    volatile uint32_t C1_AHB1LPENR;             // C1 AHB1 sleep clock
    volatile uint32_t C1_AHB2LPENR;             // C1 AHB2 sleep clock
    volatile uint32_t C1_AHB4LPENR;             // C1 APB4 sleep clock
    volatile uint32_t C1_APB3LPENR;             // C1 APB3 sleep clock
    volatile uint32_t C1_APB1LLPENR;            // C1 APB1 sleep clock
    volatile uint32_t C1_APB1HLPENR;            // C1 APB1 sleep clock
    volatile uint32_t C1_APB2LPENR;             // C1 APB2 sleep clock
    volatile uint32_t C1_APB4LPENR;             // C1 APB4 sleep clock
    volatile uint32_t Reserved11[3];                     // Reserved field
    volatile uint32_t C2_RSR;                   // C2 Reset status
    volatile uint32_t C2_AHB3ENR;               // C2 AHB3 clock
    volatile uint32_t C2_AHB1ENR;               // C2 AHB1 clock
    volatile uint32_t C2_AHB2ENR;               // C2 AHB2 clock
    volatile uint32_t C2_AHB4ENR;               // C2 APB4 clock
    volatile uint32_t C2_APB3ENR;               // C2 APB3 clock
    volatile uint32_t C2_APB1LENR;              // C2 APB1 clock
    volatile uint32_t C2_APB1HENR;              // C2 APB1 clock
    volatile uint32_t C2_APB2ENR;               // C2 APB2 clock
    volatile uint32_t C2_APB4ENR;               // C2 APB4 clock
    volatile uint32_t Reserved12;                        // Reserved field
    volatile uint32_t C2_AHB3LPENR;             // C2 AHB3 sleep clock
    volatile uint32_t C2_AHB1LPENR;             // C2 AHB1 sleep clock
    volatile uint32_t C2_AHB2LPENR;             // C2 AHB2 sleep clock
    volatile uint32_t C2_AHB4LPENR;             // C2 APB4 sleep clock
    volatile uint32_t C2_APB3LPENR;             // C2 APB3 sleep clock
    volatile uint32_t C2_APB1LLPENR;            // C2 APB1 sleep clock
    volatile uint32_t C2_APB1HLPENR;            // C2 APB1 sleep clock
    volatile uint32_t C2_APB2LPENR;             // C2 APB2 sleep clock
    volatile uint32_t C2_APB4LPENR;             // C2 APB4 sleep clock
    volatile int32_t Reserved13[3];                     // Reserved field
};

#define RCC             ((RCCStruct*) RCC_Base)

enum class SysCPU {
    CurrentCPU = 0,
    M7 = 1,             //M7 is specifically called CPU1 in documentation
    M4 = 2              //M4 is specifically called CPU2 in documentation
};

enum class AHB3_Peripheral {
    mdma = 0,
    dma2d = 4,
    jpgdec = 5,
    fmc = 12,
    quadSPI = 14,
    sdmmc1 = 16
};
enum class AHB1_Peripheral {
    dma1 = 0,
    dma2 = 1,
    adc1_adc2 = 5,
    art = 14,
    eth1_mac = 15,
    eth1_tx = 16,
    eth1_rx = 17,
    usb1_otg = 25,
    usb1_phy1 = 26,
    usb2_otg = 27,
    usb_phy2 = 28
};
enum class AHB2_Peripheral {
    dcmi = 0,
    crypt = 4,
    hash = 5,
    rng = 6,
    sdmmc2 = 9,

};
enum class AHB4_Peripheral {
    gpioa = 0,
    gpiob = 1,
    gpioc = 2,
    gpiod = 3,
    gpioe = 4,
    gpiof = 5,
    gpiog = 6,
    gpioh = 7,
    gpioi = 8,
    gpioj = 9,
    gpiok = 10,
    crc = 19,
    bdma = 21,
    adc3 = 24,
    hsem = 25,
    backupRam = 28
};
enum class APB3_Peripheral{
    ltdc = 3,
    dsi = 4,
    wwdg1 = 6               //Not recommended to enable this for CPU2
};
enum class APB1L_Peripheral{
    tim2 = 0,
    tim3 = 1,
    tim4 = 2,
    tim5 = 3,
    tim6 = 4,
    tim7 = 5,
    tim12 = 6,
    tim13 = 7,
    tim14 = 8,
    lptim1 = 9,
    wwdg2 = 11,             //Not recommended to enable this for CPU1
    spi2 = 14,
    spi3 = 15,
    spdifrx = 16,
    usart2 = 17,
    usart3 = 18,
    usart4 = 19,
    usart5 = 20,
    i2c1 = 21,
    i2c2 = 22,
    i2c3 = 23,
    hdmi_cec = 24,
    dac1_dac2 = 29,
    uart7 = 30,
    uart8 = 31
};
enum class APB1H_Peripheral{
    clockRecoverySystem = 1,
    swpmi = 2,
    opamp = 4,
    mdios = 5,
    fdcan = 8
};
enum class APB2_Peripheral{
    tim1 = 0,
    tim8 = 1,
    usart1 = 4,
    usart6 = 5,
    spi1 = 12,
    spi4 = 13,
    tim15 = 16,
    tim16 = 17,
    tim17 = 18,
    spi5 = 20,
    sai1 = 22,
    sai2 = 23,
    sai3 = 24,
    dfsdm1 = 28,
    hrtim = 29
};
enum class APB4_Peripheral{
    syscfg  = 1,
    lpuart1 = 3,
    spi6 = 5,
    i2c4 = 7,
    lptim2 = 9,
    lptim3 = 10,
    lptim4 = 11,
    lptim5 = 12,
    comp1_comp2 = 14,
    vrefbuf = 15,
    rtcapb = 16,            //This is enabled by default
    sai4 = 17
};

//Allocates the peripheral

void allocatePeripheral(AHB3_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(AHB1_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(AHB2_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(AHB4_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(APB3_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(APB1L_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(APB1H_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(APB2_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void allocatePeripheral(APB4_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);

//Resets the peripheral

void resetPeripheral(AHB3_Peripheral peripheral);
void resetPeripheral(AHB1_Peripheral peripheral);
void resetPeripheral(AHB2_Peripheral peripheral);
void resetPeripheral(AHB4_Peripheral peripheral);
void resetPeripheral(APB3_Peripheral peripheral);
void resetPeripheral(APB1L_Peripheral peripheral);
void resetPeripheral(APB1H_Peripheral peripheral);
void resetPeripheral(APB2_Peripheral peripheral);
void resetPeripheral(APB4_Peripheral peripheral);

//Resets the peripheral then allocates the peripheral

void freshAllocPeripheral(AHB3_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(AHB1_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(AHB2_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(AHB4_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(APB3_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(APB1L_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(APB1H_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(APB2_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void freshAllocPeripheral(APB4_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);

//If the peripheral has not been allocated to a CPU, then reset it before allocating. Otherwise just allocate the peripheral.

void reallocPeripheral(AHB3_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(AHB1_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(AHB2_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(AHB4_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(APB3_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(APB1L_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(APB1H_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(APB2_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
void reallocPeripheral(APB4_Peripheral peripheral, SysCPU CPU=SysCPU::CurrentCPU);
