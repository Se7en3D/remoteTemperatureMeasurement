/*
 * WiFi.c
 *
 *  Created on: 25 lip 2022
 *      Author: Daniel
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CircularBuffer.h"
#include "stm32f4xx_hal.h"
#include "Wifi.h"


WiFiClass *wiFiCreateClass(UART_HandleTypeDef *huart){
	WiFiClass *me=malloc(sizeof(WiFiClass));
	WiFiHuart=huart;
	if(me!=NULL){
		me->RxBuffer=CircularBuffer_Create();
		me->TxBuffer=CircularBuffer_Create();
		me->eStateMachine=WiFiStateIdle;
		me->eStateMachineEvent=WiFIEventEntry;
		me->sysTick=0;
		me->commandInBuffor=0;
		setWiFiFunction(me,
				WifiGetState,
				WiFiIncreaseSysTick,
				WiFiSendData);
	}
	return me;
}
void setWiFiFunction(WiFiClass  *this,
		WiFiEvents (*getState)(WiFiClass *),
		void (*increaseSysTick)(WiFiClass *),
		void (*sendData)(WiFiClass *,uint8_t*,uint32_t)
		){
	this->getState=getState;
	this->increaseSysTick=increaseSysTick;
	this->sendData=sendData;
}
void WiFiMainFunction(WiFiClass  *this){
	WiFiStates newStates=WiFiStateFunctionArrayWiFiEvents[this->eStateMachine](this);
	if(newStates!=this->eStateMachine){
		this->eStateMachineEvent=WiFIEventEntry;
		this->eStateMachine=newStates;
	}
}
WiFiStates WiFiStateFunctionIdle(WiFiClass  *this){
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateIdle;
	switch(event){
	case WiFIEventEntry:
		WiFiSendCommand((uint8_t *)&stringWifiCheckConnection[0],sizeof(stringWifiCheckConnection)-(1*sizeof(uint8_t)));
		this->eStateMachineEvent=WiFiEventWaitToResponse;
		break;
	case WiFiEventTimeout:
		this->eStateMachineEvent=WiFIEventEntry;
		break;
	case WiFiEventCommandAnswer:{
		while(this->commandInBuffor>0){
			uint8_t *answer=WiFiGetCommand(this);
			//this->eStateMachineEvent=WiFiEventIdle;
			uint8_t *pointerIfOk=(uint8_t *)strstr(answer,((uint8_t*)("OK")));
			if(pointerIfOk!=NULL){
				state=WiFiStateCheckCIPSTATUS;
			}
			free(answer);
		}
		}
		break;
	default:
		break;
	}
	if(this->sysTick>=100){
		this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	}
	return state;
}
WiFiStates WiFiCheckCHIPSTATUS(WiFiClass *this){
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateCheckCIPSTATUS;
	switch(event){
	case WiFIEventEntry:
		WiFiSendCommand((uint8_t *)&stringWifiCHIPSTATUS[0],sizeof(stringWifiCHIPSTATUS)-(1*sizeof(uint8_t)));
		this->eStateMachineEvent=WiFiEventWaitToResponse;
		break;
	case WiFiEventTimeout:
		this->eStateMachineEvent=WiFIEventEntry;
		break;
	case WiFiEventCommandAnswer:{
		while(this->commandInBuffor>0){
			uint8_t *answer=WiFiGetCommand(this);
			//this->eStateMachineEvent=WiFiEventIdle;
			uint8_t *pointerToChar=(uint8_t *)strstr(answer,((uint8_t*)"STATUS:"));
			if(pointerToChar!=NULL){
				pointerToChar=strstr(answer,((uint8_t*)":"));
				pointerToChar++;
				switch(*pointerToChar){
				case '1':
					state=WiFiStateConnectToRouter;
				case '2':
					state=WiFiStateConnectToTCPServer;
					break;
				case '3':
					state=WiFiStateInitialized;
					break;
				case '4':
					state=WiFiStateConnectToTCPServer;
					break;
				case '5':
					state=WiFiStateSetMode;
					break;
				}
			}

			if(state!=WiFiStateCheckCIPSTATUS){
				do{
					free(answer);
					answer=WiFiGetCommand(this);
					pointerToChar=strstr(answer,((uint8_t*)"OK"));
				}while(pointerToChar==NULL);

			}
			free(answer);
		}
		}
		break;
	default:
		break;
	}
	if(this->sysTick>=100){
		this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	}
	return state;
}
WiFiStates WiFiStateFunctionSetMode(WiFiClass  *this){
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateSetMode;
	switch(event){
	case WiFIEventEntry:
		WiFiSendCommand((uint8_t *)&stringWifiSetMode[0],sizeof(stringWifiSetMode)-(1*sizeof(uint8_t)));
		this->eStateMachineEvent=WiFiEventWaitToResponse;
		break;
	case WiFiEventTimeout:
		this->eStateMachineEvent=WiFIEventEntry;
		break;
	case WiFiEventCommandAnswer:{
		while(this->commandInBuffor>0){
			uint8_t *answer=WiFiGetCommand(this);
			//this->eStateMachineEvent=WiFiEventIdle;
			uint8_t *pointerIfOk=(uint8_t *)strstr(answer,((uint8_t*)"OK"));
			if(pointerIfOk!=NULL){
				state=WiFiStateConnectToRouter;
			}
			free(answer);
		}
		}
		break;
	default:
		break;
	}
	if(this->sysTick>=100){
		this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	}
	return state;
}
WiFiStates WiFiStateFunctionConnectToRouter(WiFiClass *this){
		//TODO dodać stan odpowiadający braku sieci Wi-Fi
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateConnectToRouter;
	switch(event){
	case WiFIEventEntry:
		WiFiSendCommand((uint8_t *)&stringWifiConnectToRouter[0],sizeof(stringWifiConnectToRouter)-(1*sizeof(uint8_t)));
		this->eStateMachineEvent=WiFiEventWaitToResponse;
		break;
	case WiFiEventTimeout:
		this->eStateMachineEvent=WiFIEventEntry;
		break;
	case WiFiEventCommandAnswer:{
		while(this->commandInBuffor>0){
			uint8_t *answer=WiFiGetCommand(this);
			//this->eStateMachineEvent=WiFiEventIdle;
			uint8_t *pointer=(uint8_t *)strstr(answer,((uint8_t*)"OK"));
			//Komenda wykonana poprawnie
			if(pointer!=NULL){
				state=WiFiStateConnectToTCPServer;
			}
			//Zła komenda
			pointer=(uint8_t *)strstr(answer,((uint8_t*)"ERROR"));
			if(pointer!=NULL){
				this->eStateMachineEvent=WiFIEventEntry;
			}
			//Bład z połączeniem
			pointer=(uint8_t *)strstr(answer,((uint8_t*)":"));
			if(pointer!=NULL){
				pointer++;
				WiFiDecodeErrorCode(this, pointer);
			}

			free(answer);
		}
		}
		break;
	default:
		break;
	}
	//if(this->sysTick>=100){
	//	this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	//}
	return state;
}
WiFiStates WiFiStateFunctionConnectToTCPServer(WiFiClass *this){
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateConnectToTCPServer;
	switch(event){
	case WiFIEventEntry:
		WiFiSendCommand((uint8_t *)&stringWifiConntectToServer[0],sizeof(stringWifiConntectToServer)-(1*sizeof(uint8_t)));
		this->eStateMachineEvent=WiFiEventWaitToResponse;
		break;
	case WiFiEventTimeout:
		this->eStateMachineEvent=WiFIEventEntry;
		break;
	case WiFiEventCommandAnswer:{
		while(this->commandInBuffor>0){
			uint8_t *answer=WiFiGetCommand(this);
			//this->eStateMachineEvent=WiFiEventIdle;
			uint8_t *pointer=(uint8_t *)strstr(answer,((uint8_t*)"OK"));
			if(pointer!=NULL){
				state=WiFiStateSetPassthroughMode;
			}
			pointer=(uint8_t *)strstr(answer,((uint8_t*)"ALREADY CONNECTED"));
			if(pointer!=NULL){
				state=WiFiStateInitialized;
			}
			free(answer);
		}
		}
		break;
	default:
		break;
	}
	if(this->sysTick>=1000){
		this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	}
	return state;
}
WiFiStates WiFiStateFunctionSetPassthroughMode(WiFiClass *this){
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateSetPassthroughMode;
	switch(event){
	case WiFIEventEntry:
		WiFiSendCommand((uint8_t *)&stringWifiModePassthrough[0],sizeof(stringWifiModePassthrough)-(1*sizeof(uint8_t)));
		this->eStateMachineEvent=WiFiEventWaitToResponse;
		break;
	case WiFiEventTimeout:
		this->eStateMachineEvent=WiFIEventEntry;
		break;
	case WiFiEventCommandAnswer:{
		while(this->commandInBuffor>0){
			uint8_t *answer=WiFiGetCommand(this);
			//this->eStateMachineEvent=WiFiEventIdle;
			uint8_t *pointerIfOk=(uint8_t *)strstr(answer,((uint8_t*)"OK"));
			if(pointerIfOk!=NULL){
				state=WiFiStateInitialized;
			}
			free(answer);
		}
		}
		break;
	default:
		break;
	}
	if(this->sysTick>=100){
		this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	}
	return state;
}
WiFiStates WiFiStateFunctionInitialized(WiFiClass *this){
	this->sysTick=0;
	WiFiStates state=WiFiStateInitialized;
	uint32_t bufferSize=this->TxBuffer->getSize(this->TxBuffer);
	WiFiEvents event=this->eStateMachineEvent;
	switch(event){
		case WiFiEventCommandAnswer:{
			while(this->commandInBuffor>0){
				uint8_t *answer=WiFiGetCommand(this);
				//this->eStateMachineEvent=WiFiEventIdle;
				uint8_t *pointer=(uint8_t *)strstr(answer,((uint8_t*)"CLOSED"));
				if(pointer!=NULL){
					state=WiFiStateCheckCIPSTATUS;
				}
				pointer=(uint8_t *)strstr(answer,((uint8_t*)"ERROR"));
				if(pointer!=NULL){
					state=WiFiStateCheckCIPSTATUS;
				}
				free(answer);
			}
			}
			if(state!=WiFiStateInitialized){
				return state;
			}
			break;
		default:
			break;
		}
	if(bufferSize>0){
		state=WiFiStateStartSendMode;
		uint8_t n;
		this->txDataSize=bufferSize;
		n=sprintf(&sprintfBuffer[0],"AT+CIPSEND=%d\r\n",bufferSize);
		WiFiSendCommand(&sprintfBuffer[0],n);

	}
	return state;
}
WiFiStates WiFiStateFunctionStartSendMode(WiFiClass *this){
	WiFiEvents event=this->eStateMachineEvent;
	WiFiStates state=WiFiStateStartSendMode;
	switch(event){
	case WiFIEventEntry:
		break;
	case WiFiEventTimeout:
		state=WiFiStateInitialized;
		break;
	case WiFiEventSendModeMarker:{
		uint8_t * data=this->TxBuffer->getData(this->TxBuffer);
		WiFiSendCommand(data,this->txDataSize);
		for(int i=1;i<this->txDataSize;i++){
			this->TxBuffer->getData(this->TxBuffer);
		}
		state=WiFiStateInitialized;
		break;
	}
	default:
		break;
	}
	if(this->sysTick>=100){
		this->eStateMachineEvent=WiFiEventTimeout;
		this->sysTick=0;
	}
	return state;
	return WiFiStateStartSendMode;
}
WiFiStates WiFiStateFunctionStopSendMode(WiFiClass *this){
	return WiFiStateStopSendMode;
}

inline WiFiEvents WifiGetState(WiFiClass  *this){
	return this->eStateMachineEvent;
}
void WiFiSendData(WiFiClass *this,uint8_t* data,uint32_t size){
	for(int i=0;i<size;i++){
		this->TxBuffer->insert(this->TxBuffer,data[i]);
	}
}
uint8_t* WiFiGetCommand(WiFiClass *this){
	if(this->commandInBuffor==0){
		return NULL;
	}

	uint8_t tempBuffer[100];
	int number=0;
	uint8_t *pToNextData=this->RxBuffer->getData(this->RxBuffer);
	while(pToNextData!=NULL && *pToNextData!='\n'){
		tempBuffer[number]=*pToNextData;
		pToNextData=this->RxBuffer->getData(this->RxBuffer);
		number++;
	}
	this->commandInBuffor--;

	if(number==0){
		return NULL;
	}

	uint8_t *answer=malloc(sizeof(uint8_t)*(number+1));
	answer[number+1]=0;
	memcpy ( answer, &tempBuffer[0],number );
	return answer;
}
void WiFiSendCommand(const volatile uint8_t *pData, uint16_t Size){
	if(WiFiHuart!=NULL){
		HAL_UART_Transmit_IT((UART_HandleTypeDef *)WiFiHuart, pData, Size);
	}
}

void WiFiAddDataFromUART(WiFiClass *this, const volatile uint8_t *pData){
	if(*pData=='\n'){
		this->eStateMachineEvent=WiFiEventCommandAnswer;
		this->commandInBuffor++;
	}
	//if(this->eStateMachine==WiFiStateInitialized){
		if(*pData=='>'){
			this->eStateMachineEvent=WiFiEventSendModeMarker;
		}
	//}
	this->RxBuffer->insert(this->RxBuffer,*pData);
}
void WiFiIncreaseSysTick(WiFiClass *this){
	this->sysTick++;
}
void WiFiClearCommandBuffor(WiFiClass *this){
	while(this->commandInBuffor){
		uint8_t *answer=WiFiGetCommand(this);
		if(answer==NULL){
			this->commandInBuffor=0;
		}
		this->commandInBuffor--;
		free(answer);
	}
}
void WiFiDecodeErrorCode(WiFiClass *this,uint8_t *code){
	switch(this->eStateMachine){
	case WiFiStateConnectToRouter:
		switch(*code){
		case '1':
			this->errorCode=WifiCWJAPConnectionTimeout;
			break;
		case '2':
			this->errorCode=WifiCWJAPWrongPassword;
			break;
		case '3':
			this->errorCode=WifiCWJAPCannotFindTheTargetAP;
			break;
		case '4':
			this->errorCode=wifiCWJAPConnectionFailed;
			break;
		}
		break;
	}
}
uint8_t WiFiIsInicialized(WiFiClass *this){
	if(this->eStateMachine==WiFiStateInitialized){
		return 0;
	}else{
		return 1;
	}
}
