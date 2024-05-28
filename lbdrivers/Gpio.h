/*
 * Gpio.h
 *
 *  Created on: 14.04.2021
 *      Author: lechu
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "settings.h"

class Gpio {

public:
	typedef enum {
		PortA = 0, PortB = 1, PortC, PortD, PortE, PortF, PortG, PortH
	}GpioPortName;

	typedef enum{
		AF0,AF1,AF2,AF3,AF4,AF5,AF6,AF7,AF8,AF9,AF10,AF11,AF12,AF13,AF14,AF15,
	}AlternateFunction;

	typedef enum {
		LowSpeed		= 0b00,	//LowSpeed
		MediumSpeed		= 0b01,
		HighSpeed		= 0b10,
		MaximumSpeed	= 0b11,	//HighSpeed
	}GpioSpeed;

protected:
	GpioPortName	gpio;
	uint8_t	pinNr;


	static inline GPIO_TypeDef * getHwGpio(GpioPortName gpioPort){
		switch(gpioPort){
		case PortA: return GPIOA;
		case PortB: return GPIOB;
		case PortC: return GPIOC;
		case PortD: return GPIOD;
		case PortE: return GPIOE;
		case PortH: return GPIOH;
		//case PortF:
		default: return GPIOH;
		}
	}


	typedef enum { //MODER
		INPUT 			= 0b00,
		OUTPUT			= 0b01,
		ALTERNATE		= 0b10,
		ANALOG 			= 0b11,
	}StMode;

	typedef enum {	//OTYPER
		PushPull 	= 0b0,
		OpenDrain 	= 0b1,
		NoMatter	= 0b1,
	}StOType;

	typedef enum {
		NoPull		= 0b00,
		PullUp		= 0b01,
		PullDown 	= 0b10,
	}StPuPd;


	// STM


	static inline void setMODER(GPIO_TypeDef* gpioReg, uint8_t pin, const StMode mode){
		uint32_t moder = gpioReg->MODER;
		moder  &= ~(0b11UL << (pin * 2));
		moder |= ((uint32_t)mode << (pin * 2));
		gpioReg->MODER = moder;
	}

	static inline void setStSpeed(GPIO_TypeDef* gpioReg, uint8_t pin, const GpioSpeed speed){
		uint32_t ospeedr = gpioReg->OSPEEDR;
		ospeedr &= ~(0b11UL << (pin * 2));
		ospeedr |= ((uint32_t)(speed) << (pin * 2));
		gpioReg->OSPEEDR = ospeedr;
	}

	static inline void setStPullUpDn(GPIO_TypeDef* gpioReg, uint8_t pin, const StPuPd pupd){
		uint32_t pupdr = gpioReg->PUPDR;
		pupdr &= ~(0b11UL << ((uint16_t)pin * 2));
		pupdr |= (((uint32_t)pupd) << (pin * 2));
		gpioReg->PUPDR = pupdr;
	}

	/* Output mode configuration */
	static inline void setStOType(GPIO_TypeDef * gpioReg, uint8_t pin, const StOType otype){
		if (otype == StOType::PushPull){
			gpioReg->OTYPER &= (~(1UL << (pin)));
		}else{
			gpioReg->OTYPER |= (1UL << (pin));
		}
	}

	static inline StMode getMODER(GPIO_TypeDef* gpioReg, uint8_t pin){
		uint32_t tmp = gpioReg->MODER >>(pin * 2);
		return static_cast<StMode>(tmp & 0b11UL);
	}

	static inline StPuPd getPull(GPIO_TypeDef* gpioReg, uint8_t pin){
		uint32_t tmp = gpioReg->PUPDR >>(pin * 2);
		return static_cast<StPuPd>(tmp & 0b11UL);
	}

	static inline GpioSpeed getSpeed(GPIO_TypeDef* gpioReg, uint8_t pin){
		uint32_t tmp = gpioReg->OSPEEDR >>(pin * 2);
		return static_cast<GpioSpeed>(tmp & 0b11UL);
	}

	static inline StOType getOType(GPIO_TypeDef* gpioReg, uint8_t pin){
		return static_cast<StOType>((gpioReg->OTYPER >> pin) & 1UL);
	}

	static inline void setup(GPIO_TypeDef* gpioReg, uint8_t pin, StMode mode, StOType oType, StPuPd pupd, GpioSpeed speed){
		setMODER(gpioReg, pin, mode);
		setStOType(gpioReg, pin, oType);
		setStPullUpDn(gpioReg, pin, pupd);
		setStSpeed(gpioReg, pin, speed);
	}

	static inline void setStOutputUp(GPIO_TypeDef* gpioReg, uint8_t pin){
		gpioReg->BSRR = (1<<pin);
	}
	static inline void setStOutputDown(GPIO_TypeDef* gpioReg, uint8_t pin){
		gpioReg->BSRR = (1<<(pin + 16l));
	}

	static inline void setStOutput(GPIO_TypeDef* gpioReg, uint8_t pin, bool newstate){
		if (newstate){
			setStOutputUp(gpioReg, pin);
		}else{
			setStOutputDown(gpioReg, pin);
		}
	}

	static inline bool getStOutput(GPIO_TypeDef* gpioReg, uint8_t pin){
		return (gpioReg->ODR & (1<<pin));
	}

	static inline void toggleStOutput(GPIO_TypeDef* gpioReg, uint8_t pin){
		getStOutput(gpioReg, pin) ? setStOutputDown(gpioReg, pin) : setStOutputUp(gpioReg, pin);
	}


	static inline bool getStInput(GPIO_TypeDef* gpioReg, uint8_t pin){
		return (gpioReg->IDR & (1<<pin));
	}

	static void setupFromCloneSt(GPIO_TypeDef* gpioRegDest, uint8_t pinDest, GPIO_TypeDef* gpioRegSrc, uint8_t pinSrc){
		setMODER(gpioRegDest, pinDest, getMODER(gpioRegSrc, pinSrc));
		setStOType(gpioRegDest, pinDest, getOType(gpioRegSrc, pinSrc));
		setStPullUpDn(gpioRegDest, pinDest, getPull(gpioRegSrc, pinSrc));
		setStSpeed(gpioRegDest, pinDest, getSpeed(gpioRegSrc, pinSrc));
	}

	static bool setAlternateFuncSt(GPIO_TypeDef* gpioReg, uint8_t pin, uint8_t alternateFunction){
		if (alternateFunction > 7) return false;
		constexpr uint32_t BitMask = 0b00111; // najstarszy zarezerwowany
		uint32_t val = alternateFunction;
		if (pin < 8){
			uint8_t offset = uint8_t(pin * 4);
			uint32_t tmp = gpioReg->AFR[0];
			tmp &= ~(BitMask << offset);
			tmp |= val << offset;
			gpioReg->AFR[0] = tmp;
		}
		if ((pin >= 8)&&(pin < 16)){
			uint8_t offset = uint8_t((pin - 8) * 4);
			uint32_t tmp = gpioReg->AFR[1];
			tmp &= ~(BitMask << offset);
			tmp |= val << offset;
			gpioReg->AFR[1] = tmp;
		}
		return true;
	}

	static inline uint8_t getAlternativeFuncSt(GPIO_TypeDef* gpioReg, uint8_t pin){
		constexpr uint32_t BitMask = 0b00111;
		uint32_t tmp = 0;
		if (pin < 8){
			tmp = (gpioReg->AFR[0]) >> (pin * 4);
		}
		if ((pin >= 8)&&(pin < 16)){
			tmp = (gpioReg->AFR[1]) >> ((pin - 8) * 4);
		}
		return uint8_t(tmp  & BitMask);
	}


