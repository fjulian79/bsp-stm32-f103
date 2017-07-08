/*
 * fifo.c
 *
 *  Created on: May 28, 2017
 *      Author: julian
 */

#include <string.h>

#include "mybuildroot/common.h"

#include "fifo.h"

void fifoInit(fifo_t * f, char * buf, size_t size)
{
	f->pData = buf;
	f->siz = size;
	f->head = 0;
    f->tail = 0;
}

size_t fifoGetSize(fifo_t *f)
{
    return f->siz;
}

size_t fifoGetUsed(fifo_t * f)
{
	size_t used = 0;

	if (f->head >= f->tail)
		used = f->head - f->tail;
	else
		used = f->head + (f->siz - f->tail);

	return used;
}

size_t fifoGetFree(fifo_t * f)
{
	return f->siz - fifoGetUsed(f) - 1;
}

size_t fifoWrite(fifo_t * f, const void *buf, size_t siz)
{
	size_t tmp = 0;

	siz = min(siz, fifoGetFree(f));

	if (siz == 0)
		goto out;

	if (f->head + siz > f->siz)
	{
		tmp = f->siz - f->head;
		memcpy(&f->pData[f->head], buf, tmp);
		siz -= tmp;
		buf += tmp;
		f->head = 0;
	}

	memcpy(&f->pData[f->head], buf, siz);
	f->head += siz;

	out:
	return siz + tmp;
}

size_t fifoPut(fifo_t * f, const void *c)
{
    size_t tmp = 0;

    if (fifoGetFree(f) == 0)
        goto out;

    if (f->head + 1 > f->siz)
    {
        f->head = 0;
    }

    f->pData[f->head++] = *((char*)c);
    tmp = 1;

    out:
    return tmp;
}

size_t fifoRead(fifo_t * f, void *buf, size_t siz)
{
	size_t tmp = 0;

	siz = min(siz, fifoGetUsed(f));

	if (siz == 0)
		goto out;

	if (f->tail + siz > f->siz)
	{
		tmp = f->siz - f->tail;
		memcpy(buf, (const void*) &f->pData[f->tail], tmp);
		siz -= tmp;
		buf += tmp;
		f->tail = 0;
	}

	memcpy(buf, (const void*) &f->pData[f->tail], siz);
	f->tail += siz;

	if (f->tail == f->head)
	{
		f->head = 0;
		f->tail = 0;
	}

	out:
	return siz + tmp;
}

size_t fifoGetReadBlock(fifo_t *f, void **buf)
{
	size_t used = fifoGetUsed(f);

	if (used == 0)
		goto out;

	*buf = &f->pData[f->tail];

	if (f->tail + used > f->siz)
	{
		used = f->siz - f->tail;
	}

	out:
	return used;
}

void fifoFree(fifo_t *f, size_t siz)
{
	size_t used = min(siz, fifoGetUsed(f));

	if (used == 0)
		return;

	if ((f->tail + used) >= f->siz)
	{
		used -= f->siz - f->tail;
		f->tail = 0;
	}

	f->tail += used;

	if (f->tail == f->head)
	{
		f->head = 0;
		f->tail = 0;
	}
}
