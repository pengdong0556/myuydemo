/****************************************************************************
* Copyright (C), 2017 GPS Tracker项目
*
* 文件名: ringbuf.c
* 内容简述:环形队列
*       
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2017-05-09 Jason  创建该文件
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

    /* 对读写指针清零*/
    rb->read_index = rb->write_index = 0;

    /* 环形buffer的内存数据块 */
    rb->buffer_ptr = pool;
    rb->buffer_size = size;
}

/* 向环形buffer中写入数据 */
uint32_t rb_put(struct rb* rb, const uint8_t *ptr, uint16_t length)
{
    uint32_t size;

    /* 判断是否有足够的剩余空间 */
    if (rb->read_index > rb->write_index)
        size = rb->read_index - rb->write_index;
    else
        size = rb->buffer_size - rb->write_index + rb->read_index;

    /* 没有多余的空间 */
    if (size == 0) return 0;

    /* 数据不够放置完整的数据，截断放入 */
    if (size < length) length = size;

    if (rb->read_index > rb->write_index)
    {
        /* read_index - write_index 即为总的空余空间 */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        rb->write_index += length;
    }
    else
    {
        if (rb->buffer_size - rb->write_index > length)
        {
            /* write_index 后面剩余的空间有足够的长度 */
            memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
            rb->write_index += length;
        }
        else
        {
            /*
             * write_index 后面剩余的空间不存在足够的长度，需要把部分数据复制到
             * 前面的剩余空间中
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

/* 向环形buffer中写入一个字符 */
uint32_t rb_putchar(struct rb* rb, const uint8_t ch)
{
    uint16_t next;

    /* 判断是否有多余的空间 */
    next = rb->write_index + 1;
    if (next >= rb->buffer_size) next = 0;

    if (next == rb->read_index) return 0;

    /* 放入字符 */
    rb->buffer_ptr[rb->write_index] = ch;
    rb->write_index = next;

    return 1;
}

/* 从环形buffer中读出数据 */
uint32_t rb_get(struct rb* rb, uint8_t *ptr, uint16_t length)
{
    uint32_t size;

    /* 判断是否有足够的数据 */
    if (rb->read_index > rb->write_index)
        size = rb->buffer_size - rb->read_index + rb->write_index;
    else
        size = rb->write_index - rb->read_index;

    /* 没有足够的数据 */
    if (size == 0) return 0;

    /* 数据不够指定的长度，取环形buffer中实际的长度 */
    if (size < length) length = size;

    if (rb->read_index > rb->write_index)
    {
        if (rb->buffer_size - rb->read_index > length)
        {
            /* read_index的数据足够多，直接复制 */
            memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
            rb->read_index += length;
        }
        else
        {
            /* read_index的数据不够，需要分段复制 */
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
         * read_index要比write_index小，总的数据量够（前面已经有总数据量的判
         * 断），直接复制出数据。
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

    /* 返回ringbuffer中存在的数据大小 */
    return size;
}

void rb_reset(struct rb* rb)
{
	if (rb == NULL) return;

    /* 对读写指针清零*/
    rb->read_index = rb->write_index = 0;

    /* 环形buffer的内存数据块 */
	memset(rb->buffer_ptr, 0x00, rb->buffer_size);
}



