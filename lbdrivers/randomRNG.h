/*
 * randomRNG.h
 *
 *  Created on: 5 mar 2022
 *      Author: lechu
 */

#ifndef RANDOMRNG_H_
#define RANDOMRNG_H_

#include "settings.h"

#ifdef STM32F405xx
static inline void rng_init(){
	RCC->AHB2ENR = RCC_AHB2ENR_RNGEN;
	RNG->CR = RNG_CR_RNGEN;
}

static inline uint32_t getRandomU32(){
	uint32_t sr;
	do{
		 sr= RNG->SR;
		//	if((sr & (RNG_SR_CECS | RNG_SR_SECS)) != 0){
		//		rng_init();
		//		return 0;
		//	}
	}while ((sr & RNG_SR_DRDY) == 0);
	return RNG->DR;
}

#else

uint32_t lfsr113_Bits (void);

static inline void rng_init(){
}

static inline uint32_t getRandomU32(){
	return lfsr113_Bits();
}

#endif

static inline uint32_t getRandom(uint32_t minVal, uint32_t maxVal){
	if (minVal >= maxVal) return minVal;
	uint32_t range = maxVal - minVal;
	uint32_t rnd = getRandomU32();
	return minVal + (rnd%range);
}

//uint32_t getRandom(uint32_t minVal, uint32_t maxVal);

#endif /* RANDOMRNG_H_ */


