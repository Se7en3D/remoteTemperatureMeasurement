/*
 * TempSensor.h
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */

#ifndef TEMPERATURESENSORS_TEMPSENSOR_H_
#define TEMPERATURESENSORS_TEMPSENSOR_H_
#include "TempSensComms.h"
#include <stdint.h>
class TempSensor {
private:
public:
	TempSensor();
	virtual ~TempSensor();
	virtual uint32_t Initialize();
	virtual void StartOfMeasurment();
	virtual void TimeHandler();
	virtual int GetTempValue(int* data,uint32_t size);
	virtual void SetCommunication(TempSensComms *tempSensComms);
	virtual void SetResolution(uint8_t resolution);
};

#endif /* TEMPERATURESENSORS_TEMPSENSOR_H_ */
