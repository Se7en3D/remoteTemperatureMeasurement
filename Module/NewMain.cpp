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
#include "CommunicationWithPC/Freames/FramesCreator.h"
#include "CommunicationWithPC/Freames/FramesHTTPRequestsCreator.h"
#include "CommunicationWithPC/Freames/Decode/FramesDecoder.h"
#include "CommunicationWithPC/Freames/Decode/FramesDecoderRest.h"
#include "GlobalData.h"
#include <stdio.h>
#include <string>

FramesCreator *framesCreator=new FramesHTTPRequestsCreator();
DS18B20 *ds18b20=nullptr;
ComWithPC *com=nullptr;
volatile bool startFlag=false;
volatile uint32_t time=0;
volatile uint8_t huart4RxData;

void newMain(){
		//
	FramesDecoder *framesDecoder=new FramesDecoderRest();
		//Read Unique device ID
	stm32UniqueDeviceID.id[0]=*(STM32F4_SYSCFG::UID31_0);
	stm32UniqueDeviceID.id[1]=*(STM32F4_SYSCFG::UID63_32);
	stm32UniqueDeviceID.id[2]=*(STM32F4_SYSCFG::UID95_64);
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
	  	  //Loop
	bool statusLed=false;
	while(true){
		std::string readFrame;
		if(com->GetResponse(&readFrame)==0){
			DecodedFrame decodedFrame;
			if(framesDecoder->decodeFrame(readFrame, &decodedFrame)==0){
				printf("Kod statusu=%d\r\n",decodedFrame.StatusCode);
			}else{
				printf("Nie odkodowano dane:\r\n");
				printf("%s\r\n",readFrame.c_str());
			}
			//printf("%s\r\n",readFrame.c_str());
		}
		if(time>=10){
			time=0;
			printf("Pomiar\r\n");
			if(statusLed){
				HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
				statusLed=false;
			}else{
				HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
				statusLed=true;
			}
			ds18b20->StartOfMeasurment();
			  //HAL_Delay(1000);
		}
		int temperatures[50]{0};
		int returnValue=ds18b20->GetTempValue(&temperatures[0],50);
		if(returnValue>0){
			std::string tempFrame=framesCreator->creatingTemperaturesTransmissionFrame(temperatures, returnValue);
			//tempFrame.append("\r\n");
			com->SendData((uint8_t*)tempFrame.c_str(), tempFrame.size());
		}
		com->Main();
		if(huart4.RxState==HAL_UART_STATE_READY){
			HAL_UART_Receive_IT(&huart4,(uint8_t*) &huart4RxData, sizeof(huart4RxData));
		}

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart4){
		if(com!=nullptr){
			com->addUartData((uint8_t*)&huart4RxData,sizeof(huart4RxData));
		}

		HAL_UART_Receive_IT(&huart4,(uint8_t*) &huart4RxData, sizeof(huart4RxData));
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
		//1 ms
	if(htim==&htim10){
		//if(ds18b20!=nullptr){
		//	ds18b20->TimeHandler();
		//}
		if(com!=nullptr){
			com->TimeInterrupt();
		}
		if(ds18b20!=nullptr){
			ds18b20->TimeHandler();
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
int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart1,(uint8_t*) ptr, len, 1000);
	//if(com!=nullptr){
	//	com->SendData((uint8_t*)(ptr),len);
	//}
	return len;
}
