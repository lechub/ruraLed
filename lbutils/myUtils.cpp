/*
 * myUtils.c
 *
 *  Created on: 14-02-2013
 *      Author: lechu
 */

#include "myUtils.h"

/* ***************************************************
 *	@brief Konwerter Integer do Ascii.
 *	Dziala rowniez dla liczb ujemnych.
 *	Nalezy pamietac o odpowiednio duzym buforze:
 *	- dla liczb 8-bitowych - min. 4 bajty
 *	- dla liczb 16-bitowych - min. 6 bajtow
 *	- dla liczb 32-bitowych - min. 21 bajtow
 *	@param val	Liczba do konwersji
 *	@param buf	Wskaznik do bufora wynikowego
 *	@return		Ilosc zajetych znakow w buforze, nie liczac terminujacego zera.
 * */
uint16_t my_itoa(int32_t val, char *buf){
	uint16_t minus	= 0;	// jesli ujemna to zajmie wiecej w buforze - o znak '-'
	if (val < 0){		// detekcja minusa
		*buf++ = '-';	// bedzie ujemna
		val = - val;	// wiec zmienic na dodatnia
		minus++;
	}
	uint16_t cyfr = 0;	// ilosc zwracanych cyfr
	do{
		buf[cyfr++] = val%10;	// reszta z dzielenia przez 10 to nasza cyferka
		val = val/10;			// reszte podzielic
	}while (val > 0);	// dopoki jest co dzielic

	buf[cyfr] = 0;	// terminator zwracanego stringa

// teraz sa w odwrotnej kolejnosci jako bcd
	uint16_t i = 0, dlug = cyfr - 1; //    //	if (dlug > 0) dlug--;
	uint8_t tmp;
	do{
		if (dlug == i){			// tu juz nie trzeba nic zamieniac
			buf[i++] += '0';	// tylko zamiana bcd na ASCII
		}
		else{					// zamiana miejscami
			tmp = buf[dlug];	// zapamietac
			buf[dlug--] = '0' + buf[i];	// zamienic pierwsza na ostatnia
			buf[i++] = '0' + tmp;	// a ostatnia na pierwsza
		}
	}
	while (dlug >= i);	// juz wiecej cyfr nie ma
	return cyfr + minus;
}


/* ***************************************************
 *	@brief Konwerter zapisujacy bajt jako dwia znaki Ascii reprezentujące liczbę Hex.
 *	Np. val = 26 wygeneruje "1A\0"
 *	@param val	Liczba do konwersji
 *	@param buf	Wskaznik do bufora wynikowego
 * */
void my_byteToAsciiHex(uint8_t val, char *buf){
	uint8_t half = val >> 4;
	half = half > 9 ? half - 10 + 'A' :  half + '0';
	*buf++ = half;
	half = val & 0x0f;
	half = half > 9 ? half - 10 + 'A' :  half + '0';
	*buf++ = half;
	*buf = '\0';
}


/* ***************************************************
 *	@brief Konwerter zapisujacy slowo Hex w postaci szeregu Ascii.
 *	Nalezy pamietac o odpowiednio duzym buforze - minimum 9 bajtow:
 *	@param val	Liczba do konwersji
 *	@param buf	Wskaznik do bufora wynikowego
 * */
void my_htoa(uint32_t val, char *buf){
	uint16_t znak;
	for (uint8_t i = 0; i<8; i++){
		znak = (val>>(28 -(i<<2))) & 0x0f;
		*buf++ = (znak > 9)? (znak + 'A') - 10 : znak + '0';
	}
	*buf = '\0';
}

/* ***************************************************
 *	@brief Konwerter zapisujacy bajt w postaci szeregu zer i jedynek Ascii.
 *	Nalezy pamietac o odpowiednio duzym buforze - minimum 9 bajtow:
 *	@param val	Liczba do konwersji
 *	@param buf	Wskaznik do bufora wynikowego
 * */
void my_btoa(uint8_t val, char *buf){
	for (uint8_t i = 0; i<8; i++, val <<= 1){
		*buf++ = (val & 0x80)?'1':'0';
	}
	*buf = 0;
}

