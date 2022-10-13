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
#include "CommunicationWithPC/ComWithPC.h"
#include "CommunicationWithPC/WifiESP8266ATCom.h"

DS18B20 *ds18b20=nullptr;
ComWithPC *com=nullptr;
volatile bool startFlag=false;
volatile uint32_t time=0;
volatile uint8_t huart4RxData;
int _write(int file, char *ptr, int len){
	//wiFiClass->sendData((WiFiClass*)wiFiClass,(uint8_t *)ptr,len);

	return len;
}



void newMain(){
		//Setup
	  OneWire *oneWire=new OneWire();
	  oneWire->InterfaceSet(&huart5);
	  ds18b20=new DS18B20();
	  ds18b20->SetCommunication(oneWire);
	  ds18b20->Initialize();
	  ds18b20->StartOfMeasurment();
	  HAL_TIM_Base_Start_IT(&htim10);
	  HAL_TIM_Base_Start_IT(&htim11);
	  com=new WifiESP8266ATCom(&huart4,WIFIESP8266RESET_GPIO_Port,WIFIESP8266RESET_Pin);
	  HAL_UART_Receive_IT(&huart4,(uint8_t*) &huart4RxData, sizeof(huart4RxData));
	  com->Initialized();
	  	  //Loop
	  while(true){
		  if(time>=10){
			  time=0;
			  ds18b20->StartOfMeasurment();
		  }
		  int value[50];
		  ds18b20->GetTempValue(&value[50],50);
		  com->Main();
		  if(huart4.RxState==HAL_UART_STATE_READY){
			  HAL_UART_Receive_IT(&huart4,(uint8_t*) &huart4RxData, sizeof(huart4RxData));
		  }
	  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		if(com!=nullptr){
			com->addData((uint8_t*)&huart4RxData,1);
		}
		//WiFiAddDataFromUART(wiFiClass,&huart4RxData);
		HAL_UART_Receive_IT(&huart4,(uint8_t*) &huart4RxData, sizeof(huart4RxData));
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
		//1 ms
	if(htim==&htim10){
		if(ds18b20!=nullptr){
			ds18b20->TimeHandler();
		}
		if(com!=nullptr){
			com->TimeInterrupt();
		}
		//wiFiClass->increaseSysTick(wiFiClass);
	}
		//1 sekunda
	if(htim==&htim11){
		time++;
		//uint8_t data[]="Hello World\r\n";
		//uint32_t sizeData=sizeof(data);
		//wiFiClass->sendData(wiFiClass,&data[0],sizeData);
	}
}
