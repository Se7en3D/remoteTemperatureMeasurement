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

#endif /* GLOBALDATA_H_ */
