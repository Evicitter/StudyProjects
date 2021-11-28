#include "omni_cfind.h"
#include "main.h"
#include "omnicomm.h"


/* defines */
#define PREA1_COMM1_LEN  4
#define PREA1_COMM2_LEN  5

void omni_cfind_obj_clear(omni_cfind_obj_t * obj)
{
  obj->prea_sel = 0;
  obj->comm_sel = 0;
  obj->byte_cnt = 0;
}

ErrorStatus omni_cfind_slave(omni_cfind_obj_t * obj, unsigned char rx_byte, omni_packet_t * out)
{  
  /* if detect 0x31 preambule */
  if (0x31 == obj->prea_sel) 
  {
    obj->packet[obj->byte_cnt] = rx_byte;
    
    // if (1 == obj->byte_cnt) // no critical part in this byte

    if (2 == obj->byte_cnt) 
    {
      obj->comm_sel = rx_byte;
    }
    
    if (2 < obj->byte_cnt)
    {
      switch(obj->comm_sel)
      {
        case 0x06:
          if ('e' == omnicomm_check_crc(&obj->packet[0], obj->byte_cnt+1))      
          {
            omni_cfind_obj_clear(obj);
            return ERROR;
          }
          else
          {
            out->arr = &obj->packet[0];
            out->len = obj->byte_cnt;
            omni_cfind_obj_clear(obj);
            return SUCCESS;
          }
        break;
      
        case 0x07:
          if ('e' == omnicomm_check_crc(&obj->packet[0], obj->byte_cnt+1))      
          {
            omni_cfind_obj_clear(obj);
            return ERROR;
          }
          else
          {
            out->arr = &obj->packet[0];
            out->len = obj->byte_cnt;
            omni_cfind_obj_clear(obj);
            return SUCCESS;
          }
        break;
      
        case 0x13:    
          if (3 != obj->byte_cnt)
          {
            obj->packet[obj->byte_cnt] = rx_byte;  
            if ('e' == omnicomm_check_crc(&obj->packet[0], obj->byte_cnt+1))      
            {
              omni_cfind_obj_clear(obj);
              return ERROR;
            }
            else
            {
              out->arr = &obj->packet[0];
              out->len = obj->byte_cnt;
              omni_cfind_obj_clear(obj);
              return SUCCESS;
            }
          }
        break;
        
        default:
          omni_cfind_obj_clear(obj); // non-existing command   
        break;     
      }
    }
    
    obj->byte_cnt++; 
    return ERROR; // Packet isn't pad full. Go from task.  
  }  
 
  // High priority symbols
  switch(rx_byte)
  {
    case 0x31: 
      obj->prea_sel = rx_byte;
      obj->packet[0] = rx_byte;  
      obj->byte_cnt = 1;      
    break;
    default:
      obj->prea_sel = 0;
    break;      
  }
  return ERROR;
}



ErrorStatus omni_cfind_master(omni_cfind_obj_t * obj, unsigned char rx_byte, omni_packet_t * out)
{  
  /* if detect 0x3E preambule */
  if (0x3E == obj->prea_sel) 
  {
    obj->packet[obj->byte_cnt] = rx_byte;
    
    // if (1 == obj->byte_cnt) // no critical part in this byte

    if (2 == obj->byte_cnt) 
    {
      obj->comm_sel = rx_byte;
    }
    
    if (2 < obj->byte_cnt) //when command is determinate  
    {
      switch(obj->comm_sel)
      {
        case 0x06:
        
          if (8 == obj->byte_cnt)
          {
            if ('e' == omnicomm_check_crc(&obj->packet[0], obj->byte_cnt+1))      
            {
              omni_cfind_obj_clear(obj);
              return ERROR;
            }
            else
            {
              out->arr = &obj->packet[0];
              out->len = obj->byte_cnt;
              omni_cfind_obj_clear(obj);
              return SUCCESS;
            }
          }
        break;
      
       
        default:
          omni_cfind_obj_clear(obj); // non-existing command   
          return ERROR;
        break;     
      }
    }
    
    obj->byte_cnt++; 
    return ERROR; // Packet isn't pad full. Go from task.  
  }  
 
  // High priority symbols
  switch(rx_byte)
  {
    case 0x3E: 
      obj->prea_sel = rx_byte;
      obj->packet[0] = rx_byte;  
      obj->byte_cnt = 1;      
    break;
    default:
      obj->prea_sel = 0;
    break;      
  }
  return ERROR;
}
