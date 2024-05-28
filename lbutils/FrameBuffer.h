/*
 * FrameBuffer.h
 *
 *  Created on: 11 wrz 2018
 *      Author: lechu
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <stdint.h>
#include <string.h>
#include "Fifo.h"
#include "myUtils.h"

class FrameBuffer {
public:

  typedef enum {
    HIDDEN, BLINK, SOLID, DASH} CursorMode;

private:
    uint32_t	cursorPosition;	// 0=lewy gorny, kolejne to

    uint32_t COLUMNS;// = 16;
    uint32_t ROWS;// = 2;
    uint32_t BUFFERSIZE;// = ROWS*COLUMNS;


    static constexpr uint32_t MAX_TEMP_BUFFER = 100UL;

    //uint8_t bytes[COLUMNS][ROWS];
    uint8_t * bytes;

    CursorMode	cursor;

    bool        refreshNeed;  //

    void inline setRefreshNeed(){
      refreshNeed = true;
    }

    void inline clearRefreshNeed(){
      refreshNeed = false;
    }

    //    void checkCursorPosition(){
    //      uint32_t oldCur = cursorPosition;
    //      if (cursorPosition >= BUFFERSIZE){
    //        cursorPosition = 0;
    //      }
    ////      cursorPosition = cursorPosition % BUFFERSIZE;
    //      if (cursorPosition != oldCur){
    //        setRefreshNeed();
    //      }
    //    }


public:

    FrameBuffer(uint32_t maxColumns, uint32_t maxRows, uint8_t *buffer) {
      COLUMNS = maxColumns;
      ROWS = maxRows;
      bytes = buffer;
      BUFFERSIZE = ROWS*COLUMNS;
      cursorPosition = 0;
      cursor = CursorMode::HIDDEN;
      clearRefreshNeed();
    }

    virtual ~FrameBuffer();

    inline uint32_t getROWS()const {return ROWS;}
    inline uint32_t getCOLUMNS()const {return COLUMNS;}

    inline uint8_t * getBuffer()const { return bytes;}

    bool	getLine(uint32_t lineNr, Fifo * destination){
      if (lineNr >= getROWS()) return false;
      uint32_t lineLength = getCOLUMNS();
      uint32_t offset = lineNr * lineLength;
      for (uint32_t i = 0; i<lineLength; i++){
        destination->put(*(getBuffer() + offset + i));
      }
      return true;
    }

    // zwraca numer aktualnej kolumny i wiersza, licząc od zera
    void getCursor(uint32_t &column, uint32_t &row){
      if (cursorPosition >= BUFFERSIZE){ // niewłaściwa wartość
        cursorPosition = 0;
      }
      column = cursorPosition % getCOLUMNS();
      row = cursorPosition / getCOLUMNS();
    }

    bool isCursorAtEnd(){
      return cursorPosition >= BUFFERSIZE;
    }

    bool isCursorAtStart(){
      return cursorPosition == 0;
    }

    // drukuje do konca ekranu i dalej juz nie. Jesli to koniec ekranu, to zwroci false, w przeciwnym wypadku true
    bool printToEnd(const char * str){
      //      while(*str != '\0'){
      //        if (isCursorAtEnd()){
      //          print(*str);
      //          return false;
      //        }
      //        print(*str++);
      //      }
      while(*str != '\0'){
        if (isCursorAtEnd()){
          return false;
        }else{
          print(*str++);
        }
      }
      return true;
    }

    // wypełnia ekran od kursora do końca znakiem 'znak"
    void printFillToEnd(char znak){
      while(!isCursorAtEnd()){
        print(znak);
      }
      //      while(isCursorAtStart()){
      //        print(znak);
      //      }
    }


    void cursorRight(void){
      setRefreshNeed();
      cursorPosition++;
      if (cursorPosition > BUFFERSIZE){
        cursorPosition = 1;
      }
    }

    void cursorRight(uint32_t charsNr){
      while (charsNr-- > 0){
        cursorRight();
      }
    }

    void cursorLeft(void){
      setRefreshNeed();
      cursorPosition = (cursorPosition == 0) ? BUFFERSIZE-1 : cursorPosition -1;
    }

    void cursorLeft(uint32_t charsNr){
      while (charsNr-- > 0){
        cursorLeft();
      }
    }

    bool gotoXY(uint32_t col, uint32_t row){
      if (row >= ROWS) return false;
      if (col >= COLUMNS) return false;
      uint32_t oldCur = cursorPosition;
      cursorPosition = row * COLUMNS + col;
      if (oldCur != cursorPosition){
        setRefreshNeed();
      }
      return true;
    }

    // drukowanie znaku i przesuniecie kursora
    void print(char znak){      //checkCursorPosition();
      if (cursorPosition >= BUFFERSIZE){
        cursorPosition = 0;
      }
      *(bytes + cursorPosition) = znak;
      cursorRight();
      setRefreshNeed();
    }


    void print(const char * str){
      while(*str != '\0'){
        print(*str++);
      }
    }

    void printXY(uint32_t col, uint32_t row, const char * str){
      gotoXY(col, row);
      print(str);
    }

    void clearScreen(){
      for (uint32_t i = 0; i <= BUFFERSIZE; i++)
        *(bytes + i) = ' ';
      setRefreshNeed();
    }

    void clearLine(uint16_t lineNr){
      if (lineNr >= getROWS()) return;
      uint32_t offset = lineNr * getCOLUMNS();
      for (uint32_t i = 0; i <= getCOLUMNS(); i++)
        *(bytes + i + offset) = ' ';
      setRefreshNeed();
    }

    void homeScreen(void){
      if (cursorPosition != 0){
        setRefreshNeed();
      }
      cursorPosition = 0;
    }


    void cursorMode(CursorMode mode){
      cursor = mode;
    }

    bool printNumbersWithPattern(const char *pattern, uint32_t value){
      char buff[MAX_TEMP_BUFFER];
      if (strlen(pattern) >= MAX_TEMP_BUFFER) return false;
      bool result = numberWithPattern(pattern, value, (char*)buff);
      print((const char*)buff);
      return result;
    }

    /**
     * @brief Zapisuje liczby według wzorca
     * np.pattern= "[0000]" i initValues="#<>B" zapisze "[#<>B]"
     * znaki specjalne:
     * 0 - znaczace liczby wlacznie z wiodacymi zerami
     *
     *@param pattern wskaznik do tekstu - szablonu, w ktorym bedzie umieszczony wynik
     *@param initValue  liczba, ktora bedzie skonwertowana na ASCII i umieszczona we wzorze
     * @return true, jesli wszystkie cyfry sie zmiescily
     */
    bool printSymbolsWithPattern(const char *pattern, const char * values, const char MARKER){
      uint32_t valIndex = 0;
      while (*pattern != '\0'){
        char znak = *pattern;
        if (znak == MARKER){
          znak = values[valIndex++];
        }
        print(znak);
        pattern++;
      }
      return true;
    }


};

#endif /* FRAMEBUFFER_H_ */
