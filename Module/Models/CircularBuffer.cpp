/*
 * CircularBuffer.cpp
 *
 *  Created on: 26 lut 2023
 *      Author: Daniel
 */

#include "Models/CircularBuffer.h"
//template <uint32_t size, class type>
//CircularBuffer<size,type>::CircularBuffer(){
//	// TODO Auto-generated constructor stub
//
//}
//template <uint32_t size, class type>
//CircularBuffer<size,type>::~CircularBuffer() {
//	if(this->buffer!=nullptr){
//		delete[] this->buffer;
//	}
//}
//template <uint32_t size, class type>
//bool CircularBuffer<size,type>::addData(type data){
//	uint32_t nextAddPosition=this->addPosition+1;
//	if(nextAddPosition>=size){
//		nextAddPosition=0;
//	}
//	if(nextAddPosition==readPosition){
//		return false;
//	}
//	this->buffer[addPosition]=data;
//	this->addPosition=nextAddPosition;
//	return true;
//}
//template <uint32_t size, class type>
//bool CircularBuffer<size,type>::getData(type *data){
//	uint32_t nextReadPosition=this->readPosition+1;
//	if(nextReadPosition>=size){
//		nextReadPosition=0;
//	}
//	if(nextReadPosition>addPosition){
//		return false;
//	}
//	*data=this->buffer[readPosition];
//	readPosition=nextReadPosition;
//	return true;
//}
//template <uint32_t size, class type>
//CircularBuffer<size,type>::CircularBuffer(const CircularBuffer &other) {
//	// TODO Auto-generated constructor stub
//
//}
//template <uint32_t size, class type>
//CircularBuffer<size,type>& CircularBuffer<size,type>::operator=(const CircularBuffer &other) {
//	// TODO Auto-generated method stub
//
//}
//template <uint32_t size, class type>
//CircularBuffer<size,type>::CircularBuffer(CircularBuffer &&other) {
//	// TODO Auto-generated constructor stub
//
//}
//template <uint32_t size, class type>
//CircularBuffer<size,type>& CircularBuffer<size,type>::operator=(CircularBuffer &&other) {
//	// TODO Auto-generated method stub
//}

