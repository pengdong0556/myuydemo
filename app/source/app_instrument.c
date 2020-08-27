#include "include.h"

T_pro_frame    g_tRecv_frame_data;
uint8_t        g_uart3_rx_buf[UART3_RX_BUF_LEN];
T_ring_buf     g_uart3_rx;
uint32_t       g_send_cnt = 0;
data_node_t   *g_sending_node = NULL;
data_node_t    empty_node_head;
data_node_t    busy_node_head;
uint8_t        g_uBuf_tmp[4 + PRO_FRARME_DATA_LEN_MAX];
const uint8_t  dp[] = {0,0,1,2,0,1,1,2,2,0,0,0,1,0,0};
air_box        air_box_org[MAX_REG_CNT];
AIR_BOX       *airp;
AIR_BOX        air;
const char     deg[] = {0xa1,0xe3,0};
/*add+cmd+dat[0] .... dat[n]+crc16H+crc16L*/
uint8_t pro_add_data_to_send_list(uint8_t add,uint8_t cmd, uint8_t * data, uint32_t len)
{
  
	
	uint16_t crc16;
	uint8_t tmp[13];
	data_node_t * node = NULL;
	uint32_t tmp_len = 0;
	
	if((len + 4) > NODE_DATA_LEN)
	{
		
		TRACE("pro_add_data_to_send_list len error.\r\n");
		return 1;
	}
  /*????????????*/
	node = list_get_node(&empty_node_head);

	if(node == NULL)
	{
		TRACE("pro_add_data_to_send_list get node empty.\r\n");
		return 1;
	}
	
	/*????*/
	tmp[0] = add;
	tmp[1] = cmd;
	

	memcpy(node->data, tmp, 2);
	
	tmp_len = 2;

	if(len > 0)
	{
		memcpy(node->data + tmp_len, data, len);
		tmp_len += len;
	}

	crc16 = Get_Crc16(node->data, tmp_len);
	
	node->data[tmp_len] = (crc16 >> 8)& 0xff; /*modbus crc crc16H*/
	
	node->data[tmp_len + 1] =  crc16 & 0xff ;/*modbus crc crc16L*/
	
	node->len = tmp_len + 2;
	
	/*????????????????????*/
	list_add_node(&busy_node_head, node);
	
//	TRACE("node->data:");
//	
//	for (index = 0 ; index < node->len ; index++)
//	TRACE(" 0x%x", node->data[index]);
//  
//	TRACE("\r\n");
	
	//TRACE("len = %d\r\n",node->len);
	
	return 0;
}
void send_data_from_list(void)
{
	
	static uint32_t time_cnt = 7;	
	
	
	if(g_sending_node == NULL)
	{
		
		g_sending_node = list_get_node(&busy_node_head);
		if(g_sending_node != NULL)
		{
			
			RS485_Send((uint8_t *)g_sending_node->data ,g_sending_node->len);
			g_send_cnt = 1;
			time_cnt = 0;
		  OSTimeDlyHMSM(0, 0, 0 , 50);
		}
	}
	else
	{
		
		
		if(g_send_cnt > UART_RESEND_TIMES)
		{
			list_add_node(&empty_node_head, g_sending_node);
			g_sending_node = NULL;
			g_send_cnt = 0;
		}
		else
		{
			if(++time_cnt >= UART_RESEND_TIME_INTERVAL)
			{
				time_cnt = 0;
			  if(g_sending_node != NULL)
				{
				
				  RS485_Send((uint8_t *)g_sending_node->data ,g_sending_node->len);
				  g_send_cnt++;
					OSTimeDlyHMSM(0, 0, 0 , 50);

				}
				else
				{
				  g_send_cnt = 0;
				}
			}
		}
	}
}

void get_data_cmd_to_down(uint8_t Src,uint16_t len)
{
  
	uint8_t buf[4] = { 0 };
	
	buf[0] = MOSQ_MSB(Src);
	buf[1] = MOSQ_LSB(Src);
	buf[2] = MOSQ_MSB(len);
	buf[3] = MOSQ_LSB(len);
	
	pro_add_data_to_send_list(SLAVE_ADD,READ_CMD_REQ, buf, 4);
 
}


