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
	char *pData;		///<! The data array.
	size_t siz;			///<! Size of the data array.
	size_t head;		///<! Write index.
	size_t tail;		///<! Read index.
	size_t used;        ///<! Number of used bytes.

}fifo_t;

/**
 * Dfault values.
 */
#define FIFO_INIT                    \
                                     \
        {                            \
            .pData = NULL,           \
            .siz   = 0,              \
            .head  = 0,              \
            .tail  = 0,              \
            .used  = 0,              \
        }

/**
 * Used to initialize the fifo.
 *
 * @param f
 * @param buf
 * @param siz
 */
void fifoInit(fifo_t *f, char *buf, size_t siz);

/**
 *
 * @param f
 * @return
 */
size_t fifoGetSize(fifo_t *f);

/**
 *
 * @param f
 * @return
 */
size_t fifoGetUsed(fifo_t *f);

/**
 *
 * @param f
 * @return
 */
size_t fifoGetFree(fifo_t *f);

/**
 *
 * @param f
 * @param buf
 * @param siz
 * @return
 */
size_t fifoWrite(fifo_t *f, const void *buf, size_t siz);

/**
 *
 * @param f
 * @param c
 * @return
 */
size_t fifoPut(fifo_t * f, const void *c);

/**
 *
 * @param f
 * @param buf
 * @param siz
 * @return
 */
size_t fifoRead(fifo_t *f, void *buf, size_t siz);

/**
 *
 * @param f
 * @param buf
 * @return
 */
size_t fifoGetReadBlock(fifo_t *f, void **buf);

/**
 *
 * @param f
 * @param siz
 */
void fifoFree(fifo_t *f, size_t siz);

#endif /* DEBUG_FIFO_H_ */
