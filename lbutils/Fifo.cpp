/*
 * Fifo.cpp
 *
 *  Created on: 24 sty 2014
 *      Author: lechu
 */

#include "Fifo.h"


// -----------------------------------------------------------------
/** ***************************************************************
 * @brief  Konstruktor bufora fifo
 * @param  bufor	wskaznik do zaalokowanej pamieci dla fizycznego bufora bajtow
 * @param  size		rozmiar przekazywanej pamieci
 */
Fifo::Fifo(uint8_t *buforb, uint32_t size){
	bufSize = size;
	this->bufor = buforb;
	in = out = 0;
}

// -----------------------------------------------------------------
/** ***************************************************************
 * @brief  Dodaje 1 bajt do bufora fifo
 * @param  bajt	Bajt do umieszczenia w buforze
 * @return true jesli operacja sie powiodla, albo false, jesli bufor jest pelny
 */
bool Fifo::put(uint8_t bajt){
	uint32_t tmpIn = in;
	uint32_t tmpOut = out;
	if ((tmpIn + 1) == tmpOut) return false;	//bufor pelny, bo ! Trzeba co najmniej 1 wolny bajt w buforze
	if ((tmpOut == 0)&&(tmpIn == (bufSize-1))) return false; // bufor pelny, indeksy na koncu i poczatku tablicy
	bufor[tmpIn++] = bajt;		// zapis pod indeks in (pierwsze wolne) i inkrementacja indeksu
	in = (tmpIn >= (bufSize))? 0 : tmpIn;	//korekcja indeksu w strukturze
	return true;
}

// -----------------------------------------------------------------
/** ***************************************************************
 * @brief  Pobiera 1 bajt z bufora.
 * @return Zwraca znak (bajt) lub BUFFER_EMPTY_FLAG_U16, gdy pusty bufor
 */
uint16_t Fifo::get(){
	uint32_t tmpOut = out;
	uint32_t tmpIn = in;
	if (tmpOut == tmpIn) return BUFFER_EMPTY_FLAG_U16; //bufor pusty, bo in = out! Zwracamy 0xff00
	uint8_t wynik = bufor[tmpOut++];
	out = (tmpOut >= bufSize)? 0 : tmpOut;
	return wynik;
}


/** ***************************************************************
 * @brief  Zwraca ilosc bajtow czekajacych w buforze.
 * @return Ilosc bajtow znajdujacych sie aktualnie w buforze.
 */
uint32_t Fifo::get_count(){
	uint32_t tmpOut = out;
	uint32_t tmpIn = in;
	if (tmpIn >= tmpOut) return (tmpIn - tmpOut);
	else return ((bufSize - tmpOut) + tmpIn);
}

/** ***************************************************************
 * @brief  Sprawdzenie stanu bufora - czy jest pelny
 * @return True jesli bufor jest pelny, lub false, gdy jeszcze mozna zapisac co najmniej 1 bajt
 */
bool Fifo::isFull(){
	uint32_t tmpOut = out;
	uint32_t tmpIn = in;
	if (++tmpIn == tmpOut) return true;//bufor pelny, bo ! Trzeba co najmniej 1 wolny bajt w buforze
	if ((tmpOut == 0)&&(tmpIn >= bufSize)) return true;// bufor pelny, indeksy na koncu i poczatku tablicy
	return false;
}

/** ***************************************************************
 * @brief  Zapisuje do docelowego bufora zawartosc wszystkich przechowywanych danych.
 * Dane zrodlowe nie sa tracone, poniewaz kazdy przepisywany bajt jest z powrotem dodawany do bufora
 * @param destination Bufor do ktorego odbedzie sie zapis
 * @return True operacja sie powiodla, lub false, gdy zabraklo miejsca w docelowym buforze
 */
bool Fifo::clone(InputStream * destination){
	bool result = true;
	//destination->flush();
	uint8_t znak;
	for (uint32_t dlug = get_count(); dlug > 0; dlug--){
		znak = get();
		put(znak);
		if (!destination->put(znak)) result = false;
	}
	return result;
}

uint16_t Fifo::peek(){
	if (out == in) return BUFFER_EMPTY_FLAG_U16; //bufor pusty, bo in = out! Zwracamy 0xff00
	uint8_t wynik = bufor[out];
	return wynik;
}

