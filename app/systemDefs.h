/*
 * systemDefs.h
 *
 *  Created on: 9 lis 2018
 *      Author: lechu
 */

#ifndef SYSTEMDEFS_H_
#define SYSTEMDEFS_H_

#ifdef STM32F411
#include "stm32f4xx.h"
#endif


#if defined (STM32F405xx) or defined(STM32F401xE)
#include "stm32f4xx.h"
#endif


#ifdef STM32F401xE
#include "stm32f401xe.h"
#endif

#ifdef STM32F302
#include "stm32f3xx.h"
#endif

#ifdef STM32F405xx
#include "stm32f4xx.h"
//#include "system_stm32f4xx.h"
#endif


#ifdef STM32F072
#include "stm32f0xx.h"
#endif

#ifdef STM32F302x8
#include "stm32f3xx_hal.h"
#endif


#include "inttypes.h"

//********** makra

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define READ_REG_AND_FORGET(REG) ({volatile unsigned int tmp = REG; do{UNUSED(tmp);}while(0);})

//******************

//
//
//#ifdef  USE_FULL_ASSERT
//
///**
// * @brief  The assert_param macro is used for function's parameters check.
// * @param  expr: If expr is false, it calls assert_failed function which reports
// *         the name of the source file and the source line number of the call
// *         that failed. If expr is true, it returns no value.
// * @retval None
// */
//#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
///* Exported functions ------------------------------------------------------- */
//void assert_failed(uint8_t* file, uint32_t line);
//#else
//#define assert_param(expr) ((void)0)
//#endif /* USE_FULL_ASSERT */



#endif /* SYSTEMDEFS_H_ */
