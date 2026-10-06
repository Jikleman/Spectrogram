#pragma once

namespace dd {

enum class AHB3 {
    mdma = 0,
    dma2d = 4,
    jpgdec = 5,
    fmc = 12,
    quadSPI = 14,
    sdmmc1 = 16
};
enum class AHB1 {
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
enum class AHB2 {
    dcmi = 0,
    crypt = 4,
    hash = 5,
    rng = 6,
    sdmmc2 = 9,

};
enum class AHB4 {
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
enum class APB3 {
    ltdc = 3,
    dsi = 4,
    wwdg1 = 6               //Not recommended to enable this for CPU2
};
enum class APB1L {
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
enum class APB1H {
    clockRecoverySystem = 1,
    swpmi = 2,
    opamp = 4,
    mdios = 5,
    fdcan = 8
};
enum class APB2 {
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
enum class APB4 {
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


//Template traits for the peripheral buses.
template <typename T> struct peripheral_bus;
template<> struct peripheral_bus<AHB3>  { static constexpr int index = 0; };
template<> struct peripheral_bus<AHB1>  { static constexpr int index = 1; };
template<> struct peripheral_bus<AHB2>  { static constexpr int index = 2; };
template<> struct peripheral_bus<AHB4>  { static constexpr int index = 3; };
template<> struct peripheral_bus<APB3>  { static constexpr int index = 4; };
template<> struct peripheral_bus<APB1L> { static constexpr int index = 5; };
template<> struct peripheral_bus<APB1H> { static constexpr int index = 6; };
template<> struct peripheral_bus<APB2>  { static constexpr int index = 7; };
template<> struct peripheral_bus<APB4>  { static constexpr int index = 8; };

};