static void rs485_poll_handle(void)
{
	 
	  if (timer_expired(&rs485_poll))
	  {
				 
			timer_restart(&rs485_poll);
			get_data_cmd_to_down(0x0,0xf);
			TRACE("rs485_poll\r\n");
         
		}
	
	
	
}
AIR_BOX  *air_box_data_init(void)
{
	
	return &air;
	
}

void pro_init(void)
{
	
	
	memset(&g_tRecv_frame_data, 0, sizeof(g_tRecv_frame_data));
	g_tRecv_frame_data.lock = PRO_FRAME_DATA_UNLOCK;
	airp = air_box_data_init();
	
	TRACE(" :%d\r\n",sizeof(AIR_BOX));

}

void rs485_recv_start(void)
{
	
	rb_init(&g_uart3_rx, &g_uart3_rx_buf[0], UART3_RX_BUF_LEN);
	RS485_RECV;
}

void pro_del_buf_data(uint32_t size)
{
	rb_del_data(&g_uart3_rx, size);
}

uint32_t pro_buf_data_len(void)
{
	return rb_buf_len(&g_uart3_rx);
}

void pro_clear_timeout_cnt(void)
{
	//g_recv_timeout = 0;
}



void pro_clear_frame_data_lock(void)
{
	g_tRecv_frame_data.lock = PRO_FRAME_DATA_UNLOCK;
}

void pro_read_data_from_buf_only(uint8_t * buf, uint32_t len)
{
	rb_read_only(&g_uart3_rx, buf, len);
}

void check_resp(uint8_t cmd)
{
	if(g_sending_node != NULL)
	{
		
		//TRACE("cmd: %0x,g_sending_node->data[1] :%x\r\n",cmd,g_sending_node->data[1]);
		
		if(cmd == g_sending_node->data[1])
		{
			list_add_node(&empty_node_head, g_sending_node);
			g_sending_node = NULL;
		}
	}
}


void air_box_data_handler(T_pro_frame * data)
{
	
	
	
	uint16_t i = 0;
	
	Data_Base  data_base;
	
	uint16_t *p = &airp->CO2;
	
 
	
  char str[30] = {0};
  
	
	check_resp(data->cmd);
	
	
	for(i = 0 ; i < (data ->len /2) ; i++ )
	{
	  air_box_org[i]  = data->data[i*2];
		air_box_org[i] <<= 8 ;
		air_box_org[i] |= data->data[i*2 + 1];
	}
	for(i = 0 ; i < (data ->len /2) ; i++ )
	{
		 switch ( dp[i] )
		 {
			 case 0:
				
			 *p = air_box_org[i];
			  
			  if (i != (data ->len /2 -1))
			  p = p + 1;
			 
			 
			 break;
			 
			 
			 case 1:
				 
			 data_base.ft = ( float )air_box_org[i] / 10.0;
			 
			 *p = data_base.ut_16[0];
			 
			 *(p+1) = data_base.ut_16[1];
			 
			 if (i != (data ->len /2 -1))
			 p = p + 2;
			 
			 break;
			 
			 
			 case 2:
				 
			 data_base.ft = ( float )air_box_org[i] / 100.0;
			 
			 *p = data_base.ut_16[0];
			 
			 *(p+1) = data_base.ut_16[1];
			 
			 if (i != (data ->len /2 -1))
			 p = p + 2;
			 
			 break;
			 
			 default:
			 break;
			 
		 }
	}
	sprintf(str,"%3.1f%s",airp->TEMP,deg);
	TRACE("\r\n");
	TRACE("    TVOC     PM2.5     CO2\r\n");
	TRACE("    %3.2f      %d       %d\r\n",airp->VOC,airp->PM25,airp->CO2);
	TRACE("    mg/m3    ug/m3     ppm\r\n");
	TRACE("        %sC    %3.1f%%   \r\n",str,airp->HUM);
	TRACE("     O2       O-       HCHO\r\n");
	TRACE("    %3.1f     %d      %3.2f\r\n",airp->O2,airp->NAI,airp->HCHO);
	TRACE("    ug/m3    p/ml      ppm\r\n");
	TRACE("\r\n");
	
}


