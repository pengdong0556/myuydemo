#ifndef  __INSTRUMENT__H
#define  __INSTRUMENT__H

#include "include.h"

/*define*/
#define  UART3_RX_BUF_LEN                      512	
#define  UART_RESEND_TIMES                     10
#define  UART_RESEND_TIME_INTERVAL             25
#define  PRO_FRARME_DATA_LEN_MAX               200
#define  PRO_FRAME_DATA_LOCK                   1
#define  PRO_FRAME_DATA_UNLOCK                 0
#define  PRO_FRAME_HEADER_TO_LEN_DOMAIN_LEN    2
#define  PRO_FRAME_HEADER_AND_TRAIL_LEN        5


#define  SLAVE_ADD         (uint8_t)(0x01)
#define  READ_CMD_REQ      (uint8_t)(0x03)
#define  READ_CMD_RSP      (uint8_t)(0x03)
#define  MAX_REG_CNT       (15)
#define  MOSQ_MSB(A)       (uint8_t)((A & 0xFF00) >> 8)
#define  MOSQ_LSB(A)       (uint8_t)(A & 0x00FF)


typedef  uint16_t          air_box;

typedef struct{
	
	uint8_t opt;
	uint8_t seq;
	uint8_t ack;
	uint16_t reserved;
	uint16_t cmd;
	uint16_t len;
	uint16_t lock;
	uint8_t data[PRO_FRARME_DATA_LEN_MAX];
}T_pro_frame;

#pragma pack(1) 
typedef struct{
	
  uint16_t   CO2; 
	uint16_t   PM25;
  float      O2;
	float      VOC;
  uint16_t   SMOKE;
	float      TEMP;
	float      HUM;
	float      PRESS;
	float      HCHO;
	uint16_t   CO;
	uint16_t   LIGHT;
	uint16_t   CH4;
	float      NOISE;
	uint16_t   PM10;
	uint16_t   NAI;

}AIR_BOX;


typedef union
{
	
	uint16_t  ut_16[2];
	float     ft;

}Data_Base;



typedef enum{
	STEP_HEADER = 0,
	STEP_DATA
}E_parse_step;





extern T_ring_buf g_uart3_rx;


void Task_Instrument_Run(void* p_arg);



#endif //__INSTRUMENT__H

