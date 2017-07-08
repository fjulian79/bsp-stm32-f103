/*
 * fifo.h
 *
 *  Created on: May 28, 2017
 *      Author: julian
 */

#ifndef DEBUG_FIFO_H_
#define DEBUG_FIFO_H_

#include <stdint.h>
#include <stddef.h>

/**
 * FiFo Data structure with all data elements needed.
 */
typedef struct
{
	char *pData;		        ///<! The data array.
	size_t siz;			        ///<! Size of the data array.
	volatile size_t head;		///<! Write index.
	volatile size_t tail;		///<! Read index.

}fifo_t;

/**
 * Default values.
 */
#define FIFO_INIT                    \
                                     \
        {                            \
            .pData = NULL,           \
            .siz   = 0,              \
            .head  = 0,              \
            .tail  = 0,              \
        }

/**
 * Used to initialize the fifo.
 *
 * @param f         The fifo to use.
 * @param buf       The fifo buffer to operate on.
 * @param siz       The size of the provided buffer.
 */
void fifoInit(fifo_t *f, char *buf, size_t siz);

/**
 * To get the size of the fifo during runtime.
 *
 * @param f         The fifo to use.
 *
 * @return The data buffer size in bytes.
 */
size_t fifoGetSize(fifo_t *f);

/**
 * To get the amount of used fifo data buffer space.
 *
 * @param f         The fifo to use.
 *
 * @return The number of used bytes.
 */
size_t fifoGetUsed(fifo_t *f);

/**
 * To get the amount of free fifo data buffer space.
 *
 * @param f         The fifo to use.
 *
 * @return the number of free bytes.
 */
size_t fifoGetFree(fifo_t *f);

/**
 * Used to write a given number of bytes from to the fifo.
 *
 * @param f         The fifo to use.
 * @param buf       The provided data.
 * @param siz       The number of bytes to write.
 *
 * @return The number of written bytes.
 */
size_t fifoWrite(fifo_t *f, const void *buf, size_t siz);

/**
 * Used to put just one bate to the fifo.
 *
 * @param f         The fifo to use.
 * @param c         The byte to write to the fifo.
 *
 * @return The number of bytes written (0/1).
 */
size_t fifoPut(fifo_t * f, const void *c);

/**
 * Used to copy data from the fifo to the provided buffer.
 *
 * @param f         The fifo to use.
 * @param buf       The target buffer to write to.
 * @param siz       Number of bytes to read. Hence that less bytes then
 *                  requested will be read from the fifo if less bytes are
 *                  available.
 *
 * @return          The number of bytes read.
 */
size_t fifoRead(fifo_t *f, void *buf, size_t siz);

/**
 *
 * @param f         The fifo to use.
 * @param buf       A pointer to a pointer to take the address of the fifo data
 *
 * @return          The number of bytes which can be read from the fifo in a
 *                  subsequent way.
 */
size_t fifoGetReadBlock(fifo_t *f, void **buf);

/**
 * Used to free space in the fifo. The function will free not more bytes then
 * provided, but maybe less if less bytes are used.
 *
 * @param f         The fifo to use.
 * @param siz       Number of bytes to free.
 */
void fifoFree(fifo_t *f, size_t siz);

#endif /* DEBUG_FIFO_H_ */
