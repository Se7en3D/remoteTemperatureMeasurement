/*
 * OneWire.h
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */

#ifndef TEMPERATURESENSORS_TEMPSENSCOMMS_ONEWIRE_H_
#define TEMPERATURESENSORS_TEMPSENSCOMMS_ONEWIRE_H_
#include "stm32f4xx_hal.h"
#include <TempSensComms.h>

class OneWire: public virtual TempSensComms {
private:
	UART_HandleTypeDef *huart;
	void SetBaudrate(uint32_t baudrate);\
	void WriteBit(uint8_t *value);
public:
	OneWire();
	virtual ~OneWire();
	uint32_t Reset() override;
	void Write(uint8_t *data,uint32_t bitSize) override;
	uint8_t Read() override;
	void InterfaceSet(UART_HandleTypeDef *huart);
};

#endif /* TEMPERATURESENSORS_TEMPSENSCOMMS_ONEWIRE_H_ */
