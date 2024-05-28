/*
 * watchDog.h
 *
 *  Created on: May 28, 2024
 *      Author: lechu
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "systemDefs.h"

#ifndef DEBUG
#define WATCHDOG_ENABLE 1
#endif

#define IWDG_KEY_RELOAD                 0x0000AAAAU  /*!< IWDG Reload Counter Enable   */
#define IWDG_KEY_ENABLE                 0x0000CCCCU  /*!< IWDG Peripheral Enable       */
#define IWDG_KEY_WRITE_ACCESS_ENABLE    0x00005555U  /*!< IWDG KR Write Access Enable  */
//#define IWDG_KEY_WRITE_ACCESS_DISABLE   0x00000000U  /*!< IWDG KR Write Access Disable */
//
#define IWDG_PRESCALER_4                0x00000000U                   /*!< IWDG prescaler set to 4   */
#define IWDG_PRESCALER_8                IWDG_PR_PR_0                  /*!< IWDG prescaler set to 8   */
#define IWDG_PRESCALER_16               IWDG_PR_PR_1                  /*!< IWDG prescaler set to 16  */
#define IWDG_PRESCALER_32               (IWDG_PR_PR_1 | IWDG_PR_PR_0) /*!< IWDG prescaler set to 32  */
#define IWDG_PRESCALER_64               IWDG_PR_PR_2                  /*!< IWDG prescaler set to 64  */
#define IWDG_PRESCALER_128              (IWDG_PR_PR_2 | IWDG_PR_PR_0) /*!< IWDG prescaler set to 128 */
#define IWDG_PRESCALER_256              (IWDG_PR_PR_2 | IWDG_PR_PR_1) /*!< IWDG prescaler set to 256 */


class WatchDog {

public:
	static inline void init(){
	#ifdef WATCHDOG_ENABLE
		DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;     // stop IWD when in debug

		// Enable write access to IWDG_PR and IWDG_RLR registers by writing 0x5555 in KR
		IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;

		// Write to IWDG registers the Prescaler & Reload values to work with
		IWDG->PR = IWDG_PRESCALER_256;		// 32kHz/256 = 125Hz == 8ms
		IWDG->RLR = IWDG_RLR_RL_Msk & (((1000/8) * 2) << IWDG_RLR_RL_Pos);	// 2 sekundy
		//while (IWDG->SR & IWDG_SR_PVU){;}

		// Enable IWDG. LSI is turned on automaticaly
		IWDG->KR =  IWDG_KEY_ENABLE;	// aktywacja IWDG
	#endif	//WATCHDOG_ENABLE
	}

	static inline void reload(){
	#ifdef WATCHDOG_ENABLE
		IWDG->KR =  IWDG_KEY_RELOAD;	// przeladowanie IWDG
	#endif	//WATCHDOG_ENABLE
	}

};

#endif /* WATCHDOG_H_ */
