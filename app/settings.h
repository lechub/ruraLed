/*
 * Settings.h
 *
 *  Created on: 20 marzec 2020
 *      Author: lechu
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

/************** DESCRIPTION *****************/
/* program dla sterownika do udawania zepsutych świetlówek
 * Makieta dla Archimod
 * data 2024.05.28
 * */

#include "systemDefs.h"


#ifdef __cplusplus
extern "C" {
#endif

/************** WERSJA HARDWARE *****************/
/* wersja hardware 1  - na bazie STM32F302R8T6 - NUCLEO */
#define HARDWARE_VERSION 01		//

#define HARDWARE_PCB "NUCLEO-F302R8"

/************** WERSJA SOFTWARE *****************/

//**** wersja programu

#define PROG_MAIN_VERSION 1
/* program dla STM32F302R8T6 -  */

//**** sub-wersje
//
#define PROG_SUB_VERSION 0  // wersja startowa - ruraLed - sterownik zeputych świetlówek


static const uint16_t SOTWARE_VERSION = PROG_MAIN_VERSION * 1000 + PROG_SUB_VERSION;

static const uint32_t TIME_POLL_PRACA_MS = 8;  // poll dla praca, sterownik, krancowki

static inline uint16_t getProgMainVersion(){ return PROG_MAIN_VERSION; }
static inline uint16_t getProgSubVersion(){ return PROG_SUB_VERSION; }
static inline const char * getPCBVersion(){
	static const char * pcb = HARDWARE_PCB;
	return pcb;
//	return reinterpret_cast<const uint8_t *>(pcb);
}


/*	plan działania dla ........
 *
 */


static constexpr uint32_t COLOR_ERROR = 0x010601;	// kolor Led, gdy coś pójdzie nie tak.
static constexpr uint32_t COLOR_OFF = 0x0UL;

static constexpr uint16_t LINE_TEST_LENGTH = 200;

static constexpr uint32_t BUZZ_FREQUENCY_MIN = 10;
static constexpr uint32_t BUZZ_FREQUENCY_MAX = 50;

static constexpr uint32_t BUZZ_TIME_MIN_MS = 2 * 1000;
static constexpr uint32_t BUZZ_TIME_MAX_MS = 8 * 1000;

static constexpr uint32_t ON_TIME_MIN_MS = 3 * 1000;
static constexpr uint32_t ON_TIME_MAX_MS = 15 * 1000;

static constexpr uint32_t OFF_TIME_MIN_MS = 2 * 1000;
static constexpr uint32_t OFF_TIME_MAX_MS = 5 * 1000;


#ifdef __cplusplus
}
#endif

#endif /* SETTINGS_H_ */
