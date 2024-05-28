/*
 * OutputStream.h
 *
 *  Created on: 5 maj 2021
 *      Author: lechu
 */

#ifndef OUTPUTSTREAM_H_
#define OUTPUTSTREAM_H_

#include "stdint.h"
#include "myUtils.h"
#include "Stream.h"


class OutputStream :public Stream {
public:

	typedef enum{
		NoConvert,
		ToLower,
		ToUpper,
	}CharOutput;

	virtual ~OutputStream();

	/** ***************************************************************
	 * @brief  Pobiera 1 bajt z bufora.
	 * @return Zwraca znak (bajt) lub BUFFER_EMPTY_FLAG_U16, gdy pusty bufor
	 */
	virtual uint16_t get() = 0;

	/** ***************************************************************
	 * @brief  Podgląda 1 bajt z bufora, nie zdejmując go z końca bufora.
	 * @return Zwraca znak (bajt) lub BUFFER_EMPTY_FLAG_U16, gdy pusty bufor
	 */
	virtual uint16_t peek() = 0;

	/** ***************************************************************
	 * @brief  Pobiera z bufora 2 znaki i interpretuje je jako liczby Hex w zapisie Ascii
	 * @return Zwraca znak (bajt) lub BUFFER_EMPTY_FLAG_U16, gdy pusty bufor lub pobrane znaki nie sa liczbami Hex
	 */
	uint16_t getByteAs2HexAscii();

	uint32_t get2BytesAs4HexAscii();

	/** ***************************************************************
	 * @brief  Pobiera 1 slowo 16-bitowe z bufora.
	 * Czyli dwa kolejne bajty - pierwszy jako starszy bajt i drugi jako mlodszy
	 * @return Zwraca slowo (16-bitowe) lub BUFFER_EMPTY_FLAG_U32, gdy pusty bufor
	 */
	uint32_t getUInt16();

	/** ***************************************************************
	 * @brief  Ściąga(pomija) z bufora 'amount' bajtów.
	 * @return Zwraca true, jeśli pominięto 'amount' bajtów, albo false, jeśli tyle nie było w buforze - wtedy bufor zostaje pusty
	 */
	bool skip(uint32_t amount){
		while (amount > 0){
			uint16_t data = get();
			if (data > 0xff) return false;
		}
		return true;
	}

	/** ***************************************************************
	 * @brief  Pobiera 1 slowo 32-bitowe z bufora.
	 * Czyli 4 kolejne bajty - pierwszy jako najstarszy bajt
	 * @param  result	Wskaznik do rezultatu operacji
	 * @return Zwraca slowo (32-bitowe). Poprawna operacja zwraca w *result true, niepoprawna false
	 */
	uint32_t getUInt32(bool *result);

	// -----------------------------------------------------------------
	/**	Podaje ilosc bajtow czekajacych w buforze.
	 */
	virtual uint32_t get_count() = 0;


	// -----------------------------------------------------------------
	bool isEmpty() { return get_count() == 0; }

	bool isNotEmpty(){ return get_count() != 0; }

	//virtual bool isFull() = 0;
	// -----------------------------------------------------------------
	/**	Zeruje bufor - ustawia pusty.
	 */
	virtual void flush() = 0;


	// zwraca bajt po konwersji z BCD na bin
	// gdy blad to liczba wieksza od 0x0f
	inline uint8_t get1BcdAsHalfByte(){
		uint16_t data =  get();
		if (data > 0x0ff) return BUFFER_UNDERRUN_FLAG_U8;
		return AsciiBcdToValue(static_cast<uint8_t>(data));
	}

	uint16_t get2BcdAs1Byte();
	uint32_t get4BcdAs2Bytes();

	uint32_t getAsString(char * dest, uint32_t maxCount, CharOutput convertOpts);

	uint8_t getHexDigit();
	uint32_t getHexadecimal(uint32_t maxdigits);
	uint32_t getDecimal(uint32_t maxdigits);

};


#endif /* OUTPUTSTREAM_H_ */
