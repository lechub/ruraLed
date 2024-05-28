/*
 * InputStream.h
 *
 *  Created on: 5 maj 2021
 *      Author: lechu
 */

#ifndef INPUTSTREAM_H_
#define INPUTSTREAM_H_

#include "OutputStream.h"


class InputStream {

public:

	/** ***************************************************************
	 * @brief  Dodaje 1 bajt do bufora fifo
	 * @param  bajt	Bajt do umieszczenia w buforze
	 * @return true jesli operacja sie powiodla, albo false, jesli bufor jest pelny
	 */
	virtual bool put(uint8_t bajt) = 0;

	bool putAll(OutputStream * src);
	bool put(OutputStream * src, uint32_t amount);

	bool putByteAs2AsciiBCD(uint8_t inValue);
	bool put2BytesAs4AsciiBCD(uint16_t inValue);
	bool putString(const char * string);
	bool putString(const char * string, uint32_t limit);

	bool putUint16(uint16_t value);
	bool putUint32(uint32_t value);

	bool putNumber(int32_t number);
	// -----------------------------------------------------------------
	/**	Podaje ilosc bajtow czekajacych w buforze.
	 */
	virtual uint32_t get_count() = 0;

	// -----------------------------------------------------------------
	/**	Podaje ilosc bajtow mozliwych do zapisania do bufora.
	 */
	virtual uint32_t countFree() = 0;


	virtual bool isFull() = 0;
	// -----------------------------------------------------------------
	/**	Zeruje bufor - ustawia pusty.
	 */
	virtual void flush() = 0;

	virtual ~InputStream(){}

};

#endif /* INPUTSTREAM_H_ */
