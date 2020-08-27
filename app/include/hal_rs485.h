
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#ifndef  __HAL_RS485_H__
#define  __HAL_RS485_H__

#include "include.h"

/*RS485_DIR(GPIOA -> H), Q4截止 ,RE -> H,DE->H,为数据发送状态*/
#define RS485_SEND GPIO_ResetBits(GPIOA, GPIO_Pin_1);

/*RS485_DIR(GPIOA -> L), Q4导通 ,RE -> L,DE->L,为数据接收状态*/
#define RS485_RECV GPIO_SetBits(GPIOA, GPIO_Pin_1);

void Rs485_Configuration(void);
void RS485_Send(uint8_t *pData, uint8_t len);

#endif
