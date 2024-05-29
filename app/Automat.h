/*
 * Automat.h
 *
 *  Created on: May 28, 2024
 *      Author: lechu
 */




#ifndef AUTOMAT_H_
#define AUTOMAT_H_

#include "randomRNG.h"
#include "main.h"

class Automat {

public:
	static constexpr uint32_t POLL_PERIOD_MS = 1;


	enum class Phase{
		TURNED_OFF,
		TURNED_ON,
		BUZZING,
		//		SHOT,
	};

	enum class Channel :uint8_t{
		CH1,
		CH2,
		//CH3,
		COUNT,	// znacznik ilości
		None,	// znacznik braku lub wyjści apoza iterator
	};

	static constexpr uint8_t ChannelCount = static_cast<uint8_t>(Channel::COUNT);

	//static constexpr uint8_t ChannelCount(){ return static_cast<uint8_t>(Channel::COUNT); }
	static constexpr Channel firstCh(){ return Channel::CH1; }
	static constexpr Channel nextCh(Channel chn){
		uint8_t next = static_cast<uint8_t>(chn) + 1;
		if (next >= ChannelCount) return Channel::None;
		return static_cast<Channel>(chn);
	}
	static constexpr bool isValid(Channel channel){ return static_cast<uint8_t>(channel) < ChannelCount; }


	struct Rura{
		uint32_t	timer;		// zlicza czas trwania fazy
		uint32_t	phaseTime;		// całkowity czas fazy
		uint16_t	startFreq;	// częstotliwośc początkowa
		uint16_t	stopFreq;	// częstotliwośc końcowa
		Phase		phase;		//
	};

private:

	static inline Rura pipes[ChannelCount] = {};

	static Rura * getRura(Channel channel){
		if (isValid(channel)){
			return &pipes[static_cast<uint8_t>(channel)];
		}else{
			return nullptr;
		}
	}

	static void setFreqSpread(Channel channel){
		Rura * rura = getRura(channel);
		if (rura == nullptr) return;
		rura->startFreq = getRandom(BUZZ_FREQUENCY_MIN, BUZZ_FREQUENCY_MAX);
		rura->stopFreq = getRandom(BUZZ_FREQUENCY_MIN, BUZZ_FREQUENCY_MAX);
	}

	static void setRura(Channel channel, bool newState){
		switch(channel){
		case Channel::CH1:
			HAL_GPIO_WritePin(rura1_GPIO_Port, rura1_Pin, newState ? GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		case Channel::CH2:
			HAL_GPIO_WritePin(rura2_GPIO_Port, rura2_Pin, newState ? GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		default: break;
		}
	}

	static void setSound(bool newState){
		HAL_GPIO_WritePin(sound_GPIO_Port, sound_Pin, newState ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}


	static bool isRuraOn(Channel channel){
		switch(channel){
		case Channel::CH1:
			return HAL_GPIO_ReadPin(rura1On_GPIO_Port, rura1On_Pin) != GPIO_PinState::GPIO_PIN_RESET;
		case Channel::CH2:
			return HAL_GPIO_ReadPin(rura2On_GPIO_Port, rura2On_Pin) != GPIO_PinState::GPIO_PIN_RESET;
		default: break;
		}
	}

	static inline void generateLight(){
		for (Channel chn = firstCh(); isValid(chn); nextCh(chn)){
			Rura * rura = getRura(chn);

			if (isRuraOn(chn)){
				rura->timer++;

				if (rura->timer >= rura->phaseTime){
					rura->timer = 0;
					switch(rura->phase){
					case Phase::TURNED_OFF:	// włączanie buzza
						rura->phase = Phase::BUZZING;
						setFreqSpread(chn);
						rura->phaseTime = getRandom(BUZZ_TIME_MIN_MS, BUZZ_TIME_MAX_MS);
						break;

					case Phase::BUZZING:	// włączanie na stałe
						rura->phase = Phase::TURNED_ON;
						rura->phaseTime = getRandom(ON_TIME_MIN_MS, ON_TIME_MAX_MS);
						break;

					case Phase::TURNED_ON:	// wyłączanie
						rura->phase = Phase::TURNED_OFF;
						rura->phaseTime = getRandom(OFF_TIME_MIN_MS, OFF_TIME_MAX_MS);
						break;

					default:
						rura->phase = Phase::TURNED_ON;
						rura->phaseTime = getRandom(ON_TIME_MIN_MS, ON_TIME_MAX_MS);
						break;
					}
				}
			}else{
				rura->phase = Phase::TURNED_OFF;
				rura->phaseTime = 100;
				rura->timer = 0;
			}
		}
	}

	static inline void generateSound(){
		bool soundOut = false;
		Rura * rura = nullptr;
		for (Channel chn = firstCh(); isValid(chn); nextCh(chn)){
			Rura * rr = getRura(chn);
			if (rr->phase == Phase::BUZZING){
				rura = rr;
				break;
			}
		}

		if (rura){

			uint32_t Freq1 =  rura->startFreq;
			uint32_t Freq2 =  rura->stopFreq;
			uint32_t time1 =  rura->timer;

			uint32_t f1 = (Freq2 - Freq1);
			f1 *= time1;
			f1 /= rura->phaseTime;
			f1 += Freq1;

			uint32_t t1 = 1000 / f1;	// w milisekundach
			//t1 >> 1;	// przez 2 żeby mieć polowe okresu
			uint32_t polowa_t1 = t1 >> 1;
			if ((time1 % t1) > polowa_t1){
				soundOut = true;
			}
		}

		setSound(soundOut);
	}



public:

	static inline void init(){
		rng_init();
	}

	static inline void poll(){
		generateLight();
		generateSound();
	}

};

#endif /* AUTOMAT_H_ */
