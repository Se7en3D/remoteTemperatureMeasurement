#include <cstdint>
#include "HttpMessage.h"
#include <iostream>
#include <limits>
HttpMessage::HttpMessage()
{
    this->clearData();
}

void HttpMessage::addData(std::string &data){
	if(data.size()>0){
		this->tempBuffer.append(data);
	}
	int returnValue=1;
	int whileReturn=20;
	while(returnValue && whileReturn>0){
		whileReturn--;
	switch(this->stateOfCompleteData){
		case WaitingForStartLine:
			returnValue=this->findStartLine();

			break;
		case WaitingForEmptyLine:
			returnValue=this->findEmptyLine();
			break;
		case WaitingForLastBodyByte:
			returnValue=this->waitToGetLastByteOfBody();
			break;
		case FrameComplete:
			break;
		default:
			this->stateOfCompleteData=WaitingForStartLine;
		break;
		}
	}
	}

inline bool HttpMessage::isMessageComplete()
{
    return (stateOfCompleteData==FrameComplete)? true:false;
}
int HttpMessage::getFrame(HttpFrame *frame){
	std::string tempData="";
	this->addData(tempData);
	if(isMessageComplete()==false){
		return -1;
	}
		//Ustawienie
	frame->header=this->header;
	frame->body=this->body;
	frame->startLine=this->startLine;
	frame->method=this->method;
	frame->type=this->type;
	frame->answerCode=this->answerCode;
		//Kasowanie
	this->clearData();
	return 0;
}
void HttpMessage::clearData()
{
    this->bodyLength=std::numeric_limits<unsigned long long>::max();
    header.clear();
    body.clear();
    startLine.clear();
    tempBuffer.clear();
    this->method=httpMessageMethod::NONE;
    this->type=httpMessageType::NONE;
    this->stateOfCompleteData=WaitingForStartLine;
    this->answerCode=-1;
}

