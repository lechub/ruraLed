/*
 * VVar.h
 *
 *  Created on: 16 sty 2020
 *      Author: lechu
 */

#ifndef EPROMVAR_H_
#define EPROMVAR_H_

#include <settings.h>

class EpromVar {

public:
  typedef enum{
    INIT = 0,
    SoftVersion,
    LICZNIK,
    NAPED,
    PASSWORD,
    FLAGS1,
    Adres,
    OPOZN_ZAMK_POZAR,
    KLAW_BISTABILNA,
    AUTO_OTWARCIE_PO_POZARZE,
    OVC_LEVEL,        // poziom zadziałania OVC od 0..9 -> 0=wyłączony, 9=od razu po wykryciu
    CZAS_OTWIERANIA,
    Checksum,       // potem usunac
    COUNT,            // licznik zmiennych - zostawic zawsze na końcu!
  }Address;


  static uint32_t getDefaultValue(Address adr){
     if (adr >= Address::COUNT) return 0;
     switch(adr){
     case SoftVersion: return SOTWARE_VERSION;
     //    case NAPED: return 0;
     //    case PASSWORD: return 0;
     case Adres: return 1;

     default: return 0;
     }
   }


};

#endif /* EPROMVAR_H_ */
