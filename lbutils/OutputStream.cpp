/*
 * OutputStream.cpp
 *
 *  Created on: 5 maj 2021
 *      Author: lechu
 */

#include <OutputStream.h>

OutputStream::~OutputStream() {
}

/** ***************************************************************
 * @brief  Pobiera z bufora 2 znaki i interpretuje je jako liczby Hex w zapisie Ascii
 * @return Zwraca znak (bajt) lub 0xffff, gdy pusty bufor lub pobrane znaki nie sa liczbami Hex
 */
uint16_t OutputStream::getByteAs2HexAscii(){
	uint16_t val1, val2;
	val1 = get();
	val2 = get();
	if ((val1 == BUFFER_EMPTY_FLAG_U16)||(val2 == BUFFER_EMPTY_FLAG_U16)) return 0xffff;
	return AsciiBcd2toByte(val1, val2);
}

/** ***************************************************************
 * @brief  Pobiera z bufora 2 znaki i interpretuje je jako liczby Hex w zapisie Ascii
 * @return Zwraca znak (bajt) lub 0xffffffff, gdy pusty bufor lub pobrane znaki nie sa liczbami Hex
 */
uint32_t OutputStream::get2BytesAs4HexAscii(){
	uint16_t val1 = getByteAs2HexAscii();
	uint16_t val2 = getByteAs2HexAscii();
	if ((val1 == 0xffff)||(val2 == 0xffff)) return 0xffffffff;
	return (val1 << 8)| val2;
}


/** ***************************************************************
 * @brief  Pobiera 1 slowo 16-bitowe z bufora.
 * Czyli dwa kolejne bajty - pierwszy jako starszy bajt i drugi jako mlodszy
 * @return Zwraca slowo (16-bitowe) lub BUFFER_EMPTY_FLAG_U32, gdy pusty bufor
 */
uint32_t OutputStream::getUInt16(){
	uint16_t tmp = get();
	if (tmp == BUFFER_EMPTY_FLAG_U16) return BUFFER_EMPTY_FLAG_U32;
	uint16_t tmp2 = get();
	if (tmp2 == BUFFER_EMPTY_FLAG_U16) return BUFFER_EMPTY_FLAG_U32;
	return (tmp << 8) | tmp2;
}

/** ***************************************************************
 * @brief  Pobiera 1 slowo 32-bitowe z bufora.
 * Czyli 4 kolejne bajty - pierwszy jako najstarszy bajt
 * @param  result	Wskaznik do rezultatu operacji
 * @return Zwraca slowo (32-bitowe). Poprawna operacja zwraca w *result true, niepoprawna false
 */
uint32_t OutputStream::getUInt32(bool *result){
	uint32_t tmp = getUInt16();
	if (tmp == BUFFER_EMPTY_FLAG_U32){ *result = false; return 0;}
	uint32_t tmp2 = getUInt16();
	if (tmp2 == BUFFER_EMPTY_FLAG_U32){ *result = false; return 0;}
	return (tmp << 16) | tmp2;
}


/** ***************************************************************
 * @brief Z 2 kolejunych znakow BCD sklada liczbe 1-bajtowa
 * Czyli 2 kolejne bajty - pierwszy jako najstarszy bajt
 * @return liczba, a gdy blad to liczba wieksza od 0x0ff
 */
uint16_t OutputStream::get2BcdAs1Byte(){
	if (get_count() < 2) return 0xefff;
	uint8_t znakHi = get1BcdAsHalfByte();
	uint8_t znakLo = get1BcdAsHalfByte();
	if ((znakHi > 0x0f)||(znakLo > 0x0f)) return 0xffff;
	return (uint16_t)((znakHi << 4) | znakLo);
}

/** ***************************************************************
 * @brief Z 4 kolejunych znakow BCD sklada liczbe 2-bajtowa
 * Czyli 4 kolejne bajty - pierwszy jako najstarszy bajt
 * @param  result
 * @return liczba, a gdy blad to liczba wieksza od 0x0ffff
 */
uint32_t OutputStream::get4BcdAs2Bytes(){
	uint16_t znakHi = get2BcdAs1Byte();
	uint16_t znakLo = get2BcdAs1Byte();
	if ((znakHi > 0xff)||(znakLo > 0xff)) return BUFFER_UNDERRUN_FLAG_U32;
	return ((znakHi << 8) | znakLo);
}

/** ***************************************************************
 * @brief  Zapisuje do pamięci do maxCount znaków z bufora.
 * W trakcie zapisu można skonwertowac znaki na małe lub duże litery
 * Operacja jest przerywana przed maxCount bajtów, jeśli jest koniec bufora lub zapisano znak '\0'
 * @param dest Bufor do ktorego odbedzie sie zapis
 * @param maxCount maksymalna ilośc danych do zapisu w buforze
 * @param convertOpts opcja konwersji w trakcie zapisu
 * @return ilość zapisanych bajtów
 */
uint32_t OutputStream::getAsString(char * dest, uint32_t maxCount, CharOutput convertOpts){
	if (isEmpty()) return 0;
	uint32_t count = maxCount;
	while (count-- > 0){
		char c = get();
		switch(convertOpts){
		case CharOutput::ToLower:
			c = tolower(c);
			break;
		case CharOutput::ToUpper:
			c = toupper(c);
			break;
		case CharOutput::NoConvert:
		default:
			break;
		}
		*dest++ = c;
		if ((c == '\0')||(isEmpty())){		// wymusić koniec
			break;
		}
	}
	return maxCount - count;
}


/** ***************************************************************
 * @brief  Pobiera kolejne 'digits' znakow z bufora lub aż napotka koniec lub znak nie będący cyfrą ASCII
 * @return Liczba - wynik
 */
uint32_t OutputStream::getDecimal(uint32_t digits){
	uint32_t result = 0;
	for (uint32_t i = 0; i < digits; i++){
		if (!isNotEmpty()){
			break;
		}
		uint8_t znak = static_cast<uint8_t>(get());
		if (isdigit(znak)){
			znak -= '0';
			result = (result * 10) + znak;
		}else{
			break;
		}
	}
	return result;
}

uint8_t OutputStream::getHexDigit(){
	uint16_t znak = get();
	if (znak > 0x00ff){
		return 0xff;
	}
	if (isdigit(znak)){
		return znak - '0';
	}
	znak = tolower(znak);
	if ((znak < 'a')||(znak > 'f')){
		return 0xff;
	}
	return znak - 'a' + 10;
}

uint32_t OutputStream::getHexadecimal(uint32_t digits){
	uint32_t result = 0;
	for (uint32_t i = 0; i < digits; i++){
		if (!isNotEmpty()){
			return result;
		}
		uint8_t znak = getHexDigit();
		if (znak == 0xff){
			return 0;
		}
		result <<= 4;
		result |= static_cast<uint32_t>(znak);
	}
	return result;
}


