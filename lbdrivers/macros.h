/*
 * macros.h
 *
 *  Created on: 23 sty 2014
 *      Author: lechu
 */

#ifndef MYSETTINGS_H_
#define MYSETTINGS_H_

#include <inttypes.h>	// uint32_t itd.

// definicje dla jezyka C
#ifndef __cplusplus

#ifndef false
#define false	(1==0)
#endif

#ifndef true
#define true	(!false)
#endif

#endif


// makro zwracajace ustawiony bit na okreslonej pozycji
#ifndef _BV
#define _BV(bit)	(1<<bit)
#endif

#define STR(string) ((const char*)(string))


//********** makra do manipulacji bitami

#define BIT_SET(REG, BIT)     ((REG) |= (BIT))

#define BIT_CLEAR(REG, BIT)   ((REG) &= ~(BIT))

#define BIT_READ(REG, BIT)    ((REG) & (BIT))

#define REG_CLEAR(REG)        ((REG) = (0x0))

#define REG_WRITE(REG, VAL)   ((REG) = (VAL))

#define REG_READ(REG)         ((REG))

#define REG_MODIFY(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define REG_READ_AND_FORGET(REG) ({volatile unsigned int tmp = REG; do{UNUSED(tmp);}while(0);})


#endif /* MACROS_H_ */
