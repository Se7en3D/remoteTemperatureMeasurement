/*
 * ds18b20.c
 *
 *  Created on: 12 sie 2022
 *      Author: Daniel
 */

#include "stm32f4xx_hal.h"
#include "ds18b20.h"
#include "OneWire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
ds18b20 *ds18b20CreateClass(UART_HandleTypeDef *huart){
	ds18b20 *this=malloc(sizeof(ds18b20));
	if(this!=NULL){
		this->huart=huart;
		this->roms=NULL;
		this->connectedDevices=0;
		ds18b20Init(this,
				ds18b20Initialization);
	}
	return this;
}
void ds18b20Init(ds18b20* this,
		void(*initialization)(ds18b20 *)){
	this->initialization=initialization;
}

void ds18b20Initialization(ds18b20 *this){
	printf("Initialization DS18b20\r\n");
	uint8_t bit,cmpBit;
	uint8_t ROM[DS18B20_ROM_SIZE]={0};
	uint8_t bitCollisionNO=0;
	do{
		uint8_t bitToSave,array,crc,tempbitCollsionNo=0;
		//Reset układów ds18b20
		if(OneWireReset(this->huart)){
			return;
		}
		//Wysłanie komendy SEARCH ROM
		ds18b20SendData(this->huart, 0xF0);
		HAL_Delay(1);
		for(uint32_t i=0;i<(64);i++){
			bit=OneWireReadBit(this->huart);
			cmpBit=OneWireReadBit(this->huart);
			if(bit==1 && cmpBit==1){
				printf("Error while reading ROM on bit %d \n",(int)(i+1));
				return;
			}
			if(bit==0 && cmpBit==0){
				//printf("Wykryto bit 0 i 0 podczas odczytu bitu nr.%d \n",(i+1));
				if(i==bitCollisionNO){
					if(bit){
						bit=0;
					}else{
						bit=1;
					}
				}else{
					if(i>tempbitCollsionNo){
						tempbitCollsionNo=i;
					}
				}
			}
			//Obliczenie akutalnego bitu oraz bajtu do zapisu
			bitToSave=i%8;
			array=i/8;

			//Wysłanie kolejnego bitu na magistralę
			if(i<bitCollisionNO){
				OneWireWriteBit(this->huart, (ROM[array]&(1<<bitToSave)));
			}else{
				OneWireWriteBit(this->huart, bit);
				ROM[array]&=~(1<<bitToSave);
				if(bit){
					ROM[array]|=1<<bitToSave;
				}
			}
		}
		crc=ds18b20GetCrc(ROM, DS18B20_ROM_SIZE-1);
		if(crc!=ROM[DS18B20_ROM_SIZE-1]){
			printf("ROM crc is incorrect\n");
			bitToSave=bitCollisionNO%8;
			array=bitCollisionNO/8;
			if((ROM[array]&1<<bitToSave)>0){
				ROM[array]&=~(1<<bitToSave);
			}else{
				ROM[array]|=(1<<bitToSave);
			}
			continue;
		}
		ds18b20AddNewRom(this,&ROM[0]);
		bitCollisionNO=tempbitCollsionNo;
	}while(bitCollisionNO!=0);
	ds18b20ShowRoms(this);
	ds18b20ConfiguringSensors(this);
}
void ds18b20ConfiguringSensors(ds18b20 *this){
	ds18b20Rom *rom=this->roms;
	int timeout=1500;
	while(rom!=NULL && timeout){
		timeout--;
		uint8_t bit=0;
		uint8_t bitToSave,array;
		//Reset układu
		OneWireReset(this->huart);
		//Match ROM
		ds18b20SendData(this->huart, DS18B20_MATCH_ROM);
		//Wysłanie 8-bajtowego kodu ROM
		for(int i=0;i<8;i++){
			ds18b20SendData(this->huart, rom->romNO[i]);
		}
		//read scratchpad
		ds18b20SendData(this->huart,DS18B20_READ_SCRATCHPAD);
		//Odczytanie pamięci scratchpad
		for(int i=0;i<(DS18B20_SCRATCHPAD_SIZE*8);i++){
			bit=OneWireReadBit(this->huart);
			bitToSave=i%8;
			array=i/8;
			rom->scratchpad[array]&=~(1<<bitToSave);
			if(bit){
				rom->scratchpad[array]|=1<<bitToSave;
			}
		}
		//Sprawdzenie CRC
		uint8_t crc=ds18b20GetCrc(rom->scratchpad, DS18B20_SCRATCHPAD_SIZE-1);
		if(crc!=rom->scratchpad[DS18B20_SCRATCHPAD_SIZE-1]){
			printf("Scratchpad crc is incorrect\n");
			continue;
		}
		//Skonfigurowanie rozdzielczości przetwornika
		rom->resolution=(rom->scratchpad[4]>>5)&0x03;

		rom=rom->nextRom;
	}
}
static void ds18b20SendData(UART_HandleTypeDef *huart,uint8_t command){
	for(int i=0;i<sizeof(uint8_t)*8;i++){
		OneWireWriteBit(huart,command&0x01);
		command=command>>1;
	}
}
static void ds18b20AddNewRom(ds18b20 *this,uint8_t *rom){
	if(this->roms==NULL){
		this->roms=malloc(sizeof(ds18b20Rom));
		memcpy(this->roms,rom,8);
		this->roms->nextRom=NULL;
		return;
	}
	ds18b20Rom *next=this->roms;
	while(next->nextRom!=NULL){
		next=next->nextRom;
	}
	next->nextRom=malloc(sizeof(ds18b20Rom));
	memcpy(next->nextRom,rom,DS18B20_ROM_SIZE);
	next->nextRom->nextRom=NULL;
	this->connectedDevices++;

}
void ds18b20ShowRoms(ds18b20 *this){

	ds18b20Rom *next=this->roms;
	while(next!=NULL){
		if(this->roms==NULL){
			return;
		}
		for(int i=0;i<8;i++){
			printf("%2x ",next->romNO[i]);
		}
		printf("\r\n");
		next=next->nextRom;
	}
}
uint8_t ds18b20GetCrc(uint8_t *data,uint8_t size){
	unsigned char result=0;

	    while(size--) {
	      result = ds18b20Crc88540Table[result ^ *data++];
	    }

	    return result;
}
