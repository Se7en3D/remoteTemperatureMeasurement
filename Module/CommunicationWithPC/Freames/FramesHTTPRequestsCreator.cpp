/*
 * FramesHTTPRequestsCreator.cpp
 *
 *  Created on: 8 sty 2023
 *      Author: Daniel
 */

#include <CommunicationWithPC/Freames/FramesHTTPRequestsCreator.h>
#include "GlobalData.h"
#include <string.h>
FramesHTTPRequestsCreator::FramesHTTPRequestsCreator() {
	// TODO Auto-generated constructor stub

}

FramesHTTPRequestsCreator::~FramesHTTPRequestsCreator() {
	// TODO Auto-generated destructor stub
}

std::string FramesHTTPRequestsCreator::creatingTemperaturesTransmissionFrame(int* temperatures,uint32_t tempCount){
	if(temperatures==nullptr || tempCount==0){
		return "";
	}
	std::string request="";
	std::string content="";
		//Dodanie numerID w formacie HEX
	content.append("uniqueDeviceID=");
	char UDIDChar[4*2*3]={0};
	sprintf(&UDIDChar[0],"%4X%4X%4X",stm32UniqueDeviceID.id[2],stm32UniqueDeviceID.id[1],stm32UniqueDeviceID.id[0]);
	//sprintf(&UDIDChar[0],"%4X",stm32UniqueDeviceID.id[2]);
	content.append(UDIDChar);
	content.append("&");
		//Przygotowanie zawartości protokołu POST

	content.append("tempCount=");
	content.append(std::to_string(tempCount));
	content.append("&");
	for(uint32_t i=0;i<tempCount;i++){
		content.append("temperatures");
		content.append(std::to_string(i));
		content.append("=");
		content.append(std::to_string(temperatures[i]));
		if(i!=(tempCount-1)){
			content.append("&");
		}
	}
		//Przygotowanie ramki
	request.append("POST /micro/temp");
	request.append(this->targetUrl);
	request.append(" HTTP/1.1\r\n");
	request.append("Content-Type: application/x-www-form-urlencoded\r\n");
	request.append("Content-Length: ");
	request.append(std::to_string(content.size()));
	request.append("\r\n\r\n");
	request.append(content);

	return request;
}
