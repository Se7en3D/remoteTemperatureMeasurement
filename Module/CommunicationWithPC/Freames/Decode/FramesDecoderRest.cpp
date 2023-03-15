/*
 * FramesDecoderRest.cpp
 *
 *  Created on: 15 lut 2023
 *      Author: Daniel
 */

#include <CommunicationWithPC/Freames/Decode/FramesDecoderRest.h>
#include "../ContentType/XWwwFormUrlencoded.h"
FramesDecoderRest::FramesDecoderRest() {
	// TODO Auto-generated constructor stub

}

FramesDecoderRest::~FramesDecoderRest() {
	// TODO Auto-generated destructor stub
	if(this->contentTypeDecoder!=nullptr){
		delete this->contentTypeDecoder;
	}
}

int FramesDecoderRest::decodeFrame(std::string &inputFrame,DecodedFrame* decodedFrame){
	this->httpMessage.addData(inputFrame);
	HttpFrame httpFrame;
	if(this->httpMessage.getFrame(&httpFrame)==0){
		if(httpFrame.type==httpMessageType::ANSWER){
			decodedFrame->ErrorValue=0;
			decodedFrame->StatusCode=httpFrame.answerCode;
			decodedFrame->type=getMessageType(&httpFrame);
			switch(decodedFrame->type){
			case decodedFrameType::onlyCode:
				break;
			case decodedFrameType::setTime:
				break;
			default:
				break;
			}
			return 0;
		}

		if(httpFrame.type==httpMessageType::REQUEST){
			return -1;
		}
	}

	return -1;
}
int FramesDecoderRest::checkContentType(HttpFrame *httpFrame){
	const char dataToFind1[]="content-type";
	const char dataToFind2[]="Content-Type";
	const char xWwwFormUrlencoded[]="x-www-form-urlencoded";
	//const char textXml[]="text/xml";
	const std::string *pointerToFindData=nullptr;
	for(std::string &data:httpFrame->header){
		if(data.find(dataToFind1,0)!=std::string::npos){
			pointerToFindData=&data;
			break;
		}
		if(data.find(dataToFind2,0)!=std::string::npos){
			pointerToFindData=&data;
			break;
		}
	}
	if(pointerToFindData->find(xWwwFormUrlencoded,0)!=std::string::npos){
		if(this->contentTypeDecoder!=nullptr){
			delete this->contentTypeDecoder;
		}
		this->contentTypeDecoder=new XWwwFormUrlencoded();
		return 0;
	}

	return -1;

}
decodedFrameType FramesDecoderRest::getMessageType(HttpFrame *httpFrame){
	if(httpFrame->body.size()==0){
		//httpFrame->type=decodedFrameType::onlyCode;
		return decodedFrameType::onlyCode;
	}
	if(this->checkContentType(httpFrame)!=0){
		return decodedFrameType::unkown;
	}
	std::string path;
	if(this->contentTypeDecoder->findStdString("path=", "&", httpFrame->body.c_str(), path)==0){
		if(path.find("/micro/date")!=std::string::npos){
			return decodedFrameType::setTime;
		}
	}
	return decodedFrameType::unkown;
}
int FramesDecoderRest::decodeSetTime(HttpFrame *httpFrame){

	return 0;
}
