#ifndef __OMNICOMM_H__
#define __OMNICOMM_H__

  #include "stm32f10x.h"

enum omnicomm_flags {
  omniRST      = 0x00000001,  //
  omniTX232    = 0x00000002,  //
  omniTXD485_0 = 0x00000004,
  omniTXD485_1 = 0x00000008,
  omniTXD485_2 = 0x00000010 
};


#ifdef __UGRA_PRM__
  #define OMNICOMM_RS232_SLAVE
#endif //__UGRA_PRM__

//#define __OMNICOMM_RS485_USE_USART1__

  #define OMNICOMM_RS232                        USART3
  #define OMNICOMM_RS232_CLK                    RCC_APB1Periph_USART3
  #define OMNICOMM_RS232_TX_PIN                 GPIO_Pin_10
  #define OMNICOMM_RS232_TX_GPIO_PORT           GPIOB
  #define OMNICOMM_RS232_TX_GPIO_CLK            RCC_APB2Periph_GPIOB
  #define OMNICOMM_RS232_RX_PIN                 GPIO_Pin_11
  #define OMNICOMM_RS232_RX_GPIO_PORT           GPIOB
  #define OMNICOMM_RS232_RX_GPIO_CLK            RCC_APB2Periph_GPIOB
  #define OMNICOMM_RS232_IRQn                   USART3_IRQn
  
#ifdef __UGRA_PRM__
  #define OMNICOMM_RS485_SLAVE
#endif //__UGRA_PRM__

  #define __OMNICOMM_RS485_USE_USART1__

  #define OMNICOMM_RS485                        USART1
  #define OMNICOMM_RS485_CLK                    RCC_APB2Periph_USART1
  #define OMNICOMM_RS485_TX_PIN                 GPIO_Pin_9
  #define OMNICOMM_RS485_TX_GPIO_PORT           GPIOA
  #define OMNICOMM_RS485_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
  #define OMNICOMM_RS485_RX_PIN                 GPIO_Pin_10
  #define OMNICOMM_RS485_RX_GPIO_PORT           GPIOA
  #define OMNICOMM_RS485_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
  #define OMNICOMM_RS485_TXE_PIN                GPIO_Pin_11
  #define OMNICOMM_RS485_TXE_GPIO_PORT          GPIOA
  #define OMNICOMM_RS485_TXE_GPIO_CLK           RCC_APB2Periph_GPIOA
  #define OMNICOMM_RS485_IRQn                   USART1_IRQn
  
  
  typedef struct {
    uint8_t  status      ;
    uint8_t  address     ;
    int16_t  temperature ;
    uint16_t rel_level   ;
    uint16_t frequency   ;  
  }ommnicomm_dat_t;
  
  /* common functions */
  char omnicomm_check_crc(unsigned char * arr, unsigned short len);
  void omnicomm_sys_tim_1s_irq(void);
  void omnicomm_set_flag(unsigned int flag);
  void omnicomm_reset_flag(unsigned int flag);
  void omnicomm_loop(void);
  
  /* rs232 functions */
  void omnicomm_rs232_hw_init(void);
  void omnicomm_rs232_ch_conf(char enable, char mode, unsigned char address);
  ommnicomm_dat_t omnicomm_rs232_get_value(unsigned char address);
  void omnicomm_rs232_set_value(ommnicomm_dat_t value);
  void omnicomm_rs232_usart_irq(unsigned char rx_byte);
  
  
  /* rs485 functions */
  char omnicomm_rs485_ch_config(unsigned char channel, char enable, char mode, unsigned char address);
  void omnicomm_rs485_hw_init(void);
  ommnicomm_dat_t omnicomm_rs485_get_value(unsigned char address);
  void omnicomm_rs485_get_value_req(unsigned char channel);
  char omnicomm_rs485_set_value(unsigned char channel, ommnicomm_dat_t value);
  void omnicomm_rs485_usart_irq(unsigned char rx_byte);



#endif //__OMNICOMM_H__
