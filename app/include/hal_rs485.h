
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#ifndef  __HAL_RS485_H__
#define  __HAL_RS485_H__

#include "include.h"

/*RS485_DIR(GPIOA -> H), Q4��ֹ ,RE -> H,DE->H,Ϊ���ݷ���״̬*/
#define RS485_SEND GPIO_ResetBits(GPIOA, GPIO_Pin_1);

/*RS485_DIR(GPIOA -> L), Q4��ͨ ,RE -> L,DE->L,Ϊ���ݽ���״̬*/
#define RS485_RECV GPIO_SetBits(GPIOA, GPIO_Pin_1);

void Rs485_Configuration(void);
void RS485_Send(uint8_t *pData, uint8_t len);

#endif