/**
 * @brief Zapisuje liczby według wzorca
 * np.pattern= "#00:00.00" i initValue=12345 zapisze "01:23.45"
 * znaki specjalne:
 * 0 - znaczace liczby wlacznie z wiodacymi zerami
 * # - nieznaczace wiodace zera - pomijane
 *
 *@param pattern wskaznik do tekstu - szablonu, w ktorym bedzie umieszczony wynik
 *@param initValue	liczba, ktora bedzie skonwertowana na ASCII i umieszczona we wzorze
 * @return true, jesli wszystkie cyfry sie zmiescily
 */

bool itoaWithPattern(char * const pattern, uint32_t initValue){
	const char patternChar = '0';
	const char patternCharInv = '#';
	char * txt = pattern;
	while (*txt != '\0'){ txt++; }		// wyszukanie konca wzoru
	while(txt > pattern){				// dzialanie od konca do poczatku
		txt--;
		if  ((*txt == patternChar)||(*txt == patternCharInv)) {
			uint8_t cyfra = (uint8_t)(initValue % 10);
			initValue /= 10;
			if ((cyfra == 0) && (initValue == 0)){
				if (*txt == patternCharInv)	*txt = ' ';	// spacja dla niewidocznych zer
			}
			else *txt = (char)(cyfra + '0');

		}	// else{ ;} // nic nie zapisuj, tylko przesun
	}
	return initValue == 0;
}




uint32_t power10(uint32_t power){
	if (power == 0) return 1;
	uint32_t result = 1;
	while (power-- > 0){ result *= 10; }
	return result;
}

char * my_strcpy (char * to, const char * from, bool withTerminator){
	uint8_t znak;
	while (true){
		znak = *from++;
		if (znak == '\0'){
			if (withTerminator){
				*to++ = znak;
			}
			break;
		}
		*to++ = znak;
	}
	return to;
}


/**
 * Wyswietla liczby według wzorca
 * np.pattern= "000:00.00" i initValue=123456 wyswietli "012:34.56"
 */
bool numberWithPattern(const char * pattern, uint32_t initValue, char * buffer){
	my_strcpy(buffer, pattern, true);	// skopiowanie wzoru razem z terminacja
	return itoaWithPattern(buffer, initValue);
}

bool my_atoi(const char * buffer, int32_t * result){
  while(*buffer != '\0'){
    if (!isspace(*buffer)) break;
    buffer++;
  }
  bool minus = false;
  if (*buffer == '-'){
    minus = true;
    buffer++;
  }
  int32_t val = 0;
  while(*buffer != '\0'){
    if (!isdigit(*buffer)){
      return false;
    }
    if (isspace(*buffer)){
      break;
    }
    val *= 10;
    val += (*buffer) - '0';
    buffer++;
  }
  if (minus) val = - val;
  *result = val;
  return true;
}


char inline toHexDigit(uint8_t n){
    if (n < 10) {
        return n + '0';
    } else {
        return (n - 10) + 'A';
    }
}

bool inline fromHexDigit(char n, uint8_t *result){
    if ((n >= '0')&&(n <= '9')) {
        *result = static_cast<uint8_t>(n - '0');
        return true;
    }
    if ((n >= 'a')&&(n <= 'f')) {
        *result = static_cast<uint8_t>(n - 'a' + 10);
        return true;
    }
    if ((n >= 'A')&&(n <= 'F')) {
        *result = static_cast<uint8_t>(n - 'A' + 10);
        return true;
    }
    return false;
}

bool my_htoi(const char * buffer, uint32_t * result){
  while(*buffer != '\0'){
    if (!isspace(*buffer)) break;
    buffer++;
  }
  if (*buffer == '0'){
    buffer++;
  }
  if ((*buffer == 'x')||(*buffer == 'X')){
    buffer++;
  }
  uint32_t val = 0;
  while(*buffer != '\0'){
    if (!isxdigit(*buffer)){
      return false;
    }
    if (isspace(*buffer)){
      break;
    }
    val <<= 4;
    uint8_t tmp;
    if (!fromHexDigit(*buffer, &tmp)){
    	return false;
    }
    val |= tmp;
    buffer++;
  }
  *result = val;
  return true;
}


bool isStringsEqual(const char * str1, const char *str2, bool isCaseSensitive){
	while(*str1 != '\0'){
		if (isCaseSensitive){
			if (*str1 != *str2) return false;
		}else{
			if (toupper(*str1) != toupper(*str2)) return false;
		}
		str1++; str2++;
	}
	return *str2 == '\0';
}

