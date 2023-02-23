/*
 * FramesDecoder.h
 *
 *  Created on: 11 lut 2023
 *      Author: Daniel
 */

#ifndef COMMUNICATIONWITHPC_FREAMES_FRAMESDECODER_H_

#include <string>
#include "GlobalData.h"

#define COMMUNICATIONWITHPC_FREAMES_FRAMESDECODER_H_

/**
 * @struct DecodedFrameSetTimeandDate
 * @brief Struktura przechowująca date wraz z godziną
 */
struct DecodedFrameSetTimeandDate{
	int year;/**<Rok*/
	int month;/**<Miesiąc*/
	int day;/**<Dzień*/
	int hour;/**<Godzina*/
	int min;/**<Minuta*/
	int sec;/**<Sekunda*/
	/**
	 * @brief Metoda przygotowujące dane zawarte w strukturze
	 * @return std::string Zawartość klasy
	 */
	std::string toString(){
		std::string tempString="DecodedFrameSetTimeandDate[year=";
		tempString.append(std::to_string(year));
		tempString+=" month=";
		tempString.append(std::to_string(month));
		tempString+=" day=";
		tempString.append(std::to_string(day));
		tempString+=" hour=";
		tempString.append(std::to_string(hour));
		tempString+=" min=";
		tempString.append(std::to_string(min));
		tempString+=" sec=";
		tempString.append(std::to_string(sec));
		tempString+="]";
		return tempString;

	}
};
/**
 * @enum decodedFrameType
 * @brief Typ wyliczeniowy zawierający typ zdekodowanej wiadomości
 */
enum class decodedFrameType{
	unkown,/**<Nieznany typ*/
	onlyCode,/**<Typ określający ramkę zawierającą tylko kod odpowiedzi*/
	setTime,/**<Typ określający ramkę ustawiająca date*/
};

/**
 * @struct decodedFrame
 * @param Struktura przechowująca strukturę która została odebrana
 * odkodowana struktura jest przechowywana w skaźniku na void
 */
struct DecodedFrame{
	int ErrorValue=-1;/**<Kod błędu(0-Brak błędu )*/
	int StatusCode=-1;/**<Kod statusu*/
	decodedFrameType type=decodedFrameType::unkown;/**<Ustandaryzowany typ wiadomości określający zawartość wskaźnika*/
	void *result=nullptr;/**<Wskażnik na klasę którą należy rzutować na odpowiedni typ zgodnie z wartością pola @ref ErrorValue*/
	~DecodedFrame(){
		if(result!=nullptr){
			//TODO wykonać rzutowanie na możliwy przechowywany typ
			switch(this->type){
			case decodedFrameType::setTime:{
				DecodedFrameSetTimeandDate *decodedFrameSetTimeandDate=static_cast<DecodedFrameSetTimeandDate *>(this->result);
				if(decodedFrameSetTimeandDate!=nullptr){
					delete decodedFrameSetTimeandDate;
				}
				this->result=nullptr;
			}break;
			default:
				break;
			}
		}
	}
};

class FramesDecoder {
public:
	virtual int decodeFrame(std::string &inputFrame,DecodedFrame* decodedFrame)=0;

private:
};

#endif /* COMMUNICATIONWITHPC_FREAMES_FRAMESDECODER_H_ */
