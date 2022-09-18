/*
 * TempSensor.cpp
 *
 *  Created on: 4 wrz 2022
 *      Author: Daniel
 */
#include <stdio.h>
#include <TempSensor.h>

TempSensor::TempSensor() {
	// TODO Auto-generated constructor stub

}

TempSensor::~TempSensor() {
	// TODO Auto-generated destructor stub
}

uint32_t TempSensor::Initialize(){return false;}
void TempSensor::StartOfMeasurment(){};
void TempSensor::TimeHandler(){};
int *TempSensor::GetTempValue(){ return nullptr;}
void TempSensor::SetCommunication(TempSensComms *tempSensComms){}
void TempSensor::SetResolution(uint8_t resolution){};
