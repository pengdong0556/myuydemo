#include "include.h"


void USART_ALL_Config(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//PA9,PA10 复用IO口功能用于配置串口。因此要使能AFIO（复用功能IO）时钟。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//usart_init----------------------------------------------------
	/* Configure USARTy Tx (PA.09,PA.02,PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USARTy Rx (PA.10,PA.03,PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;						//速率9600bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	/* Configure USARTy */
	USART_Init(USART1, &USART_InitStructure);							//配置串口参数函数
	USART_Init(USART2, &USART_InitStructure);
	USART_Init(USART3, &USART_InitStructure);
	USART_Init(UART4, &USART_InitStructure);

	/* Enable USARTy Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //使能接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);						//使能发送完成中断

	/* Enable the USARTy */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_Cmd(UART4, ENABLE);

	 /* 清发送完成标志*/
	USART_ClearFlag(USART1, USART_FLAG_TXE);
	USART_ClearFlag(USART2, USART_FLAG_TXE);
	USART_ClearFlag(USART3, USART_FLAG_TXE);
	USART_ClearFlag(UART4, USART_FLAG_TXE);
	
	/*可以解决发不出第一个数据的BUG */
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(UART4, USART_FLAG_TC);
}


void USART1_GPIO_CONFIG(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate =baud;				 //速率115200bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);							//配置串口参数函数

	/* Enable USART1 Receive and Transmit interrupts */
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //使能接收中断
	//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);	

	USART_ClearFlag(USART1, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */

  
}

void USART2_GPIO_CONFIG(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate =baud;				 		//速率9600bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);							//配置串口参数函数

	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                    //使能接收中断
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断

	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);	

	//USART_ClearFlag(USART2, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */

}

void USART3_GPIO_CONFIG(u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART2 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate =baud;				 		//速率9600bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	/* Configure USART3 */
	USART_Init(USART3, &USART_InitStructure);							//配置串口参数函数

	/* Enable USART3 Receive and Transmit interrupts */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                    //使能接收中断
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);				    //使能发送缓冲空中断
  //USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	/* Enable the USART3 */
	USART_Cmd(USART3, ENABLE);

  

	//USART_ClearFlag(USART3, USART_FLAG_TXE);     /* 清发送完成标志，Transmission Complete flag */

}

void UART4_GPIO_CONFIG(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE); 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

	USART_Init(UART4, &USART_InitStructure);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	USART_Cmd(UART4, ENABLE);
}


void USART1_NVIC_CONFIG(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//设置串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_NVIC_CONFIG(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     	//设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART3_NVIC_CONFIG(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			     	//设置串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UART4_NVIC_CONFIG(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART1_Config(u32 baud)
{
	USART1_GPIO_CONFIG(baud);
	//USART1_NVIC_CONFIG();
}

void USART2_Config(u32 baud)
{
	USART2_GPIO_CONFIG(baud);
	USART2_NVIC_CONFIG();
}

void USART3_Config(uint32_t baud)
{
	USART3_GPIO_CONFIG(baud);
	USART3_NVIC_CONFIG();
}

void UART4_Config(u32 baud)
{
	UART4_GPIO_CONFIG(baud);
	UART4_NVIC_CONFIG();
}


int SendChar (int ch)  
{               
	/* Write character to Serial Port     */

	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}
void Usart_Sendbyte(USART_TypeDef* USARTx,uint8_t Data)
{
	uint8_t st;
	
	USARTx -> DR = Data;
	do
	{
		st = USART_GetFlagStatus(USARTx, USART_FLAG_TC);
		
		
	}while(st == RESET);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	
	
	
}

void USART_SEND(USART_TypeDef* USARTx, uint8_t *Data,uint8_t len)
{
//	if (USARTx == USART3) RS485_SEND;
	
//	USART_GetFlagStatus(USARTx, USART_FLAG_TC);
	while(len--)
	{

		Usart_Sendbyte(USARTx,*Data++);
		
	}
	
	
	
	

//	if (USARTx == USART3) RS485_RECV;
}


/******************************************************
		格式化串口输出函数
        "\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
		"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
		"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
		"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
**********************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
{ 
	const char *s;
	int d;

	char buf[16];
	va_list ap;
	va_start(ap, Data);

	USART_GetFlagStatus(USARTx, USART_FLAG_TC);

	while(*Data!=0)
	{				                          //判断是否到达字符串结束符
		if(*Data==0x5c)
		{									  //'\'
			switch (*++Data)
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   
					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				case '"':							          //换行符
					USART_SendData(USARTx, 0x22);			  //引号
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%')
		{									  //
			switch (*++Data)
			{				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            		case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);

	}
}

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int fputc(int ch,FILE *f)
{
	//ch送给USART1
	USART_SendData(USART1, ch);
	//等待发送完毕
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) { }
	//返回ch
	return(ch);
}
