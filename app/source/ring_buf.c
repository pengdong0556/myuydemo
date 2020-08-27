#include "include.h"

#define DATA_BUF_LEN     500  //NODE_DATA_LEN * DATA_NODE_NUM

uint8_t g_data_buf[DATA_BUF_LEN];
data_node_t data_node[DATA_NODE_NUM];


/*����buffer*/
//typedef struct{
//	uint32_t w_ptr;
//	uint32_t r_ptr;
//	uint32_t buf_len;
//	uint8_t *buf;
//}T_ring_buf;

//T_ring_buf g_uart5_rx;
//uint8_t g_uart5_rx_buf[UART5_RX_BUF_LEN];
//#define  UART5_RX_BUF_LEN     512
//rb_init(&g_uart5_rx, &g_uart5_rx_buf[0], UART5_RX_BUF_LEN);

/*����buffer �е�дָ���ַ-��ָ���ַ,��ʾʣ�¶�������δ��,����buffer��ͷβ���ӵ�ѭ���ṹ,�������ɶ�ָ��ʼ����׷��дָ��*/
E_err rb_init(T_ring_buf * buf, uint8_t * data, uint32_t len)
{
	if(data == NULL)
	{
		return RB_ERR;
	}
	
	buf->w_ptr = 0;
	buf->r_ptr = 0;
	buf->buf_len = len; //512
	buf->buf = data;  //g_uart5_rx_buf
	
	return RB_OK;
}

uint32_t rb_buf_is_full(T_ring_buf * buf)
{
	
	/*����buffer��ָ���дָ���غ�*/
	if(((buf->w_ptr + 1) % buf->buf_len) == buf->r_ptr)
	{
		return 1; //����buffer����
	}
	else
	{
		return 0;
	}
}

uint32_t rb_buf_len(T_ring_buf * buf)
{
	if(buf->w_ptr >= buf->r_ptr)
	{
		return buf->w_ptr - buf->r_ptr;   
	}
	else
	{
		return buf->w_ptr + buf->buf_len - buf->r_ptr;
	}
}

uint32_t rb_write(T_ring_buf * buf, uint8_t * data, uint32_t len)
{
	uint32_t i;

	if(data == NULL)
	{
		return 0;
	}
	
	if(buf->w_ptr >= buf->buf_len)
	{
		//print("%s: buffer error\r\n", __function__);
		buf->w_ptr = 0;
	}

	if(rb_buf_is_full(buf))
	{
		return 0;
	}
	
	for(i = 0; i < len; i++)
	{
		if(((buf->w_ptr + 1) % buf->buf_len) != buf->r_ptr) /*����bufferδд��*/
		{
			buf->buf[buf->w_ptr] = data[i]; /*д�뻷��buffer*/
			buf->w_ptr = (buf->w_ptr + 1) % buf->buf_len; /*дָ���ۼ�һ��*/
		}
		else
		{
			return i; /*����bufferд��*/
		}
	}
	
	return i;
}

uint32_t rb_read(T_ring_buf * buf, uint8_t * data, uint32_t len)
{
	uint32_t i;
	
	if(rb_buf_len(buf) >= len)
	{
		for(i = 0; i < len; i++)
		{
			data[i] = buf->buf[buf->r_ptr]; /*������buffer*/
			buf->r_ptr = (buf->r_ptr + 1) % buf->buf_len;/*��ָ���ۼ�һ��*/
		}
	}
	else
	{
		return 0;
	}
}

uint32_t rb_read_only(T_ring_buf * buf, uint8_t * data, uint32_t len)
{
	uint32_t i;
	uint32_t r_tmp;
	
	if(rb_buf_len(buf) >= len) /*˵������buffer������ ,�ӻ���buffer������,ֻ��ȡ,��δ�ı��ָ���λ��*/
	{
		r_tmp = buf->r_ptr;
		
		for(i = 0; i < len; i++)
		{
			data[i] = buf->buf[r_tmp];
			r_tmp = (r_tmp + 1) % buf->buf_len;
		}
		
		return i;
	}
	else
	{
		return 0;
	}
}

uint32_t rb_del_data(T_ring_buf * buf, uint32_t len)
{
	uint32_t i;
	
	if(rb_buf_len(buf) >= len)
	{
		for(i = 0; i < len; i++)
		{
			buf->r_ptr = (buf->r_ptr + 1) % buf->buf_len;/*��ָ���ۼ�һ��  ,��������*/
		}
	}
	else
	{
		return 0;
	}
}

void list_init(data_node_t * empty_head, data_node_t * busy_head)
{
	uint8_t i;
	data_node_t * p_node;

	if((empty_head == NULL) || (busy_head == NULL))
	{
		TRACE("list init fail.\r\n");
		return;
	}

	busy_head->next = NULL;
	p_node = empty_head;
	
	 /*next -> next1 -> next2 .... ->nextn ->null*/
	/*head -> node1 ->node2....->noden->null*/
	/*���ӳɵ�������*/
	for(i = 0; i < DATA_NODE_NUM; i++)
	{
		
		data_node[i].data = &g_data_buf[i * NODE_DATA_LEN];
		data_node[i].index = i;
		p_node->next = &data_node[i];
		p_node = &data_node[i];
		p_node->next = NULL;
	}

}

data_node_t * list_get_node(data_node_t * head)
{
	data_node_t * p_node;
	
	
	/*ȡ����*/
	/*next ->null*/
	/*head ->null*/
	if(head->next == NULL)
	{
		return NULL;
	}
	 
	
	
	/*�ӵ���������ȡ��һ���ڵ�,ȡ֮ǰ�Ľṹ*/
  /*next -> next1 -> next2 .... ->nestn ->null*/
	/*head -> node1 ->node2....->noden->null*/
	
	
	p_node = (data_node_t *)head->next;
	
	/*ָ����һ���ڵ�,����ȡ��������, ȡ֮��Ľṹ,����ȡ���ڵ�1*/
	
	
	/*next -> next2 -> next3 .... ->nestn ->null*/
	/*head -> node2....->noden->null*/
	head->next = p_node->next;
	
	return p_node;
}
                                /*busy_node*/
int32_t list_add_node(data_node_t * head, data_node_t * node)
{
	uint8_t i = 0;
	
	data_node_t * p_node;
	
	p_node = head;
	
	/*next -> next1 -> next2->null*/
	/*head -> node1 ->node2->null*/
	
	/*�Ѵ����node�ڵ�,�����Ӧ��������.������������(һ��æ����,һ����������),��������ڵ�3*/
	
	
	
	for(i = 0; i < DATA_NODE_NUM; i++)
	{
		
		/*��ͷ��ʼ����������,�ҵ����һ���ڵ���*/
		if(p_node->next == NULL)
		{
			/*next -> next1 -> next2 ->next3->null*/
	    /*head -> node1 ->node2 ->node3->null*/
	
			p_node->next = node;
			node->next = NULL;
			return 1;
		}
		
		
		else
		{
			p_node = p_node->next;
		}
	}
	
	return 0;
}

/*
void add_data_to_list(uint8_t * data, uint32_t len)
{
	data_node_t * node = NULL;
	
	if(len > NODE_DATA_LEN)
	{
		return;
	}
	
	node = list_get_node((data_node_t *)&empty_node_head);
	
	if(node != NULL)
	{
		memcpy(node->data, data, len);
		node->len = len;
		list_add_node(&busy_node_head, node);
	}
}
*/




