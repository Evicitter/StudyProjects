//Last update time : 01.07.2015 21:35:20

#include "stm32f10x.h"
#include "inc\stm32f10x_gpio.h"

/********************************************************************/
#include "inc\stm32f10x_rcc.h"
#include "stm32f10x_conf.h"
 
/********************************************************************/
GPIO_InitTypeDef port;
uint8_t buttonState;
 
/********************************************************************/
//#include "stm32_ub_led.h"
void Delay(volatile uint32_t nCount)
{
  while(nCount--)
  {}
}

int main(void) {
    // SystemInit() ?????????? ?? startup_stm32f10x_md_vl.c

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init( GPIOC , &GPIO_InitStructure);

    while (1) {
        GPIOC->ODR |= GPIO_ODR_ODR9;
        Delay(0x1FFFF);

        GPIOC->ODR &= ~GPIO_ODR_ODR9;
        Delay(0x7FFFF);
    }

    return 0;
}