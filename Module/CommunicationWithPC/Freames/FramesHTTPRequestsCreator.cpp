/*
 * FramesHTTPRequestsCreator.cpp
 *
 *  Created on: 8 sty 2023
 *      Author: Daniel
 */

#include <CommunicationWithPC/Freames/FramesHTTPRequestsCreator.h>

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
