/****************************************************************************
* Copyright (C), 2017 GPS Tracker��Ŀ
*
* �ļ���: ringbuf.c
* ���ݼ���:���ζ���
*       
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2017-05-09 Jason  �������ļ�
*
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "ringbuf.h"

void rb_init(struct rb* rb, uint8_t *pool, uint16_t size)
{
//  ASSERT(rb != NULL);
	if (rb == NULL) return;

    /* �Զ�дָ������*/
    rb->read_index = rb->write_index = 0;

    /* ����buffer���ڴ����ݿ� */
    rb->buffer_ptr = pool;
    rb->buffer_size = size;
}

/* ����buffer��д������ */
uint32_t rb_put(struct rb* rb, const uint8_t *ptr, uint16_t length)
{
    uint32_t size;

    /* �ж��Ƿ����㹻��ʣ��ռ� */
    if (rb->read_index > rb->write_index)
        size = rb->read_index - rb->write_index;
    else
        size = rb->buffer_size - rb->write_index + rb->read_index;

    /* û�ж���Ŀռ� */
    if (size == 0) return 0;

    /* ���ݲ����������������ݣ��ضϷ��� */
    if (size < length) length = size;

    if (rb->read_index > rb->write_index)
    {
        /* read_index - write_index ��Ϊ�ܵĿ���ռ� */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        rb->write_index += length;
    }
    else
    {
        if (rb->buffer_size - rb->write_index > length)
        {
            /* write_index ����ʣ��Ŀռ����㹻�ĳ��� */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
            rb->write_index += length;
        }
        else
        {
            /*
             * write_index ����ʣ��Ŀռ䲻�����㹻�ĳ��ȣ���Ҫ�Ѳ������ݸ��Ƶ�
             * ǰ���ʣ��ռ���
             */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr,
                   rb->buffer_size - rb->write_index);
            memcpy(&rb->buffer_ptr[0], &ptr[rb->buffer_size - rb->write_index],
                   length - (rb->buffer_size - rb->write_index));
            rb->write_index = length - (rb->buffer_size - rb->write_index);
        }
    }

    return length;
}

/* ����buffer��д��һ���ַ� */
uint32_t rb_putchar(struct rb* rb, const uint8_t ch)
{
    uint16_t next;

    /* �ж��Ƿ��ж���Ŀռ� */
    next = rb->write_index + 1;
    if (next >= rb->buffer_size) next = 0;

    if (next == rb->read_index) return 0;

    /* �����ַ� */
    rb->buffer_ptr[rb->write_index] = ch;
    rb->write_index = next;

    return 1;
}

/* �ӻ���buffer�ж������� */
uint32_t rb_get(struct rb* rb, uint8_t *ptr, uint16_t length)
{
    uint32_t size;

    /* �ж��Ƿ����㹻������ */
    if (rb->read_index > rb->write_index)
        size = rb->buffer_size - rb->read_index + rb->write_index;
    else
        size = rb->write_index - rb->read_index;

    /* û���㹻������ */
    if (size == 0) return 0;

    /* ���ݲ���ָ���ĳ��ȣ�ȡ����buffer��ʵ�ʵĳ��� */
    if (size < length) length = size;

    if (rb->read_index > rb->write_index)
    {
        if (rb->buffer_size - rb->read_index > length)
        {
            /* read_index�������㹻�ֱ࣬�Ӹ��� */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
            rb->read_index += length;
        }
        else
        {
            /* read_index�����ݲ�������Ҫ�ֶθ��� */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index],
                   rb->buffer_size - rb->read_index);
            memcpy(&ptr[rb->buffer_size - rb->read_index], &rb->buffer_ptr[0],
                   length - rb->buffer_size + rb->read_index);
            rb->read_index = length - rb->buffer_size + rb->read_index;
        }
    }
    else
    {
        /*
         * read_indexҪ��write_indexС���ܵ�����������ǰ���Ѿ���������������
         * �ϣ���ֱ�Ӹ��Ƴ����ݡ�
         */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        rb->read_index += length;
    }

    return length;
}

uint32_t rb_available(struct rb* rb)
{
    uint32_t size;

    if (rb->read_index > rb->write_index)
        size = rb->buffer_size - rb->read_index + rb->write_index;
    else
        size = rb->write_index - rb->read_index;

    /* ����ringbuffer�д��ڵ����ݴ�С */
    return size;
}

void rb_reset(struct rb* rb)
{
	if (rb == NULL) return;

    /* �Զ�дָ������*/
    rb->read_index = rb->write_index = 0;

    /* ����buffer���ڴ����ݿ� */
	memset(rb->buffer_ptr, 0x00, rb->buffer_size);
}



