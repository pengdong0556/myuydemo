
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#ifndef  COM_RINGBUF
#define  COM_RINGBUF

#include "include.h"
struct rb
{
	uint16_t read_index, write_index;
	uint8_t *buffer_ptr;
	uint16_t buffer_size;
};


/* 环形队列初始化 */
void rb_init(struct rb* rb, uint8_t *pool, uint16_t size);

/* 向环形buffer中写入数据 */
uint32_t rb_put(struct rb* rb, const uint8_t *ptr, uint16_t length);

/* 向环形buffer中写入一个字符 */
uint32_t rb_putchar(struct rb* rb, const uint8_t ch);

/* 从环形buffer中读出数据 */
uint32_t rb_get(struct rb* rb, uint8_t *ptr, uint16_t length);

/* 检查环形队列已有数据量*/
uint32_t rb_available(struct rb* rb);

/* 清空环形队列 */
void rb_reset(struct rb* rb);

#endif
