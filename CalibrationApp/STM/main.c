//Last update time : 29.07.2015 15:31:41
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <misc.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_it.h>

#include "omnicomm.h"
#include "omni_cfind.h"
#include "main.h"

void delay(unsigned int time, char c)
{
  switch (c)
  {
    volatile uint32_t nCount;
    RCC_ClocksTypeDef RCC_Clocks;
  case 'u':    
    RCC_GetClocksFreq (&RCC_Clocks);
    nCount=(RCC_Clocks.HCLK_Frequency/8000000)*time;
    for (; nCount!=0; nCount--);
    break;
  case 'm':  
    RCC_GetClocksFreq (&RCC_Clocks);
    nCount=(RCC_Clocks.HCLK_Frequency/8000)*time;
    for (; nCount!=0; nCount--);
    break;
  case 's':  
    RCC_GetClocksFreq (&RCC_Clocks);
    nCount=(RCC_Clocks.HCLK_Frequency/80)*time;
    for (; nCount!=0; nCount--);
    break;
  }
}

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	unsigned short relfuel[2];

	ommnicomm_dat_t omnicomm_dat;

  //delay(2, 'm');

  /* GPIOC Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* System clocks configuration ---------------------------------------------*/
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
  
  GPIO_SetBits(GPIOC, GPIO_Pin_8); 
  delay(50, 'm');
  GPIO_ResetBits(GPIOC, GPIO_Pin_8);
  delay(50, 'm');
  GPIO_SetBits(GPIOC, GPIO_Pin_8);
  delay(100, 'm');
  GPIO_ResetBits(GPIOC, GPIO_Pin_8);  
	
  omnicomm_rs232_hw_init(); 
	omnicomm_rs232_ch_conf(0,'s',0xFF); // param: master, address

  //omnicomm_rs485_ch_config(0, 'e', 'm', 0xFF);  // param: #ch, 'e' = enable, 's' = slave, address

  while (1)
  {
    omnicomm_dat = omnicomm_rs232_get_value(0xFF);
    //omnicomm_dat = omnicomm_rs485_get_value(0xFF);

		relfuel[0] = omnicomm_dat.rel_level;
		relfuel[1] = omnicomm_dat.temperature;

	 GPIO_SetBits(GPIOC, GPIO_Pin_9); 
   delay(500, 'm');
	 GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		
   // delay(500, 'm');
   // GPIO_ResetBits(GPIOC, GPIO_Pin_8);
   // delay(500, 'm');
   // GPIO_SetBits(GPIOC, GPIO_Pin_8);  
  }
}
