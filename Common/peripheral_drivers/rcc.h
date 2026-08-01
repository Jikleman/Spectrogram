#pragma once

#include <stdint.h>
#include "rcc_enums.h"

namespace dd {

struct rcc_def {
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

#define RCC_Base        (0x58024400)
#define RCC             (*(rcc_def *) RCC_Base)

};
