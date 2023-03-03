/*
 * SendController.cpp
 *
 *  Created on: 2 mar 2023
 *      Author: Daniel
 */

#include <Controller/CommController.h>

CommController::CommController(ComWithPC *comWithPC,FramesCreator *framesCreator,FramesDecoder *framesDecoder,uint32_t maxTempBuffer):tempBufferMaxSize(maxTempBuffer) {
	this->comWithPC=comWithPC;
	this->framesCreator=framesCreator;
	this->framesDecoder=framesDecoder;
	this->time=0;
	if(this->comWithPC==nullptr){
		printf("Konstruktor SendController comWithPC jest równy nullptr\r\n");
	}
	if(this->framesCreator==nullptr){
		printf("Konstruktor SendController framesCreator jest równy nullptr\r\n");
	}
	if(this->framesDecoder==nullptr){
		printf("Konstruktor SendController framesDecoder jest równy nullptr\r\n");
	}
}

CommController::~CommController() {
	if(tempBuffer.size()>0){
		for(TempFrameBuffer *data: this->tempBuffer){
			delete data;
		}
		tempBuffer.clear();
	}
}
void CommController::main(){
	std::string readFrame;
	if(comWithPC->GetResponse(&readFrame)==0){
		DecodedFrame decodedFrame;
		if(framesDecoder->decodeFrame(readFrame, &decodedFrame)==0){
			if(decodedFrame.StatusCode>0 && decodedFrame.StatusCode<300){
				if(this->tempBuffer.size()>0){
					auto tempSendMessage=tempBuffer.front();
					this->tempBuffer.erase(tempBuffer.begin());
					delete tempSendMessage;
					this->time=0;
					printf("Usunieto ramke. Wielkosc bufora=%d \r\n",tempBuffer.size());
				}
			}else{
				printf("Kod statusu=%d\r\n",decodedFrame.StatusCode);
			}
		}else{
			printf("Nie odkodowano dane:\r\n");
			printf("%s\r\n",readFrame.c_str());
		}
		//printf("%s\r\n",readFrame.c_str());
	}
	if(this->time>0){
		return;
	}
	if(this->tempBuffer.size()==0){
		return;
	}
	auto tempSendMessage=tempBuffer.front();
	std::string dataToSend=framesCreator->creatingTemperaturesTransmissionFrame(tempSendMessage->pTempValue, tempSendMessage->size);
	if(this->comWithPC->SendData((uint8_t*)dataToSend.c_str(), dataToSend.size())==0){
		this->time=5000;
		printf("Wyslano dane. Wielkosc bufora=%d ",tempBuffer.size());
	}


}
void CommController::timerInterrupt(uint32_t time){
	if(this->time>0){
		this->time-=time;
	}
}
bool CommController::addTempFrame(int *pTemperatures,uint32_t size){
	if(size==0){
		return false;
	}
	if(tempBuffer.size()>=tempBufferMaxSize){
		return false;
	}
	TempFrameBuffer *tempFrameBuffer=new TempFrameBuffer();
	tempFrameBuffer->pTempValue=new int[size];
	tempFrameBuffer->size=size;
	for(uint32_t i=0;i<size;i++){
		tempFrameBuffer->pTempValue[i]=pTemperatures[i];
	}
	tempBuffer.push_back(tempFrameBuffer);
	return true;
}
