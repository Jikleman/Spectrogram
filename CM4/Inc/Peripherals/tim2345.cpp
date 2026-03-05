#include <tim2345.h>


#define CEN         (1<<0)          //Counter enable

void tim_controls(TIMStruct* tim, TIMSettingsCR s){
    uint16_t cr1 = 0;
    uint16_t cr2 = 0;
    uint16_t resMask = 0;
    cr1 |= (s.UIFREMAP<<11) | (s.CDK<<8) | (s.ARPE<<7) | (s.CMS<<5) | (s.DIR<<4) | (s.OPM<<3) | (s.URS<<2) | (s.UDIS<<1);
    resMask |= (0xF<<12) | (1<<10); //Mask in CR1 reserved bits
    cr1 |= tim->CR1 & resMask;
    tim->CR1 = cr1;

    resMask = 0;
    cr2 |= (s.TI1S<<7) | (s.MMS<<4) | (s.CCDS<<3);
    resMask |= (0xFF<<8) | (0x7<<0); //Mask in CR2 reserved bits;
    cr2 |= tim->CR2 & resMask;
    tim->CR2 = cr2;
}
void tim_prescale(TIMStruct* tim, uint16_t presc){
    tim->PSC = presc;
}
void tim_auto_reload(TIMStruct* tim, uint32_t ar){
    tim->ARR = ar;
}

void tim_enable(TIMStruct* tim){
    tim->CR1 |= (CEN);
}
void tim_disable(TIMStruct* tim){
    tim->CR1 &= ~(CEN);
}

void tim_dma_ir_en(TIMStruct* tim, TIMSettingsDIER s){
	uint16_t dier = 0;
	dier |= (s.TDE<<14) | (s.CC4DE<<12) | (s.CC3DE<<11) | (s.CC2DE<<10) | (s.CC1DE<<9) | (s.UDE<<8) | (s.TIE<<6)
            | (s.CC4IE<<4) | (s.CC3IE<<3) | (s.CC2IE<<2) | (s.CC1IE<<1);
}

bool tim_read_flag(TIMStruct* tim, tim_flag flag){
    return tim->SR & (1<<flag);
}
void tim_clear_flag(TIMStruct* tim, tim_flag flag){
    tim->SR = (1<<flag);
}