std::vector<std::string> HttpMessage::stringSplit(const std::string &data,const std::string &del,bool includeEmptyLine){
	std::vector<std::string> splitVector;
	if(data.size()==0){
		return splitVector;
	}
	if(del.size()==0){
		splitVector.push_back(data);
		return splitVector;
	}
	size_t startPosition=0;
	size_t findPosition=0;
	do{
		std::string substrString;
		findPosition=data.find(del, startPosition);
		if(findPosition==std::string::npos){
			substrString=data.substr(startPosition);
		}else{
			int sizeToSplit=0;
			sizeToSplit=findPosition-startPosition;
			substrString=data.substr(startPosition,sizeToSplit);
			startPosition=findPosition+del.size();
		}
		if(substrString.size()>0 || includeEmptyLine){
			splitVector.push_back(substrString);
		}
	}while(findPosition!=std::string::npos);
	return splitVector;
}
int HttpMessage::findStartLine(){
		//Wyszukanie końca linii \r\n
	const char endSigns[]="\r\n";
	const char answerStartLine[]="HTTP";
	size_t endPosition=this->tempBuffer.find(endSigns);
	if(endPosition==std::string::npos){
		return 0;
	}
		//Wyszukanie początku
	const int MethodValueSize=8;
	const std::string MethodValue[MethodValueSize]={
			"GET",
			"POST",
			"HEAD",
			"OPTIONS",
			"PUT",
			"DELETE",
			"CONNECTION",
			"TRACE"
	};

	size_t methodPosition=std::string::npos;
	for(int i=0;i<MethodValueSize;i++){
		methodPosition=this->tempBuffer.find(MethodValue[i].c_str());
		if(methodPosition!=std::string::npos){
			this->method=static_cast<httpMessageMethod>(i+1);
			break;
		}
	}
	if(methodPosition==std::string::npos){
		this->method=httpMessageMethod::NONE;
		methodPosition=this->tempBuffer.find(answerStartLine);
		if(methodPosition==std::string::npos){
			return 0;
		}
		this->type=httpMessageType::ANSWER;
	}else{
		this->type=httpMessageType::REQUEST;
	}
	size_t sizeToStr=endPosition+sizeof(endSigns)/sizeof(endSigns[0])-methodPosition-1;
	if(sizeToStr<=0){
		return 0;
	}
	this->startLine=this->tempBuffer.substr(methodPosition,sizeToStr);
	this->tempBuffer.erase(0,endPosition+sizeof(endSigns)/sizeof(endSigns[0])-1);
	this->stateOfCompleteData=WaitingForEmptyLine;
	if(this->type==httpMessageType::ANSWER){
		if(this->findAnswerCode()){
			this->clearData();
		}
	}
	this->header.clear();
	return 1;
}
int HttpMessage::findEmptyLine(){
	char endSigns[]="\r\n";
	char colonSigns[]=":";
	size_t endPosition=this->tempBuffer.find(endSigns);
	if(endPosition==std::string::npos){
		return 0;
	}
	std::string tempString=this->tempBuffer.substr(0,endPosition+sizeof(endSigns)/sizeof(endSigns[0])-1);
	this->header.push_back(tempString);
	this->tempBuffer.erase(0,endPosition+sizeof(endSigns)/sizeof(endSigns[0])-1);
	if(tempString.size()==2){
		this->findContentLength();
		if(this->bodyLength>0){
			this->stateOfCompleteData=WaitingForLastBodyByte;
		}else{
			this->stateOfCompleteData=FrameComplete;
		}

	}else{
		size_t colonPosition=tempString.find(colonSigns);
		if(colonPosition==std::string::npos){
			this->stateOfCompleteData=WaitingForStartLine;
			this->header.clear();
			return 0;
		}
	}
	return 1;
}
int HttpMessage::waitToGetLastByteOfBody(){
	if(this->bodyLength<=0){
		return 0;
	}
	size_t tempBufferSize=this->tempBuffer.size();
	if(tempBufferSize>=this->bodyLength){
		tempBufferSize=this->bodyLength;
	}
	this->body.append(this->tempBuffer.substr(0,tempBufferSize));
	this->tempBuffer.erase(0,tempBufferSize);
	this->bodyLength-=tempBufferSize;
	return 0;
}
void HttpMessage::findContentLength(){
	char findText[]="Content-Length: ";
	char findText2[]="content-length: ";
	for(std::string &line:this->header){
		size_t findPosition=line.find(findText);
		if(findPosition==std::string::npos){
			findPosition=line.find(findText2);
		}
		if(findPosition!=std::string::npos){
			std::string number=line.substr((findPosition+sizeof(findText)/sizeof(char)-1));
			//size_t numberSize=number.size();
			for(size_t i=0;i<number.size();i++){
				if(number.at(i)<'0' ||number.at(i)>'9'){
					number.erase(i,1);
					i--;
				}
			}
			this->bodyLength=stoi(number);
			break;
		}
	}
}
int  HttpMessage::findAnswerCode(){
	const char spaceChar[]=" ";
	size_t firstFind=std::string::npos;
	size_t secondFind=std::string::npos;
	size_t spaceCharSize=sizeof(spaceChar)/sizeof(spaceChar[0])-1;
	std::string strString;
	size_t strStringSize;
	firstFind=this->startLine.find(spaceChar);
	if(firstFind==std::string::npos){
		return -1;
	}
	secondFind=this->startLine.find(spaceChar,firstFind+spaceCharSize);
	if(secondFind==std::string::npos){
		return -1;
	}
	strString=this->startLine.substr(firstFind+spaceCharSize,secondFind-(firstFind+spaceCharSize));
	strStringSize=strString.size();
	for(size_t i=0;i<strStringSize;i++){
		if(strString.at(i)<'0' || strString.at(i)>'9'){
			strString.erase(i,1);
			i--;
			strStringSize--;
		}
	}
	if(strStringSize==0){
		return -1;
	}
	this->answerCode=stoi(strString);
	return 0;
}
