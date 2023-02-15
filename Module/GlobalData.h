/*
 * GlobalData.h
 *
 *  Created on: 30 sty 2023
 *      Author: Daniel
 */

#ifndef GLOBALDATA_H_
#define GLOBALDATA_H_

namespace STM32F4_SYSCFG{
    const unsigned int * const UID31_0=reinterpret_cast<unsigned int*>(0x1FFF7A10);/**<Bajt od 0 do 31 zawierjący unikalny numer identyfikacyjny dla STM32F4*/
    const unsigned int * const UID63_32=UID31_0+0x04;/**<Bajt od 32 do 63 zawierjący unikalny numer identyfikacyjny dla STM32F4*/
    const unsigned int * const UID95_64=UID31_0+0x08;/**<Bajt od 64 do 95 zawierjący unikalny numer identyfikacyjny dla STM32F4*/
};

/**
 * @struct Stm32UniqueDeviceID
 * @brief Struktura zawierająca unikalny numer identyfikacyjny dla STM32
 *
 */
struct Stm32UniqueDeviceID{
	unsigned int id[3]{0};/**<Tablica zawierająca unikalny numer identyfikacyjny*/
};

inline Stm32UniqueDeviceID stm32UniqueDeviceID;/**<Globalna zmienna dla struktury @ref Stm32UniqueDeviceID*/

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
	setTime,/**<Typ określjacy ramkę ustawiająca date*/
};
/**
 * @struct decodedFrame
 * @param Struktura przechowująca strukturę która została odebrana
 * odkodowana struktura jest przechowywana w skaźniku na void
 */
struct DecodedFrame{
	int ErrorValue=-1;/**<Kod błędu(0-Brak błędu )*/
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
#endif /* GLOBALDATA_H_ */
