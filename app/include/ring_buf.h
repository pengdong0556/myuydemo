#ifndef __RING_BUF_H__
#define __RING_BUF_H__

#include   "stm32f10x.h"
#include   "stm32f10x_conf.h"

#define NODE_DATA_LEN    100
#define DATA_NODE_NUM    5

typedef struct{
	uint32_t w_ptr;
	uint32_t r_ptr;
	uint32_t buf_len;
	uint8_t *buf;
}T_ring_buf;

typedef enum{
	RB_OK,
	RB_ERR
}E_err;

typedef struct data_node{
	struct data_node * next;
	uint32_t len;
	uint8_t index;
	uint8_t * data;
}data_node_t;

E_err rb_init(T_ring_buf * buf, uint8_t * data, uint32_t len);
uint32_t rb_buf_is_full(T_ring_buf * buf);
uint32_t rb_buf_len(T_ring_buf * buf);
uint32_t rb_write(T_ring_buf * buf, uint8_t * data, uint32_t len);
uint32_t rb_read(T_ring_buf * buf, uint8_t * data, uint32_t len);
uint32_t rb_read_only(T_ring_buf * buf, uint8_t * data, uint32_t len);
uint32_t rb_del_data(T_ring_buf * buf, uint32_t len);

void list_init(data_node_t * empty_head, data_node_t * busy_head);
data_node_t * list_get_node(data_node_t * head);
int32_t list_add_node(data_node_t * head, data_node_t * node);
void add_data_to_list(uint8_t * data, uint32_t len);

#endif