void pro_read_frame_from_data_buffer(void)
{
	static uint8_t step = STEP_HEADER;
	static uint8_t len_req = PRO_FRAME_HEADER_TO_LEN_DOMAIN_LEN;
	uint16_t crc16 = 0;
	uint16_t crc16_2 = 0;

	
	
	if (step == STEP_DATA)
	{
	
	}

	
/*
 *
 * modbus : add + cmd + len+ dat[0]...dat[len] +crc16H+crc16L
 *
 *
 */
	
	
	
	while(g_tRecv_frame_data.lock == PRO_FRAME_DATA_UNLOCK)
	{
		
		
		
		if(pro_buf_data_len() < len_req) /*???????*/
		{
		   break;                        /*????*/
		}

		switch(step)
		{
			case STEP_HEADER:
				
			
			pro_read_data_from_buf_only(&g_uBuf_tmp[0], PRO_FRAME_HEADER_TO_LEN_DOMAIN_LEN); 
			if ( SLAVE_ADD  == g_uBuf_tmp[0]  && READ_CMD_RSP == g_uBuf_tmp[1])
			{
					
      
					//TRACE("g_uBuf_tmp[0]: %x,g_uBuf_tmp[1] :%x\r\n ", g_uBuf_tmp[0],g_uBuf_tmp[1]);
					/*? ?????? ??*/
					g_tRecv_frame_data.len = g_uBuf_tmp[2] + PRO_FRAME_HEADER_AND_TRAIL_LEN ;

					if(g_tRecv_frame_data.len <= PRO_FRARME_DATA_LEN_MAX)
					{
						len_req = g_tRecv_frame_data.len ;
						step = STEP_DATA;
					}
					else
					{
						pro_del_buf_data(1);
					}
			}
			else
			{
					pro_del_buf_data(1);
			}
	
			break;
			
			
			case STEP_DATA:
				
			 
			  pro_read_data_from_buf_only(&g_uBuf_tmp[0], len_req );
			
//			  for (i = 0 ; i < len_req ; i++)
//			  TRACE(" 0x%x ", g_uBuf_tmp[i]);
        
			
			  //TRACE("\r\n");
			  //TRACE("len: %d\r\n ", len_req);
			
			  //crc16 = Get_Crc16(node->data, tmp_len);
	
	      //node->data[tmp_len] = (crc16 >> 8)& 0xff; /*modbus crc crc16H*/
	
	      //node->data[tmp_len + 1] =  crc16 & 0xff ;/*modbus crc crc16L*/ 
			
        crc16 = Get_Crc16(&g_uBuf_tmp[0], (len_req - 2));
				crc16_2 =  g_uBuf_tmp[len_req - 2] << 8 | g_uBuf_tmp[len_req - 1];
			              
					 
				if( crc16 == crc16_2) 
				{
						
						g_tRecv_frame_data.cmd = g_uBuf_tmp[1];
						
						
					
						//TRACE("rs485_rec_data_ok\r\n"); 
						
						g_tRecv_frame_data.len = g_uBuf_tmp[2];
						
						/*?????data*/
						memcpy(&g_tRecv_frame_data.data[0], &g_uBuf_tmp[3], g_tRecv_frame_data.len);
						
						
						/*???????*/
						pro_del_buf_data(len_req);
						
						//g_tRecv_frame_data.lock = PRO_FRAME_DATA_LOCK;

            
					
			      air_box_data_handler(&g_tRecv_frame_data);
					
					  //CO2 =  g_tRecv_frame_data.data[0] << 8 |g_tRecv_frame_data.data[1] ;
					  
						
					
						
						//g_tRecv_frame_data.lock = PRO_FRAME_DATA_UNLOCK;
						
						/*??????????*/
						step = STEP_HEADER;
						len_req = PRO_FRAME_HEADER_TO_LEN_DOMAIN_LEN;
						
					
				}
				else
				{
						pro_del_buf_data(1);
						step = STEP_HEADER;
						len_req = PRO_FRAME_HEADER_TO_LEN_DOMAIN_LEN;
				}
			
			break;
				
			default:
				pro_del_buf_data(1);
				step = STEP_HEADER;
				len_req = PRO_FRAME_HEADER_TO_LEN_DOMAIN_LEN;
			break;
		}
	
	  
	}
	
	
}


void Task_Instrument_Run(void* p_arg)
{
	
	pro_init();
	
	rs485_recv_start();
	
	list_init(&empty_node_head, &busy_node_head);
	
	while(1)
	
	{
		
		
		rs485_poll_handle();
		send_data_from_list();
		pro_read_frame_from_data_buffer();
		OSTimeDlyHMSM(0, 0, 0 , 20);
	
	}

}

