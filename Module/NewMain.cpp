/*
 * NewMain.cpp
 *
 *  Created on: 17 wrz 2022
 *      Author: Daniel
 */
/*#include "DS18B20.h"
#include "OneWire.h"
#include "TempSensComms.h"
#include "TempSensor.h"
#include "usart.h"
#include "tim.h"
#include <cstdio>*/

#include <NewMain.h>
	//HAL include
#include "main.h"
#include "usart.h"
#include "tim.h"
	//My include
#include "DS18B20.h"
#include "OneWire.h"

DS18B20 *ds18b20;
volatile bool startFlag=false;

int _write(int file, char *ptr, int len){
	//wiFiClass->sendData((WiFiClass*)wiFiClass,(uint8_t *)ptr,len);

	return len;
}



void newMain(){
	  OneWire *oneWire=new OneWire();
	  oneWire->InterfaceSet(&huart5);
	  ds18b20=new DS18B20();
	  ds18b20->SetCommunication(oneWire);
	  ds18b20->Initialize();
	  ds18b20->StartOfMeasurment();
	  HAL_TIM_Base_Start_IT(&htim10);
	  HAL_TIM_Base_Start_IT(&htim11);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		//WiFiAddDataFromUART(wiFiClass,&huart4RxData);
		//HAL_UART_Receive_IT(&huart4,(uint8_t*) &huart4RxData, sizeof(huart4RxData));
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim==&htim10){
		//wiFiClass->increaseSysTick(wiFiClass);
	}
	if(htim==&htim11){
		//uint8_t data[]="Hello World\r\n";
		//uint32_t sizeData=sizeof(data);
		//wiFiClass->sendData(wiFiClass,&data[0],sizeData);
	}
}
