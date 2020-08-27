
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


/* ���ζ��г�ʼ�� */
void rb_init(struct rb* rb, uint8_t *pool, uint16_t size);

/* ����buffer��д������ */
uint32_t rb_put(struct rb* rb, const uint8_t *ptr, uint16_t length);

/* ����buffer��д��һ���ַ� */
uint32_t rb_putchar(struct rb* rb, const uint8_t ch);

/* �ӻ���buffer�ж������� */
uint32_t rb_get(struct rb* rb, uint8_t *ptr, uint16_t length);

/* ��黷�ζ�������������*/
uint32_t rb_available(struct rb* rb);

/* ��ջ��ζ��� */
void rb_reset(struct rb* rb);

#endif
