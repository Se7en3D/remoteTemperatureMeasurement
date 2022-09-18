/*
 * ds18b20.h
 *
 *  Created on: 12 sie 2022
 *      Author: Daniel
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_
#include "OneWire.h"
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
#define FALSE 0
#define TRUE 1
typedef struct ds18b20 ds18b20;
typedef struct ds18b20Rom ds18b20Rom;
typedef enum ds18b20state ds18b20state;
typedef enum ds18b20resolution ds18b20resolution;
enum ds18b20resolution{
	ds18b20_9bitResolution,
	ds18b20_10bitResolution,
	ds18b20_11bitResolution,
	ds18b20_12bitResolution,
};
struct ds18b20{
	UART_HandleTypeDef *huart;
	GPIO_TypeDef gpio;
	uint16_t bit;
	void(*initialization)(ds18b20 *);
	ds18b20Rom *roms;
	uint8_t connectedDevices;
};
struct ds18b20Rom{
	uint8_t romNO[DS18B20_ROM_SIZE];
	ds18b20Rom *nextRom;
	uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
	ds18b20resolution resolution;
};

ds18b20 *ds18b20CreateClass(UART_HandleTypeDef *huart);
void ds18b20Init(ds18b20* this,
		void(*initialization)(ds18b20 *));
void ds18b20Initialization(ds18b20 *this);
void ds18b20ConfiguringSensors(ds18b20 *this);
static void ds18b20SendData(UART_HandleTypeDef *huart,uint8_t command);
static void ds18b20AddNewRom(ds18b20 *this,uint8_t *rom);
void ds18b20ShowRoms(ds18b20 *this);
uint8_t ds18b20GetCrc(uint8_t *data,uint8_t size);


volatile static uint8_t ds18b20Crc88540Table[256] = {
    0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
  157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
   35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
  190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
   70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
  219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
  101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
  248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
  140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
   17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
  175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
   50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
  202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
   87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
  233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
  116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};

volatile static uint32_t ds18b20ConversionTime[4]={
		94,
		188,
		375,
		750
};
#endif /* INC_DS18B20_H_ */
