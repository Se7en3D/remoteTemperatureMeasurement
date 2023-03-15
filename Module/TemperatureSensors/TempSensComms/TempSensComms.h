/*
 * TempSensComms.h
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */

#ifndef TEMPERATURESENSORS_TEMPSENSCOMMS_TEMPSENSCOMMS_H_
#define TEMPERATURESENSORS_TEMPSENSCOMMS_TEMPSENSCOMMS_H_
#include <stdint.h>
class TempSensComms {
public:
	TempSensComms();
	virtual ~TempSensComms();
	virtual uint32_t Reset();
	virtual void Write(uint8_t *data,uint32_t bitSize);
	virtual uint8_t Read();
};

#endif /* TEMPERATURESENSORS_TEMPSENSCOMMS_TEMPSENSCOMMS_H_ */
