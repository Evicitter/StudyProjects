#include "stm32f10x.h"
#include "omnicomm.h"
#include "omni_cfind.h"
//#include "uart_printf.h"
#include "main.h"

#include <stm32f10x_gpio.h> // ?????????? ??? ?????? ? ??????? ????? - ??????
#include <stm32f10x_rcc.h> // ?????? ? ?????????????
#include <misc.h> // ?????? ? ????????????
#include <stm32f10x_exti.h> // ?????? ? ???????? ????????????
#include <stm32f10x_tim.h> // ?????? ? ?????????
#include <stm32f10x_usart.h>
#include <stm32f10x_it.h>

#include <string.h>

//#define OMNICOMM_DEBUG_PRINTF

#define OMNICOMM_WITHOUT_WAIT

#define RS232_TX_BUF_SZ   4
#define RS232_RX_BUF_SZ   16
#define RS485_RX_BUF_SZ   16

#define REQUEST_MSG_SZ      4
#define RESPONSE_MSG_SZ     9
#define RESPONSE_MSG_MAX_SZ 9

#define OMNICOMM_RS485_ADDR_NUM  3

/* Private data structs */

typedef struct{
  unsigned short p_read;
 unsigned short p_write;
 unsigned short size;
 char *array;
} fifo_t;

void PushChar(volatile fifo_t *fifo, char x){
  *(fifo->array + fifo->p_write) = x;
 fifo->p_write = (++fifo->p_write) % fifo->size;
}

int PopChar(volatile fifo_t* fifo, char* x){
  if (fifo->p_write != fifo->p_read){
    *x = *(fifo->array + fifo->p_read);
    fifo->p_read = (++fifo->p_read) % fifo->size;
    return 1;
  }
  else{
    return 0;
  }
}

typedef struct{
  char    enable  ;
  char    g_mode  ;  // 's' slave or 'm' master
  uint8_t address ;
  char    sl_mode ;  // 's' single by request or 'p' auto periodic 
  uint8_t period  ;  // period in sec
}ommnicom_sens_mode_t;


/* Private variables */
uint32_t omnicomm_ctrl_reg = 0x00000000;

/* Private functions */
void omnicomm_send_value(uint8_t interface, uint8_t use_command, ommnicomm_dat_t value);
void omnicomm_send_command(uint8_t interface, uint8_t address, uint8_t use_command, uint8_t resp_stat);
void USART_SendCharArray(USART_TypeDef* USARTx, uint8_t * arr, uint16_t len);
unsigned char calc_crc( unsigned char *mas, unsigned char Len);


void USART_SendCharArray(USART_TypeDef* USARTx, uint8_t * arr, uint16_t len)
{
	unsigned short i;
	//unsigned short pTimeOut = 65535;
  for (i = 0; i < len; i++)
  {    
    USART_SendData(USARTx, arr[i]);
    while ((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) ) //&& (pTimeOut>0))
    {
			//--pTimeOut;
		}
  }
}

/* Go */

//-----------------------------------------------------------------------------------------------------
// RS232 part
//-----------------------------------------------------------------------------------------------------

uint8_t omnicomm_sec_cnt = 0;
uint8_t rs232_tx_buff[RS232_TX_BUF_SZ];
uint8_t rs232_tx_buff_cnt;
uint8_t rs232_tx_flag;
uint8_t rs232_rx_buff[RS232_RX_BUF_SZ];
uint8_t rs232_rx_buff_cnt;
uint8_t rs232_rx_flag;
ommnicom_sens_mode_t sens_rs232_mode; 
ommnicomm_dat_t sens_rs232_out_value;
omni_packet_t rs232_find_out;
omni_cfind_obj_t rs232_find_object;

