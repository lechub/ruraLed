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
#include "QuickTask.h"


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
		CH3,
		COUNT,	// znacznik ilości
		None,	// znacznik braku lub wyjści apoza iterator
	};

	static constexpr uint8_t ChannelCount = static_cast<uint8_t>(Channel::COUNT);

	//static constexpr uint8_t ChannelCount(){ return static_cast<uint8_t>(Channel::COUNT); }
	static constexpr Channel firstCh(){ return Channel::CH1; }
	static inline  Channel nextCh(Channel chn){
		uint8_t next = static_cast<uint8_t>(chn) + 1;
		if (next >= ChannelCount) return Channel::None;
		return static_cast<Channel>(next);
	}
	static constexpr bool isValid(Channel channel){ return static_cast<uint8_t>(channel) < ChannelCount; }


	struct Rura{
		uint32_t	timer;		// zlicza czas trwania fazy
		uint32_t	phaseTime;		// całkowity czas fazy
		uint16_t	freq;	// częstotliwośc początkowa
//		uint16_t	stopFreq;	// częstotliwośc końcowa
		Phase		phase;		//
		bool		light;		// stan - świeci/nie świeci
		uint8_t		counter;	// dp ziczania powtorzen blyskow
		uint8_t		maxCounter;	// ilosc blyskow
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
		rura->freq = getRandom(BUZZ_FREQUENCY_MIN, BUZZ_FREQUENCY_MAX);
//		rura->stopFreq = getRandom(rura->freq, BUZZ_FREQUENCY_MAX);
	}

	static void setRura(Channel channel, bool newState){
		switch(channel){
		case Channel::CH1:
			HAL_GPIO_WritePin(rura1_GPIO_Port, rura1_Pin, newState ?  GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		case Channel::CH2:
			HAL_GPIO_WritePin(rura2_GPIO_Port, rura2_Pin, newState ?  GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		case Channel::CH3:
			HAL_GPIO_WritePin(rura3_GPIO_Port, rura3_Pin, newState ?  GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		default: break;
		}
	}

	static void setSound(bool newState){
		HAL_GPIO_WritePin(sound_GPIO_Port, sound_Pin, newState ?  GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	static inline bool isSound1Freq(){
		return HAL_GPIO_ReadPin(sound1Freq_GPIO_Port, sound1Freq_Pin) != GPIO_PinState::GPIO_PIN_SET;
	}

	static bool isRuraOn(Channel channel){
		switch(channel){
		case Channel::CH1:
			return HAL_GPIO_ReadPin(rura1On_GPIO_Port, rura1On_Pin) != GPIO_PinState::GPIO_PIN_RESET;
		case Channel::CH2:
			return HAL_GPIO_ReadPin(rura2On_GPIO_Port, rura2On_Pin) != GPIO_PinState::GPIO_PIN_RESET;
		case Channel::CH3:
			return HAL_GPIO_ReadPin(rura3On_GPIO_Port, rura3On_Pin) != GPIO_PinState::GPIO_PIN_RESET;
		default: break;
		}
		return false;
	}

	static inline void generateLight(){
		for (Channel chn = firstCh(); isValid(chn); chn = nextCh(chn)){
			Rura * rura = getRura(chn);

			switch(rura->phase){
			case Phase::BUZZING:{
//				uint32_t Freq1 =  rura->freq;
//				uint32_t Freq2 =  rura->stopFreq;
//				uint32_t time1 =  rura->timer;

				uint32_t f1 = rura->freq;	//(Freq2 - Freq1);
//				f1 *= time1;
//				f1 /= rura->phaseTime;
//				f1 += Freq1;

				uint32_t t1 = 1000 / f1;	// w milisekundach
				//t1 >> 1;	// przez 2 żeby mieć polowe okresu
				uint32_t onTime = t1/4;
				if (t1 == 0){
					t1 = 1;
				}
				rura->light = (rura->timer % t1) < onTime;
			}break;

			case Phase::TURNED_ON:
				rura->light = true;
				break;
			case Phase::TURNED_OFF:
				rura->light = false;
			default:
				rura->light = false;
				break;
			}

			setRura(chn, rura->light);
		}
	}

	static inline void calculateTimes(){

		for (Channel chn = firstCh(); isValid(chn); chn = nextCh(chn)){
			Rura * rura = getRura(chn);

			if (isRuraOn(chn)){
				rura->timer++;

				if (rura->timer >= rura->phaseTime){
					rura->timer = 0;
					switch(rura->phase){
					case Phase::TURNED_OFF:	// włączanie buzza
						rura->phase = Phase::BUZZING;
						rura->counter++;
						setFreqSpread(chn);
						rura->phaseTime = getRandom(BUZZ_TIME_MIN_MS, BUZZ_TIME_MAX_MS);
						break;

					case Phase::BUZZING:	// włączanie na stałe
						if (rura->counter < rura->maxCounter){
							rura->phase = Phase::TURNED_OFF;
							rura->phaseTime = getRandom(OFF_TIME_MIN_MS, OFF_TIME_MAX_MS);
						}else{
							rura->phase = Phase::TURNED_ON;
							rura->phaseTime = getRandom(ON_TIME_MIN_MS, ON_TIME_MAX_MS);
						}

						break;

					case Phase::TURNED_ON:	// wyłączanie
						rura->phase = Phase::TURNED_OFF;
						rura->maxCounter = getRandom(BUZZ_COUNT_MIN, BUZZ_COUNT_MAX);
						rura->counter = 0;
						rura->phaseTime = getRandom(OFF_TIME_MIN_MS, OFF_TIME_MAX_MS);
						break;

					default:
						rura->phase = Phase::TURNED_ON;
						rura->phaseTime = getRandom(ON_TIME_MIN_MS, ON_TIME_MAX_MS);
						break;
					}
				}
			}else{
				rura->phase = Phase::TURNED_ON;
				rura->phaseTime = 100;
				rura->timer = 0;
			}
		}
	}

	static inline void generateSound(){
		bool soundOut = false;
		Rura * rura = nullptr;



		for (Channel chn = firstCh(); isValid(chn); chn = nextCh(chn)){
			Rura * rr = getRura(chn);
			if (rr->phase == Phase::BUZZING){
				if (isSound1Freq()){
					soundOut = (QuickTask::getCounter() % 50) > 25;
				}else{
					rura = rr;
					soundOut = rura->light;
				}
				break;
			}else{
				soundOut = false;
			}

		}

		setSound(soundOut);
	}



public:

	static inline void init(){
		rng_init();
		HAL_GPIO_WritePin(in1gnd_GPIO_Port, in1gnd_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(in2gnd_GPIO_Port, in2gnd_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(in3gnd_GPIO_Port, in3gnd_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(soundGnd_GPIO_Port, soundGnd_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(gnd5_GPIO_Port, gnd5_Pin, GPIO_PIN_RESET);
	}

	static inline void poll(){
		calculateTimes();
		generateLight();
		generateSound();
	}

};

#endif /* AUTOMAT_H_ */
