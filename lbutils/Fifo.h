/*
 * Fifo.h
 *
 *  Created on: 24 sty 2014
 *      Author: lechu
 */

#ifndef FIFO_H_
#define FIFO_H_

//#include "stdint.h"
//#include "myUtils.h"
#include "InputStream.h"
#include "OutputStream.h"

/**
 * @brief	Klasa realizujaca bufor typu fifo. Moze sluzyc tez do przekazywania pakietow danych i do konwersji typow.
 */
class Fifo : public InputStream, public OutputStream {
	static const uint16_t	FIFO_DEFAULT_SIZE = 300;	// Domyslny rozmiar buforu. Efektywnie wykorzystuje sie 1 bajt mniej
protected:
	uint32_t 	bufSize;	///< Aktualny rozmiar buforu
	volatile	uint32_t in;		///< Pierwszy wolny bajt do zapisu - indeks w buforze (0..bufSize-1)
	volatile	uint32_t out;		///< Ostatni zapisany bajt i pierwszy do odczytu - indeks w buforze
	volatile	uint8_t *bufor;	///< Przechowalnia bajtow, czyli bufor bajtow

public:
//	static constexpr uint32_t BUFFER_EMPTY_FLAG_U32 = 0xffffff00;	///< flaga pustego bufora dla wartosci 16-bitowej
//	static constexpr uint16_t BUFFER_EMPTY_FLAG_U16 = 0xff00;		///< flaga pustego bufora dla wartosci 8-bitowej
	Fifo(uint8_t *buforb, uint32_t size);				///< konstruktor musi otrzymac dane bufora
	 ~Fifo(){}

	/** ***************************************************************
	 * @brief  Dodaje 1 bajt do bufora fifo
	 * @param  bajt	Bajt do umieszczenia w buforze
	 * @return true jesli operacja sie powiodla, albo false, jesli bufor jest pelny
	 */
	virtual bool put(uint8_t bajt);

	/** ***************************************************************
	 * @brief  Pobiera 1 bajt z bufora.
	 * @return Zwraca znak (bajt) lub BUFFER_EMPTY_FLAG_U16, gdy pusty bufor
	 */
	virtual uint16_t get();


	// -----------------------------------------------------------------
	/**	Podaje ilosc bajtow czekajacych w buforze.
	 */
	uint32_t get_count();

	// -----------------------------------------------------------------
	/**	Podaje ilosc bajtow mozliwych do zapisania do bufora.
	 */
	uint32_t countFree(){return bufSize - 1 - get_count();}

	inline bool isEmpty(){
		return (in == out);
	}

	bool isFull();
	// -----------------------------------------------------------------
	/**	Zeruje bufor - ustawia pusty.
	 */
	void flush(){out = in = 0;}

	bool clone(InputStream * destination);

	/** ***************************************************************
	 * @brief  Podgląda 1 bajt z bufora, nie zdejmując go z końca bufora.
	 * @return Zwraca znak (bajt) lub BUFFER_EMPTY_FLAG_U16, gdy pusty bufor
	 */
	uint16_t peek();

};


#endif /* FIFO_H_ */
