#include "include.h"



void Rs485_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//485_DIR
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
}


void RS485_Send(uint8_t *pData, uint8_t len)
{
	RS485_SEND;
	
	USART_SEND(USART3, pData, len);
	
	RS485_RECV;
}




