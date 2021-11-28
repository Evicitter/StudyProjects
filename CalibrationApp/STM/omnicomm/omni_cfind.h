#ifndef __OMNI_CFIND_H__
#define __OMNI_CFIND_H__
  
#ifdef __cplusplus
  extern "C" {
#endif

  #include "stm32f10x.h"
  #include <stdio.h>
  #include <stdlib.h>    
    
  #define OMNI_CFIND_PBUF_SZ  9

  typedef struct {
    unsigned char prea_sel;
    unsigned char comm_sel;
    unsigned char byte_cnt;
    unsigned char packet[OMNI_CFIND_PBUF_SZ];
  }omni_cfind_obj_t; 
  
  
  typedef struct{
    unsigned char* arr ;
    short          len ;
  }omni_packet_t;    
    
  void omni_cfind_obj_clear(omni_cfind_obj_t * obj);
  ErrorStatus omni_cfind_slave(omni_cfind_obj_t * obj, unsigned char rx_byte, omni_packet_t * out);    
  ErrorStatus omni_cfind_master(omni_cfind_obj_t * obj, unsigned char rx_byte, omni_packet_t * out);
    

#ifdef __cplusplus
}
#endif

#endif //__OMNI_CFIND_H__
