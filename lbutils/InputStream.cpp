/*
 * InputStream.cpp
 *
 *  Created on: 5 maj 2021
 *      Author: lechu
 */

#include <InputStream.h>

/** ***************************************************************
 * @brief  Odklada bajt jako dwa znaki Ascii do bufora fifo.
 * Najpierw starsza polowka bajtu ('0'..'F'), potem mlodsza polowka bajtu
 * @param  inValue	Bajt do zapisania
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze
 */
bool InputStream::putByteAs2AsciiBCD(uint8_t inValue){
	bool retVal = true;
	uint8_t val = (inValue >> 4) & 0xff;
	if (val > 9) put(val + 'A' - 10);
	else put(val + '0');
	val = inValue & 0x0f;
	if (val > 9){ if (!put(val + 'A' - 10)) retVal = false; }
	else { if (!put(val + '0')) retVal = false; }
	return retVal;
}

/** ***************************************************************
 * @brief  Odklada slowo 2 bajtowe jako 4 znaki Hex Ascii do bufora fifo, poczawszy od nastarszego bajtu
 * @param  inValue	Wartosc do zapisania
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze
 */
bool InputStream::put2BytesAs4AsciiBCD(uint16_t inValue){
	putByteAs2AsciiBCD(inValue >> 8);
	return putByteAs2AsciiBCD(inValue);
}

/** ***************************************************************
 * @brief  Przepisuje wszystkie bajty ze zrodla do swojego bufora
 * @param  src	Obiekt Fifo z ktorego beda pobierane bajty. Nie pozostana one w buforze zrodlowym
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze lub bufor zrodlowy to nullptr
 */
bool InputStream::putAll(OutputStream * src){
	if (src == nullptr) return false;
	bool result = true;
	while (src->isNotEmpty()){
		if (!put(src->get())) result = false;
	}
	return result;
}

/** ***************************************************************
 * @brief  Przepisuje dokładną ilość bajtów ze zrodla do swojego bufora
 * @param  src	Obiekt Fifo z ktorego beda pobierane bajty. Odczyatne znikną z bufora zrodlowego
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo bajtów lub miejsca w buforze lub bufor zrodlowy to nullptr
 */
bool InputStream::put(OutputStream * src, uint32_t amount){
	if (src == nullptr) return false;
	bool result = true;
	for (uint32_t i = 0; i < amount; i++){
		if (!put(src->get())) result = false;
	}
	return result;
}

/** ***************************************************************
 * @brief  Dodaje tekst zakonczony zerem do bufora. (bez konczacego zera)
 * @param  string	Tekst zakonczony \0
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze
 */
bool InputStream::putString(const char * string){
	bool result = true;
	while (*string != '\0'){
		if (!put(*string++)) result = false;
	}
	return result;
}

/** ***************************************************************
 * @brief  Dodaje tekst zakonczony zerem do bufora. (bez konczacego zera) z limitem bajtow
 * @param  string	Tekst zakonczony \0
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze albo gdy przesłano "limit" bajtow
 */
bool InputStream::putString(const char * string, uint32_t limit){
	uint8_t znak;
	for ( ; limit > 0; limit--){
		znak = *string++;
		if (znak == '\0') return true;	// string mniejszy niz limit
		if (!put(znak)) return false;	// fifo juz pelne
	}
	if (*string == '\0') return true;	// string ma dokladnie limit bajtow
	return false;						// string jest dluzszy niz limit wiecej sie nie da
}


/** ***************************************************************
 * @brief  odklada do bufora dwa bajty - starszy a potem mlodszy bajt slowa 16-bitowego
 * @param  value slowo do zapisania do bufora
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze
 */
bool InputStream::putUint16(uint16_t value){
	if (!put(value>>8)) return false;
	if (!put(value)) return false;
	return true;
}

/** ***************************************************************
 * @brief  odklada do bufora 4 bajty poczawszy od najstarszego ze slowa 32-bitowego
 * @param  value slowo do zapisania do bufora
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze
 */
bool InputStream::putUint32(uint32_t value){
	if (!putUint16(value>>16)) return false;
	if (!putUint16(value)) return false;
	return true;
}


/** ***************************************************************
 * @brief  Dodaje tekst będący reprezentacją liczby.
 * @param  string	Tekst ascii niezakonczony \0
 * @return True jesli operacja sie powiodla, lub false, gdy zabraklo miejsca w buforze
 */
bool InputStream::putNumber(int32_t number){
	char buf[22];
	uint16_t digits = my_itoa(number, buf);
	char * txt = buf;
	while(digits-- > 0){
		char c = *txt++;
		if (c == '\0'){
			return true;
		}
		if(!put(c)) return false;
	}
	return true;
}



