/*
 * CircularBuffer.h
 *
 *  Created on: 26 lut 2023
 *      Author: Daniel
 */

#ifndef MODELS_CIRCULARBUFFER_H_
#define MODELS_CIRCULARBUFFER_H_
//#include <stdio.h>
#include <iostream>
template <uint32_t size, class type> class CircularBuffer {
public:
	CircularBuffer(){}
	virtual ~CircularBuffer(){
		if(this->buffer!=nullptr){
			delete[] this->buffer;
		}
	}
//	CircularBuffer(const CircularBuffer &other);
//	CircularBuffer& operator=(const CircularBuffer &other);
//	CircularBuffer(CircularBuffer &&other);
//	CircularBuffer& operator=(CircularBuffer &&other);
	bool addData(type data){
			uint32_t nextAddPosition=this->addPosition+1;
			if(nextAddPosition>=size){
				nextAddPosition=0;
			}
			if(nextAddPosition==readPosition){
				return false;
			}
			this->buffer[addPosition]=data;
			this->addPosition=nextAddPosition;
			return true;
	}
	bool getData(type *data){
			uint32_t nextReadPosition=this->readPosition+1;
			if(nextReadPosition>=size){
				nextReadPosition=0;
			}
			if(this->readPosition==addPosition){
				return false;
			}
			*data=this->buffer[readPosition];
			readPosition=nextReadPosition;
			return true;
	}
private:
	const uint32_t bufferSize=size;
	type *buffer=new type[size];
	uint32_t addPosition=0;
	uint32_t readPosition=0;
};
#include "Models/CircularBuffer.cpp"


#endif /* MODELS_CIRCULARBUFFER_H_ */
