/*
 * DS18B20.cpp
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */
#include <stdio.h>
#include <DS18B20.h>
#include <math.h>
#include <cstring>
static constexpr uint8_t CRCTable[256] = {
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
static constexpr uint32_t ds18b20ConversionTime[4]={
		94,
		188,
		375,
		750
};
DS18B20::DS18B20() {
	// TODO Auto-generated constructor stub
	this->tempSensComms=nullptr;
}

DS18B20::~DS18B20() {
	// TODO Auto-generated destructor stub
}
void DS18B20::readScratchpad(ds18b20Config *config){
		uint8_t bit=0;
		uint8_t bitToSave,array;
		uint8_t dataToSend=DS18B20_MATCH_ROM;
		//Reset układu
		this->tempSensComms->Reset();
		//Match ROM
		this->tempSensComms->Write(&dataToSend, 8);
		//Wysłanie 8-bajtowego kodu ROM
		this->tempSensComms->Write(config->romNO,DS18B20_ROM_SIZE*8);
		//read scratchpad
		dataToSend=DS18B20_READ_SCRATCHPAD;
		this->tempSensComms->Write(&dataToSend, 8);
		//Odczytanie pamięci scratchpad
		for(int i=0;i<(DS18B20_SCRATCHPAD_SIZE*8);i++){
			bit=this->tempSensComms->Read();
			bitToSave=i%8;
			array=i/8;
			config->scratchpad[array]&=~(1<<bitToSave);
			if(bit){
				config->scratchpad[array]|=1<<bitToSave;
			}
		}
		//Sprawdzenie CRC
		uint8_t crc=this->CalcCRC(config->scratchpad, DS18B20_SCRATCHPAD_SIZE-1);
		if(crc!=config->scratchpad[DS18B20_SCRATCHPAD_SIZE-1]){
			printf("Scratchpad crc is incorrect\n");
		}
		//Skonfigurowanie rozdzielczości przetwornika
		config->resolution=static_cast<ds18b20resolution>((config->scratchpad[5]>>5)&0x03);
}
void DS18B20::decodeTemp(ds18b20Config *config){
	uint32_t sign=0;
	uint32_t exponent=0;
	uint32_t fraction=0;
	uint32_t bits[11];
	uint32_t bitsSize=sizeof(bits)/sizeof(uint32_t);
	for(uint32_t i=0;i<bitsSize;i++){
		bits[i]=config->temperature&(1<<i);
	}

	for(uint32_t i=0;i<bitsSize;i++){
		if(i<4){
			if(bits[i]){
				fraction+=(10000/(pow(2,4-i)));
			}
			continue;
		}
		if(i==(bitsSize-1)){
			if(bits[i]){
				sign=1;
			}
			continue;
		}
		if(bits[i]){
			exponent+=(10000*pow(2,i-4));
		}
	}
	config->decodedTemp=0;
	config->decodedTemp|=(fraction+exponent);
	if(sign){
		config->decodedTemp*=(-1);
	}

}

uint32_t DS18B20::Initialize(){
	if(this->tempSensComms==nullptr){
		return false;
	}
	if(this->configs.size()>0){
		this->configs.clear();
	}
	printf("Initialization DS18b20\r\n");
	uint8_t bit,cmpBit,dataToSend;
	uint8_t ROM[DS18B20_ROM_SIZE]={0};
	uint8_t bitCollisionNO=0;
	do{
		uint8_t bitToSave,array,crc,tempbitCollsionNo=0;
		//Reset układów ds18b20
		if(this->tempSensComms->Reset()){
			return false;
		}
		//Wysłanie komendy SEARCH ROM
		dataToSend=DS18B20_SEARCH_ROM;
		this->tempSensComms->Write(&dataToSend,8);
		for(uint32_t i=0;i<(64);i++){
			bit=this->tempSensComms->Read();
			cmpBit=this->tempSensComms->Read();
			if(bit==1 && cmpBit==1){
				printf("Error while reading ROM on bit %d \n",(int)(i+1));
				break;
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
				uint8_t tempBit=(ROM[array]&(1<<bitToSave));
				tempBit=tempBit>>bitToSave;
				this->tempSensComms->Write(&tempBit,1);
			}else{
				this->tempSensComms->Write(&bit,1);
				ROM[array]&=~(1<<bitToSave);
				if(bit){
					ROM[array]|=1<<bitToSave;
				}
			}
		}
		crc=this->CalcCRC(&ROM[0], DS18B20_ROM_SIZE-1);
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
		bitCollisionNO=tempbitCollsionNo;

			//Dodawanie nowej konfiguracji
		ds18b20Config newConfig;
		memcpy(newConfig.romNO,ROM,DS18B20_ROM_SIZE);
		this->configs.push_back(newConfig);
	}while(bitCollisionNO!=0);
	//ds18b20ShowRoms(this);
	//ds18b20ConfiguringSensors(this);
	for(ds18b20Config &config:this->configs){
		this->readScratchpad(&config);
	}
	return true;
}
void DS18B20::StartOfMeasurment(){
	for(ds18b20Config &config:this->configs){
			uint8_t dataToSend=0;
				//Reset
			this->tempSensComms->Reset();
				//Match ROM
			dataToSend=DS18B20_MATCH_ROM;
			this->tempSensComms->Write(&dataToSend, 8);
			this->tempSensComms->Write(config.romNO, DS18B20_ROM_SIZE*8);
				//Check conversion status
			dataToSend=DS18B20_RECALL_E2;
			this->tempSensComms->Write(&dataToSend, 8);
			if(this->tempSensComms->Read()==0){
				continue;
			}
				//Start conversion
			dataToSend=DS18B20_CONVERT_T;
			this->tempSensComms->Write(&dataToSend, 8);
			this->tempSensComms->Read();
			config.conversion=true;
	}
}
void DS18B20::TimeHandler(){
}
int *DS18B20::GetTempValue(){
	for(ds18b20Config &config:this->configs){
		uint8_t dataToSend=0;
		if(config.conversion==false){
			continue;
		}
			//Reset
		this->tempSensComms->Reset();
			//Match ROM
		dataToSend=DS18B20_MATCH_ROM;
		this->tempSensComms->Write(&dataToSend, 8);
		this->tempSensComms->Write(config.romNO, DS18B20_ROM_SIZE*8);
			//Check converting
		dataToSend=DS18B20_RECALL_E2;
		this->tempSensComms->Write(&dataToSend, 8);

		if(this->tempSensComms->Read()){
			this->readScratchpad(&config);
			config.temperature=(config.scratchpad[1]<<8)+config.scratchpad[0];
			this->decodeTemp(&config);
			config.conversion=false;
			return &config.decodedTemp;
		}
	}
	return nullptr;
}
void DS18B20::SetCommunication(TempSensComms *tempSensComms){
	this->tempSensComms=tempSensComms;
}
void DS18B20::SetResolution(uint8_t resolution){

}
uint8_t DS18B20::CalcCRC(uint8_t *data,uint8_t size){
	unsigned char result=0;

	    while(size--) {
	      result = CRCTable[result ^ *data++];
	    }

	    return result;
}
