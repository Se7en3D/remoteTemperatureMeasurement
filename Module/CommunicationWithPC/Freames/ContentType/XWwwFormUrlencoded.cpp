/*
 * XWwwFormUrlencoded.cpp
 *
 *  Created on: 18 lut 2023
 *      Author: Daniel
 */

#include "XWwwFormUrlencoded.h"
#include <iostream>
#include <string.h>
XWwwFormUrlencoded::XWwwFormUrlencoded(){

}
XWwwFormUrlencoded::~XWwwFormUrlencoded(){

}
int XWwwFormUrlencoded::getIntValue(const char *argument,const char* tag,const char *data, int *value){
	std::string substringData;
	int functionResult=this->findStdString(argument, tag, data, substringData);
	if(functionResult){
		return functionResult;
	}
	for(const char &sign:substringData){
		if(sign<'0' || sign>'9'){
			return -1;
		}
	}
	*value=stoi(substringData);
	return 0;

}
char*  XWwwFormUrlencoded::getCharValue(const char *argument,const char* tag,const char *data){
	std::string substringData;
	int functionResult=this->findStdString(argument, tag, data, substringData);
	if(functionResult){
		return nullptr;
	}
	char *returnData=new char[substringData.size()];
	memset(returnData,0,substringData.size());
	strcpy(returnData, substringData.c_str());
	return returnData;
}
int XWwwFormUrlencoded::findStdString(const char *argument,const char* tag,const char *data, std::string &dataOut){
	size_t firstFind;
	size_t secondFind;
	std::string stringData=std::string(data);
	firstFind=secondFind=std::string::npos;
	firstFind=stringData.find(argument);
	if(firstFind==std::string::npos){
		return -1;
	}
	firstFind+=(strlen(argument));
	secondFind=stringData.find(tag,firstFind);
	size_t sizeToSubstring=std::string::npos;
	if(secondFind!=std::string::npos){
		sizeToSubstring=secondFind-firstFind;
	}
	if(sizeToSubstring<=0){
		return -1;
	}
	dataOut=stringData.substr(firstFind,sizeToSubstring);
	if(dataOut.size()==0){
		return -1;
	}
	return 0;
}

