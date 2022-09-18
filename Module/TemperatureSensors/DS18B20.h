/*
 * DS18B20.h
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */

#ifndef TEMPERATURESENSORS_DS18B20_H_
#define TEMPERATURESENSORS_DS18B20_H_
#include <TempSensor.h>
#include <vector>

#define DS18B20_SEARCH_ROM 0xF0
#define DS18B20_READ_ROM 0x33
#define DS18B20_MATCH_ROM 0x55
#define DS18B20_SKIP_ROM 0xCC
#define DS18B20_ALARM_SEARCH 0xEC
#define DS18B20_CONVERT_T 0x44
#define DS18B20_WRITE_SCRATCHPAD 0x4E
#define DS18B20_READ_SCRATCHPAD 0xBE
#define DS18B20_COPY_SCRATCHPAD 0x48
#define DS18B20_RECALL_E2 0xB8
#define DS18B20_READ_POWER_SUPPLY 0xB4

#define DS18B20_SCRATCHPAD_SIZE 9
#define DS18B20_ROM_SIZE 8

class DS18B20: public virtual TempSensor {
private:

	enum ds18b20resolution{
		ds18b20_9bitResolution,
		ds18b20_10bitResolution,
		ds18b20_11bitResolution,
		ds18b20_12bitResolution,
	};

	struct ds18b20Config{
		uint8_t romNO[DS18B20_ROM_SIZE];
		uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
		int temperature;
		int decodedTemp;
		ds18b20resolution resolution;
		uint32_t timer;
		bool conversion;
	};

	TempSensComms *tempSensComms;
	std::vector<ds18b20Config> configs;
	uint8_t CalcCRC(uint8_t *data,uint8_t size);
	void readScratchpad(ds18b20Config *config);
	void decodeTemp(ds18b20Config *config);
public:
	DS18B20();
	virtual ~DS18B20();
	uint32_t Initialize() override;
	void StartOfMeasurment() override;
	void TimeHandler() override;
	int *GetTempValue() override;
	void SetCommunication(TempSensComms *tempSensComms) override;
	void SetResolution(uint8_t resolution) override;
};

#endif /* TEMPERATURESENSORS_DS18B20_H_ */