void omnicomm_rs232_hw_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Clear all variables */
  rs232_rx_buff_cnt = 0;
  rs232_rx_flag     = 0;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(OMNICOMM_RS232_TX_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USART Clock */
#ifdef __OMNICOMM_RS232_USE_USART1__
  RCC_APB2PeriphClockCmd(OMNICOMM_RS232_CLK, ENABLE);  
#else //__OMNICOMM_RS232_USE_USART1__
  RCC_APB1PeriphClockCmd(OMNICOMM_RS232_CLK, ENABLE);  
#endif //__OMNICOMM_RS232_USE_USART1__ 
	
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = OMNICOMM_RS232_RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(OMNICOMM_RS232_RX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = OMNICOMM_RS232_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(OMNICOMM_RS232_TX_GPIO_PORT, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(OMNICOMM_RS232, &USART_InitStructure);

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = OMNICOMM_RS232_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xFF;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0xFF;
	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable USART Receive and Transmit interrupts */
  USART_ITConfig(OMNICOMM_RS232, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(OMNICOMM_RS232, USART_IT_TXE, ENABLE);

	//USART3->CR1 |= USART_CR1_RXNEIE;

  /* Enable USART */
  USART_Cmd(OMNICOMM_RS232, ENABLE);   
	
	//USART3->CR1 |= USART_CR1_UE;
}

int Ros=0;
char Bos[10] = {0};

void USART3_IRQHandler(void)
{
 unsigned char data;

 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //if(USART3->SR & USART_SR_RXNE)
 {
	 USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	 data = USART_ReceiveData(USART3);
	 omnicomm_rs232_usart_irq( data );

	 Bos[Ros]=data;
	 ++Ros;
	 if(Ros>=9)
	 {
		 int a = Bos[0];
		 int b = Bos[1];
		 int c = Bos[2];
		 int d = Bos[3];
		 int e = Bos[4];
		 int f = Bos[5];
		 int g = Bos[6];
		 int h = Bos[7];
		 int i = Bos[8];
		 //int j = Bos[9];
		 Ros=0;
	 }
	 
	 //priem
	 //GPIO_SetBits(GPIOC, GPIO_Pin_8); 
   //delay(50, 'm');
	 //GPIO_ResetBits(GPIOC, GPIO_Pin_8);
 }

 if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
 {
	 USART_ClearITPendingBit(USART3, USART_IT_TXE);
	 //USART_ITConfig(OMNICOMM_RS232, USART_IT_TXE, DISABLE);
 }
 
 
 /*
  short src = USART3->SR;
  
 if(USART3->SR & USART_SR_RXNE)
 {
  USART3->SR &= ~USART_SR_RXNE; // Clear the USART Receive interrupt
  omnicomm_rs232_usart_irq(USART3->DR);
 }
 
 if(USART3->SR & USART_SR_TXE)
 {   
   // char c;
  if(PopChar(&FIFO_TX, &c))  USART3->DR = c;
  else										USART3->CR1 &= ~USART_CR1_TXEIE;
 }
 */
}

void omnicomm_rs232_ch_conf(char enable, char mode, uint8_t address)
{
  sens_rs232_mode.enable = enable;
  sens_rs232_mode.g_mode = mode; // Set using mode
  sens_rs232_mode.address = address; // Set address
  sens_rs232_mode.sl_mode = 's'; // Default is single data out mode
  sens_rs232_mode.period = 0; // Default period is null
}

ommnicomm_dat_t omnicomm_rs232_get_value(uint8_t address)
{
  unsigned char msg[REQUEST_MSG_SZ];
  uint8_t timeout = 500;
  ommnicomm_dat_t out_dat;
  
  /* Shape request message */  
  msg[0] = 0x31;
  msg[1] = address;
  msg[2] = 0x06;
  msg[3] = calc_crc(&msg[0], REQUEST_MSG_SZ-1);  
    
  rs232_rx_buff_cnt = 0;  /// clear uasrt rx buff counter
  rs232_rx_flag = 0;      /// clear usart rx command flag

  //USART_SendCharArray(OMNICOMM_RS232, &msg[0], REQUEST_MSG_SZ);

  /* Wait of reciving response message */  
  while((rs232_rx_flag != 1) && (0 != timeout))
  {
    timeout--;
    delay(1, 'm');
  }
  
  if (0 == timeout)
  {
    out_dat.status = 't';  /// set "Timeout" status
    return out_dat;
  }
  
  if ('e' == omnicomm_check_crc(rs232_rx_buff, RESPONSE_MSG_SZ))
  {
    out_dat.status = 'e';  /// set "CRC error" status
    return out_dat;    
  }
  else
  {
    out_dat.status = 's';  /// set "Success" status
    out_dat.address     = rs232_rx_buff[1];
    out_dat.temperature = rs232_rx_buff[3];
    out_dat.rel_level   = (uint16_t)rs232_rx_buff[4] | ((uint16_t)rs232_rx_buff[5] << 8);
    out_dat.frequency   = (uint16_t)rs232_rx_buff[6] | ((uint16_t)rs232_rx_buff[7] << 8);
    return out_dat;   
  }
}


void omnicomm_rs232_set_value(ommnicomm_dat_t value)
{
  sens_rs232_out_value = value; 
}


void omnicomm_rs232_usart_irq(uint8_t rx_byte)
{     
  rs232_rx_buff[rs232_rx_buff_cnt] = rx_byte;
  if (rs232_rx_buff_cnt < RS232_RX_BUF_SZ-1)
  {
    rs232_rx_buff_cnt++;
  }
  else
  {
    rs232_rx_buff_cnt = 0;
  }
  
  if (SUCCESS == omni_cfind_slave(&rs232_find_object, rx_byte, &rs232_find_out))
  {
    if (('e' == sens_rs232_mode.enable) && ('s' == sens_rs232_mode.sl_mode))// if enable
    {
      if (rs232_find_out.arr[1] == sens_rs232_mode.address) // compare addresses
      {
        if (0x06 == rs232_find_out.arr[2])
        {
          sens_rs232_mode.sl_mode = 's'; // set single mode
          omnicomm_send_value(0, 0x06, sens_rs232_out_value); 
        }
        if (0x07 == rs232_find_out.arr[2])
        {
          sens_rs232_mode.sl_mode = 'p'; // set period mode 
          omnicomm_send_command(0, sens_rs232_out_value.address, 0x07, 0x00);            
        }
        if (0x13 == rs232_find_out.arr[2])
        {
          sens_rs232_mode.period = rs232_find_out.arr[3]; // set period
          omnicomm_send_command(0, sens_rs232_out_value.address, 0x13, 0x00);            
        }
      }
    }
  }
  
  /* Getting data in master mode */
  if((rs232_rx_buff_cnt == RESPONSE_MSG_SZ) && (sens_rs232_mode.g_mode == 'm'))
  {
    rs232_rx_flag = 1;
  }
}

void omnicomm_sys_tim_1s_irq(void)
{
  if ('p' == sens_rs232_mode.sl_mode)
  {
    if (omnicomm_sec_cnt >= sens_rs232_mode.period)
    {
      omnicomm_sec_cnt = 0;
      omnicomm_send_value(0, 0x07, sens_rs232_out_value);
    }
    else
    {
      omnicomm_sec_cnt++;
    }
  }
}


//-----------------------------------------------------------------------------------------------------
// RS485 part
//-----------------------------------------------------------------------------------------------------

uint8_t rs485_rx_buff[RS485_RX_BUF_SZ];
uint8_t rs485_rx_buff_cnt;
uint8_t rs485_rx_flag;

uint8_t rs485_rx_req_channel;

ommnicom_sens_mode_t sens_rs485_mode[OMNICOMM_RS485_ADDR_NUM]; 
ommnicomm_dat_t sens_rs485_in_value[OMNICOMM_RS485_ADDR_NUM];
ommnicomm_dat_t sens_rs485_out_value[OMNICOMM_RS485_ADDR_NUM];

uint8_t rs485_response_flag = 0;
uint8_t rs485_response_channel;
uint8_t rs485_hw_init_flag = 0;
omni_packet_t rs485_find_out;
omni_cfind_obj_t rs485_find_object;



char omnicomm_rs485_ch_config(uint8_t channel, char enable, char mode, uint8_t address)
{
  if (OMNICOMM_RS485_ADDR_NUM > channel) // channel number check 
  {
    sens_rs485_mode[channel].enable = enable; 
    
    if ('e' != enable) 
    {
      //sens_rs485_ena_matrix &= (0x0001 << channel) ^ 0xFFFF; // disable channel
      return 'd'; // channel disable complete
    }
    
    sens_rs485_mode[channel].g_mode  = mode;    // Set using mode
    sens_rs485_mode[channel].address = address; // Set address
    sens_rs485_mode[channel].sl_mode = 's';     // Default is single data out mode
    sens_rs485_mode[channel].period  = 0;       // Default period is null
        
    if (0 == rs485_hw_init_flag) // If hardware initialization hasn't done. 
    {
      omni_cfind_obj_clear(&rs485_find_object); // Claer command find object
      omnicomm_rs485_hw_init();
      rs485_hw_init_flag = 1; 
    }
    return 's'; // succsess
  }
  return 'e'; // error in channel number
}




void omnicomm_rs485_hw_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
  /* Clear all variables */
  rs485_rx_buff_cnt = 0;
  rs485_rx_flag = 0;  
  
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(OMNICOMM_RS485_TX_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USART Clock */
#ifdef __OMNICOMM_RS485_USE_USART1__
  RCC_APB2PeriphClockCmd(OMNICOMM_RS485_CLK, ENABLE);   
#else //__OMNICOMM_RS485_USE_USART1__
  RCC_APB1PeriphClockCmd(OMNICOMM_RS485_CLK, ENABLE);   
#endif //__OMNICOMM_RS485_USE_USART1__
  

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = OMNICOMM_RS485_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(OMNICOMM_RS485_RX_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = OMNICOMM_RS485_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(OMNICOMM_RS485_TX_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure pin as Tx Enable */
  GPIO_InitStructure.GPIO_Pin = OMNICOMM_RS485_TXE_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(OMNICOMM_RS485_TXE_GPIO_PORT, &GPIO_InitStructure);
  
  /* RS485 in recive mode */
  GPIO_ResetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);

  USART_InitStructure.USART_BaudRate = 19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(OMNICOMM_RS485, &USART_InitStructure);

  /* Configure the NVIC Preemption Priority Bits */  
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = OMNICOMM_RS485_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable USART Receive and Transmit interrupts */
  USART_ITConfig(OMNICOMM_RS485, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(OMNICOMM_RS485, USART_IT_TC, ENABLE);

  /* Enable USART */
  USART_Cmd(OMNICOMM_RS485, ENABLE);
}

ommnicomm_dat_t omnicomm_rs485_get_value(uint8_t channel)
{
#ifdef OMNICOMM_WITHOUT_WAIT

  return sens_rs485_in_value[channel];

#else //OMNICOMM_WITHOUT_WAIT

  unsigned char msg[REQUEST_MSG_SZ];
  uint8_t timeout = 200;
  ommnicomm_dat_t out_dat;
  
  if (channel >= OMNICOMM_RS485_ADDR_NUM)
  {
    out_dat.status = 'e';
    return out_dat; // set channel error
  }  
  
  /* Shape request message */  
  msg[0] = 0x31;
  msg[1] = sens_rs485_mode[channel].address;
  msg[2] = 0x06;
  msg[3] = calc_crc(&msg[0], REQUEST_MSG_SZ-1);
    
  rs485_rx_buff_cnt = 0;  /// clear uasrt rx buff counter
  rs485_rx_flag = 0;      /// clear usart rx command flag
  
  GPIO_SetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
  USART_SendCharArray(OMNICOMM_RS485, &msg[0], REQUEST_MSG_SZ);
  GPIO_ResetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
  
  omnicomm_send_command(1, sens_rs485_mode[channel].address, 0x06, 0);

  /* Wait of reciving response message */  
  while((rs485_rx_flag != 1) && (0 != timeout))
  {
    timeout--;
    delay(1, 'm');
  }
  
  /* Check timeout event */
  if (0 == timeout)
  {
    out_dat.status = 't';  /// set error status
#ifdef OMNICOMM_DEBUG_PRINTF
    printf("timout\n\r");
#endif //OMNICOMM_DEBUG_PRINTF
    return out_dat;
  }

  out_dat.status = 's';  /// set "Success" status
  out_dat.address     = rs485_find_out.arr[1];
  out_dat.temperature = rs485_find_out.arr[3];
  out_dat.rel_level   = (uint16_t)rs485_find_out.arr[4] | ((uint16_t)rs485_find_out.arr[5] << 8);
  out_dat.frequency   = (uint16_t)rs485_find_out.arr[6] | ((uint16_t)rs485_find_out.arr[7] << 8);
  return out_dat;
  
#endif //OMNICOMM_WITHOUT_WAIT  
}



void omnicomm_rs485_get_value_req(uint8_t channel){
  unsigned char msg[REQUEST_MSG_SZ];
	
  /* Check pre-channel */
  if (0 == rs485_rx_flag) {
    sens_rs485_in_value[rs485_rx_req_channel].status = 't'; // set timeout status
  }

  if (channel < OMNICOMM_RS485_ADDR_NUM)
  {
    /* Shape request message */  
    msg[0] = 0x31;
    msg[1] = sens_rs485_mode[channel].address;
    msg[2] = 0x06;
    msg[3] = calc_crc(&msg[0], REQUEST_MSG_SZ-1);  
    
    rs485_rx_buff_cnt = 0;  /// clear uasrt rx buff counter
    rs485_rx_flag = 0;      /// clear usart rx command flag
    rs485_rx_req_channel = channel;
    
    GPIO_SetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
    USART_SendCharArray(OMNICOMM_RS485, &msg[0], REQUEST_MSG_SZ);
    GPIO_ResetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
    
    omnicomm_send_command(1, sens_rs485_mode[channel].address, 0x06, 0);
  }
}



char omnicomm_rs485_set_value(uint8_t channel, ommnicomm_dat_t value)
{
  if (OMNICOMM_RS485_ADDR_NUM > channel)
  {
    sens_rs485_out_value[channel] = value; 
    return 's'; // success
  }
  return 'e'; // channel don't exist.
}



void omnicomm_rs485_usart_irq(unsigned char rx_byte)
{      
	unsigned char channel;
	
#ifdef OMNICOMM_DEBUG_PRINTF    
  /* Rx data sniffer */
  if (0x3E == rx_byte)
  {
    printf("\n\r%x ", rx_byte);
  }
  else
  {
    printf("%x ", rx_byte);
  }
  
  rs485_rx_buff[rs485_rx_buff_cnt] = rx_byte;
  if (rs485_rx_buff_cnt >= RS485_RX_BUF_SZ-1)
  {
    rs485_rx_buff_cnt = 0;
  }
  else
  {
    rs485_rx_buff_cnt++;
  }    
#endif //OMNICOMM_DEBUG_PRINTF    
  
  
#ifdef OMNICOMM_RS485_SLAVE
  if (SUCCESS == omni_cfind_slave(&rs485_find_object, rx_byte, &rs485_find_out))
  {
    for (uint8_t channel = 0; channel < OMNICOMM_RS485_ADDR_NUM; channel++)
    {
      if ('e' == sens_rs485_mode[channel].enable) // if channel is enable
      {
        if (rs485_find_out.arr[1] == sens_rs485_mode[channel].address) // compare addresses
        {
          if (0x06 == rs485_find_out.arr[2])
          {
            printf ("\n\r");
            /* In current channel recive right request command 0x06 */
            sens_rs485_out_value[channel].address = sens_rs485_mode[channel].address;
            //delay (5,'m');
            //omnicomm_send_value(1, 0x06, sens_rs485_out_value[channel]);
            if (channel == 0)
              omnicomm_set_flag(omniTXD485_0);
            else if (channel == 1)
              omnicomm_set_flag(omniTXD485_1);
            else if (channel == 2)
              omnicomm_set_flag(omniTXD485_2);
          }    
        }
      }
    }        
  }
#else //OMNICOMM_RS485_SLAVE
  if (SUCCESS == omni_cfind_master(&rs485_find_object, rx_byte, &rs485_find_out))
  {
    for (channel = 0; channel < OMNICOMM_RS485_ADDR_NUM; channel++)
    {     
      if ('e' == sens_rs485_mode[channel].enable) // if channel is enable
      {        
        if (rs485_find_out.arr[1] == sens_rs485_mode[channel].address) // compare addresses
        {
          if (0x06 == rs485_find_out.arr[2])
          {
            /* In current channel recive right response command 0x06 */
            rs485_rx_flag = 1;     
            
            sens_rs485_in_value[channel].status = 's';  /// set "Success" status
            sens_rs485_in_value[channel].address     = rs485_find_out.arr[1];
            sens_rs485_in_value[channel].temperature = rs485_find_out.arr[3];
            sens_rs485_in_value[channel].rel_level   = (uint16_t)rs485_find_out.arr[4] | ((uint16_t)rs485_find_out.arr[5] << 8);
            sens_rs485_in_value[channel].frequency   = (uint16_t)rs485_find_out.arr[6] | ((uint16_t)rs485_find_out.arr[7] << 8);
            
            ///* Debug */
            //rs485_rx_buff_cnt = 0;
            //memset(&rs485_rx_buff[0], 0, RS485_RX_BUF_SZ);              
          }   
        }
      }
    }        
  }
#endif //OMNICOMM_RS485_SLAVE
}




//-----------------------------------------------------------------------------------------------------
// Common part
//-----------------------------------------------------------------------------------------------------

void omnicomm_set_flag(uint32_t flag){
  omnicomm_ctrl_reg |= flag; 
}

void omnicomm_reset_flag(uint32_t flag){
  omnicomm_ctrl_reg &= (flag ^ 0xFFFFFFFF);
}

void omnicomm_loop(void){
  if (omniTXD485_0 & omnicomm_ctrl_reg){
    
    delay(10, 'm');
    omnicomm_send_value(1, 0x06, sens_rs485_out_value[0]);
    omnicomm_reset_flag(omniTXD485_0);
    
  } else if (omniTXD485_1 & omnicomm_ctrl_reg){
    
    delay(10, 'm');    
    omnicomm_send_value(1, 0x06, sens_rs485_out_value[1]);
    omnicomm_reset_flag(omniTXD485_1);
    
  } else if (omniTXD485_2 & omnicomm_ctrl_reg){
    
    delay(10, 'm');    
    omnicomm_send_value(1, 0x06, sens_rs485_out_value[2]);
    omnicomm_reset_flag(omniTXD485_2);
    
  }
}


/*  */
void omnicomm_send_command(uint8_t interface, uint8_t address, uint8_t use_command, uint8_t resp_stat)
{
  unsigned char msg[RESPONSE_MSG_MAX_SZ];
  uint8_t byte_to_send = 0;
  
  if ((0x07 == use_command) || (0x13 == use_command))
  {
    byte_to_send = 5;
    msg[0] = 0x3E;
    msg[1] = address;
    msg[2] = use_command;
    msg[3] = resp_stat;  // Send response status
    msg[4] = calc_crc(&msg[0], byte_to_send-1); 
    
    if (interface == 0) // RS 232
    {
      USART_SendCharArray(OMNICOMM_RS232,&msg[0], byte_to_send); 
    }
    else // RS 485
    {
      GPIO_SetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
      USART_SendCharArray(OMNICOMM_RS485, &msg[0], byte_to_send);
      GPIO_ResetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
    }          
  }    
}

void omnicomm_send_value(uint8_t interface, uint8_t use_command, ommnicomm_dat_t value)
{
  unsigned char msg[RESPONSE_MSG_MAX_SZ];
  uint8_t byte_to_send = 0;
  
  if ((use_command == 0x06) || (use_command == 0x07))
  {
    byte_to_send = 9;
    msg[0] = 0x3E;
    msg[1] = value.address;
    msg[2] = use_command;
    msg[3] = value.temperature;
    msg[4] = (unsigned char)(value.rel_level & 0xFF);
    msg[5] = (unsigned char)(value.rel_level >> 8);
    msg[6] = (unsigned char)(value.frequency & 0xFF);
    msg[7] = (unsigned char)(value.frequency >> 8);
    msg[8] = calc_crc(&msg[0], byte_to_send-1);
    
    if (interface == 0) // RS 232
    {
      USART_SendCharArray(OMNICOMM_RS232,&msg[0], byte_to_send); 
    }
    else // RS 485
    {
      GPIO_SetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
      USART_SendCharArray(OMNICOMM_RS485, &msg[0], byte_to_send);
      GPIO_ResetBits(OMNICOMM_RS485_TXE_GPIO_PORT, OMNICOMM_RS485_TXE_PIN);
    }   
  }
}

/* Funcrion of checking CRC in data array */
char omnicomm_check_crc(uint8_t * arr, uint16_t len)
{
  if (calc_crc(arr, len-1) == arr[len-1])
  {
    return 's';
  }
  else
  {
    return 'e';
  }
}

/* Подсчет CRC8 массива mas длиной Len */
unsigned char calc_crc( unsigned char *mas, unsigned char Len )
{
  unsigned char i,dat,crc,fb,st_byt;
  st_byt=0; crc=0;
  do{
    dat=mas[st_byt];
    for( i=0; i<8; i++) {  // счетчик битов в байте
      fb = crc ^ dat;
      fb &= 1;
      crc >>= 1;
      dat >>= 1;
      if( fb == 1 ) crc ^= 0x8c; // полином
    }
    st_byt++;
  } while( st_byt < Len ); // счетчик байтов в массиве
  return crc;
}

