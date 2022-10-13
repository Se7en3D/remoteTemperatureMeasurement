/*
 * ComWithPC.h
 *
 *  Created on: 21 wrz 2022
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_COMWITHPC_H_
#define COMMUNICATIONWITHPC_COMWITHPC_H_
#include <stdint.h>
class ComWithPC {
public:
	ComWithPC();
	virtual ~ComWithPC();
	virtual int Initialized();
	virtual void Main();
	virtual int SendData(uint8_t *data,uint32_t size);
	virtual int GetFrameFromBuffer(uint8_t *data);
	virtual void Reset();
	virtual void TimeInterrupt();
	virtual int addData(uint8_t *data,uint32_t size);
};

#endif /* COMMUNICATIONWITHPC_COMWITHPC_H_ */