public:

	typedef enum {
		INIT_STATE = 0,			//   DIR 0  INEN 0  PULLEN 0  OUT x
		OUTPUT_OPEN_D,			//   DIR 0  INEN 0  PULLEN 0  OUT x
		OUTPUT_OPEN_D_W_PULL,	//   DIR 0  INEN 0  PULLEN 1  OUT 1
		INPUT_NOPULL,			//   DIR 0  INEN 1  PULLEN 0  OUT x
		INPUT_W_PULL_DOWN,		//   DIR 0  INEN 1  PULLEN 1  OUT 0
		INPUT_W_PULL_UP,		//   DIR 0  INEN 1  PULLEN 1  OUT 1

		OUTPUT_PUSH_PULL,		//   DIR 1  INEN 0  PULLEN 0  OUT --
		INPUT_ANALOG,
		UNUSED,
		ALTERNATIVE,		//   DIR 0  INEN 0  PULLEN 0
	}GpioMode;


	Gpio(GpioPortName gpioPort, const uint8_t gioPin)
	: gpio{gpioPort}, pinNr{gioPin} {
	}

	GpioPortName getPortName(){
		return gpio;
	}
	uint8_t	getPinNumber(){
		return pinNr;
	}

	static inline void setOutputLow(GpioPortName gpioPort, uint8_t gioPin){
		setStOutputDown(getHwGpio(gpioPort), gioPin);
	}

	static inline void setOutputHigh(GpioPortName gpioPort, uint8_t gioPin){
		setStOutputUp(getHwGpio(gpioPort), gioPin);
	}

	static inline void setOutputHiZ(GpioPortName gpioPort, uint8_t gioPin){
		GPIO_TypeDef * hw = getHwGpio(gpioPort);
		setStOType(hw, gioPin, StOType::OpenDrain);
		setStOutputUp(hw, gioPin);
	}

	static inline void setOutputLevel(GpioPortName gpioPort, uint8_t gioPin, bool newstate){
		setStOutput(getHwGpio(gpioPort), gioPin, newstate);
	}



	static void setMode(GpioPortName gpioPort, uint8_t gioPin, GpioMode mode){
		GPIO_TypeDef * hw = getHwGpio(gpioPort);
		switch(mode){
		case GpioMode::INPUT_NOPULL:
			setMODER(hw, gioPin, StMode::INPUT);
			setStPullUpDn(hw, gioPin, StPuPd::NoPull);
			break;
		case GpioMode::INPUT_W_PULL_UP:
			setMODER(hw, gioPin, StMode::INPUT);
			setStPullUpDn(hw, gioPin, StPuPd::PullUp);
			break;
		case GpioMode::INPUT_W_PULL_DOWN:
			setMODER(hw, gioPin, StMode::INPUT);
			setStPullUpDn(hw, gioPin, StPuPd::PullDown);
			break;
		case GpioMode::OUTPUT_PUSH_PULL:
			setMODER(hw, gioPin, StMode::OUTPUT);
			setStOType(hw, gioPin, StOType::PushPull);
			setStPullUpDn(hw, gioPin, StPuPd::NoPull);
			break;
		case GpioMode::OUTPUT_OPEN_D_W_PULL:
			setMODER(hw, gioPin, StMode::OUTPUT);
			setStOType(hw, gioPin, StOType::OpenDrain);
			setStPullUpDn(hw, gioPin, StPuPd::PullUp);
			break;
		case GpioMode::OUTPUT_OPEN_D:
			setMODER(hw, gioPin, StMode::OUTPUT);
			setStOType(hw, gioPin, StOType::OpenDrain);
			setStPullUpDn(hw, gioPin, StPuPd::NoPull);
			break;
		case GpioMode::INIT_STATE:
		case GpioMode::INPUT_ANALOG:
			setMODER(hw, gioPin, StMode::ANALOG);
			setStPullUpDn(hw, gioPin, StPuPd::NoPull);
			break;
		case GpioMode::ALTERNATIVE:
			setStPullUpDn(hw, gioPin, StPuPd::NoPull);
			setMODER(hw, gioPin, StMode::ALTERNATE);
			break;
		default:
			break;
		}
	}

	static bool setAlternate(GpioPortName gpioPort, uint8_t gioPin, AlternateFunction alternate){
		GPIO_TypeDef * hw = getHwGpio(gpioPort);
		if (alternate > AlternateFunction::AF15) return false;
		constexpr uint32_t BitMask = 0b00111UL; // najstarszy zarezerwowany
		uint32_t val = alternate;
		if (gioPin < 8){
			uint8_t offset = uint8_t(gioPin * 4);
			uint32_t tmp = hw->AFR[0];
			tmp &= ~(BitMask << offset);
			tmp |= val << offset;
			hw->AFR[0] = tmp;
		}else if ((gioPin >= 8)&&(gioPin < 16)){
			uint8_t offset = uint8_t((gioPin - 8) * 4);
			uint32_t tmp = hw->AFR[1];
			tmp &= ~(BitMask << offset);
			tmp |= val << offset;
			hw->AFR[1] = tmp;
		}else return false;
		return true;
	}


	static void setup(GpioPortName gpioPort, uint8_t gpioPin, GpioMode mode, GpioSpeed speed){
		setStSpeed(getHwGpio(gpioPort), gpioPin, speed);
		setMode(gpioPort, gpioPin, mode);
	}

	static inline bool getOutputLevel(GpioPortName gpioPort, uint8_t gpioPin){
		return getStOutput(getHwGpio(gpioPort), gpioPin);
	}

	static inline bool getInputLevel(GpioPortName gpioPort, uint8_t gpioPin){
		return getStInput(getHwGpio(gpioPort), gpioPin);
	}

	static inline void toggleOutput(GpioPortName gpioPort, uint8_t gpioPin){
		bool lvl = getOutputLevel(gpioPort, gpioPin);
		setOutputLevel(gpioPort, gpioPin, !lvl);
	}


	static void setupFromClone(GpioPortName gpioDest, uint8_t pinDest, GpioPortName gpioSrc, uint8_t pinSrc){
		GPIO_TypeDef * src = getHwGpio(gpioSrc);
		GPIO_TypeDef * dst = getHwGpio(gpioDest);
		setupFromCloneSt(dst, pinDest, src, pinSrc);
	}



	inline void setOutputHigh(){
		setOutputHigh(gpio, pinNr);
	}

	inline void setOutputLow(){
		setOutputLow(gpio, pinNr);
	}

	inline void setOutputHiZ(){
		setOutputHiZ(gpio, pinNr);
	}


	inline void setOutputLevel(bool newstate){
		setOutputLevel(gpio, pinNr, newstate);
	}

	void setMode(const GpioMode mode){
		setMode(gpio, pinNr, mode);
	}

	void setSpeed(const GpioSpeed speed){
		setStSpeed(getHwGpio(gpio), pinNr, speed);
	}

	void setAlternative(AlternateFunction alternate){
		setAlternate(gpio, pinNr, alternate);
	}


	void setup(GpioMode mode, GpioSpeed speed){
		setup(gpio, pinNr, mode, speed);
	}

	inline bool getOutputLevel(){
		return getOutputLevel(gpio, pinNr);
	}

	inline bool getInputLevel(){
		return getInputLevel(gpio, pinNr);
	}

	inline bool isInputHigh(){
		return getInputLevel(gpio, pinNr);
	}

	inline bool isInputLow(){
		return !getInputLevel(gpio, pinNr);
	}

	inline void toggleOutput(){
		toggleOutput(gpio, pinNr);
	}

	void setupFromClone(Gpio * srcGpio){
		setupFromClone(this->gpio, this->pinNr, srcGpio->gpio, srcGpio->pinNr);
	}



};

#endif /* GPIO_H_ */
