/*
 * Automat.h
 *
 *  Created on: May 28, 2024
 *      Author: lechu
 */



static void pseudoPwm(){

}




#ifndef AUTOMAT_H_
#define AUTOMAT_H_

class Automat {

public:
	static constexpr uint32_t POLL_PERIOD_MS = 1;

	enum class ChannelState{
		TURNED_OFF,
		TURNED_ON,
		BUZZING,
//		SHOT,
	};

	inline ChannelState channel1 = TURNED_OFF;
	inline ChannelState channel2 = TURNED_OFF;

private:


public:
	static inline void poll(uint32_t milis){

		switch(channel1){
		case ChannelState::TURNED_OFF:
		case ChannelState::TURNED_ON:
		case ChannelState::BUZZING:
//		case ChannelState::SHOT:
		default:
			break;
		}

	}

};

#endif /* AUTOMAT_H_ */
