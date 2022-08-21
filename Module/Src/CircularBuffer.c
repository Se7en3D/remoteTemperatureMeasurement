#include <stdio.h>
#include <stdlib.h>
#include "CircularBuffer.h"



CircularBufferStruct *CircularBuffer_Create() {
	CircularBufferStruct* me = (CircularBufferStruct*)malloc(sizeof(CircularBufferStruct));
	if (me != NULL) {
		CircularBuffer_Init(me,
			CircularBuffer_isFull,
			CircularBuffer_isEmpty,
			CircularBuffer_getSize,
			CircularBuffer_insert,
			CircularBuffer_getData,
			CircularBuffer_show,
			CircularBuffer_getDataToDMA);
	}
	return me;
}
void CircularBuffer_Init(CircularBufferStruct* const me,
						uint32_t(*isFull)(CircularBufferStruct* const me),
						uint32_t(*isEmpty)(CircularBufferStruct* const me),
						uint32_t(*getSize)(CircularBufferStruct* const me),
						void (*insert)(CircularBufferStruct* const me, circ_t value),
						circ_t*(*getData)(CircularBufferStruct* const me),
						void (*show)(CircularBufferStruct* const me),
						circ_t* (*getDataFromDMA)(CircularBufferStruct* const me)) {
		/*initialize attributes*/

	me->head = 0;
	me->tail = 0;
	me->size = CIRCULARBUFFER_MAX_SIZE_BUFFER;
		/* initialize member function pointers */
	me->isFull = isFull;
	me->isEmpty = isEmpty;
	me->getSize = getSize;
	me->insert = insert;
	me->getData = getData;
	me->show = show;
	me->getDataToDMA=getDataFromDMA;
}
uint32_t CircularBuffer_isFull(CircularBufferStruct* const me) {
	uint32_t next = me->head + 1;
	if (next >= CIRCULARBUFFER_MAX_SIZE_BUFFER) {
		next = 0;
	}
	if (me->tail == next) {
		return 1;
	}
	else {
		return 0;
	}
}
uint32_t CircularBuffer_isEmpty(CircularBufferStruct* const me) {
	if (me->tail == me->head) {
		return 1;
	}
	else {
		return 0;
	}
}
uint32_t CircularBuffer_getSize(CircularBufferStruct* const me) {
	uint32_t totalSize=0;
	uint32_t head=me->head;
	uint32_t tail=me->tail;
	if(head==tail){
		return 0;
	}

	if (head < tail) {
		totalSize = CIRCULARBUFFER_MAX_SIZE_BUFFER - tail;
		//totalSize += head;
	}
	else {
		totalSize = head - tail;
	}
	return totalSize;

}
void CircularBuffer_insert(CircularBufferStruct* const me, circ_t value) {
	if (me->isFull(me)==0) {
		uint32_t next = me->head;
		me->buffer[next] = value;
		me->head = next;
		next++;
		if (next >= CIRCULARBUFFER_MAX_SIZE_BUFFER) {
			next = 0;
		}
		me->head = next;
	}
}
circ_t* CircularBuffer_getData(CircularBufferStruct* const me){
	if (me->isEmpty(me) == 0) {
		circ_t* result = &me->buffer[me->tail];
		me->tail++;
		if (me->tail >= CIRCULARBUFFER_MAX_SIZE_BUFFER) {
			me->tail = 0;
		}
		return result;
	}else {
		return NULL;
	}
}
void CircularBuffer_show(CircularBufferStruct* const me) {
	circ_t* pointerToData;
	while ((pointerToData = me->getData(me))) {
		printf("Dana=%d\n",(int) pointerToData);
	}
}
circ_t* CircularBuffer_getDataToDMA(CircularBufferStruct* const me){
	if (me->isEmpty(me) == 0) {
		uint32_t head=me->head;
		uint32_t tail=me->tail;

		circ_t* result = &me->buffer[tail];
		if (head < tail) {
			me->tail = 0;

		}
		else {
			me->tail = head;
		}
		return result;
	}else {
		return NULL;
	}
}
