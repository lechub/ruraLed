/*
 * Led.h
 *
 *  Created on: 24 sty 2014
 *      Author: lechu
 */

#ifndef LED_H_
#define LED_H_

#include "Gpio.h"

class Led :public Gpio{
public:
	typedef	enum {SWIECI, ZGASZONA, MRUGA_WOLNO, MRUGA_FAST, PULSUJE, PULSUJE_NIEROWNO} Mode;
	typedef	enum {ACTIVEHIGH, ACTIVELOW} Type;

protected:
	uint8_t ticks;
	Mode tryb;
	Type typ;
public:
	Led(Gpio::GpioPortName gpioName, uint8_t pinNr2, Type type, Mode mode):
		Gpio(gpioName, pinNr2),
		ticks(0), tryb(mode), typ(type)
	{;}

	void init(){
		setMode(Gpio::GpioMode::OUTPUT_PUSH_PULL);
		setSpeed(Gpio::GpioSpeed::LowSpeed);
	}

	void set(Mode sw){tryb = sw;}
	void turnOn(void){set(SWIECI); tick();}
	void turnOff(void){set(ZGASZONA); tick();}
	void tick(void){
		ticks++;
		if (ticks >= 8) ticks = 0;

		switch (tryb){
		case SWIECI:
			setOutputHigh();
			break;
		case ZGASZONA:
			setOutputLow();
			break;
		case MRUGA_WOLNO:
			if (ticks < 4) setOutputHigh(); else setOutputLow();
			break;
		case MRUGA_FAST:
			if ((ticks == 1) || (ticks == 2) || (ticks == 5) || (ticks == 6)) setOutputHigh();
			else setOutputLow();
			break;
		case PULSUJE:
			if ((ticks == 2) || (ticks == 6)) setOutputLow(); else setOutputHigh();
			break;
		case PULSUJE_NIEROWNO:
			if ((ticks == 1) || (ticks == 5) || (ticks == 7)) setOutputLow(); else setOutputHigh();
			break;
		default:
			tryb = MRUGA_FAST;
			break;
		}
	}
};

#endif /* LED_H_ */
