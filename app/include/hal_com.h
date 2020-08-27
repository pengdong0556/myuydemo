#ifndef __HAL_COM__H
#define __HAL_COM__H
#include "include.h"
void USART3_Config(uint32_t baud);
void USART1_Config(u32 baud);
void USART_SEND(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len);

#endif //__HAL_COM__H
