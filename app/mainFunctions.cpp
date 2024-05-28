/*
 * mainFunctions.cpp
 *
 *  Created on: May 28, 2024
 *      Author: lechu
 */

#include "mainFunctions.h"

//#include "RS485.h"
#include "QuickTask.h"
//#include "Hardware.h"
//#include "ConnectionAux.h"
//#include "WS2812B.h"
//#include "Automat.h"
//#include "Sygnalizacja.h"
//#include "Pinout.h"
//#include "Animation.h"
#include "watchDog.h"
#include "Automat.h"

//QuickTask ledDrvQtsk = QuickTask(QuickTask::QT_PERIODIC, ws2812b_callback,  WS2812B::TIME_POLL_INTERVAL_MS);

//QuickTask animationQtsk = QuickTask(QuickTask::QT_PERIODIC, Animation::poll, Animation::TIME_POLL_INTERVAL_MS );	// co 100 ms zmniejszamy "power"

QuickTask watchDogQtsk = QuickTask(QuickTask::QT_PERIODIC, WatchDog::reload,  1000);


int userInit(){
//	Hardware::init(); // CPU, freq, IRQ
	QuickTask::hold(true);
	WatchDog::init();

//	Pinout * pins = Pinout::getInstance();
//	pins->init();

	QuickTask::delayMsWithStoppedTasks(600);

//	RS485 * rs = RS485::getInstance(USART1);
//	rs->setup(RS485::USART_BAUD_RATE, &pins->rs485Dir);
//	ConnectionAux * aux = ConnectionAux::getInstance();
//	aux->init(rs);

//	Sygnalizacja::init();
	//enkoder.init(&pins->in1, &pins->in2, POWER_MAXIMUM);
//	WS2812B::getInstance()->init();
//	Animation::init();
//	Automat::getInstance()->init();
	//StepperMotor::getMotor()->init(&pins->Out1, &pins->Out2, &pins->Out3, &pins->Out4, StepperMotor::StepType::STEP_HALF);
	QuickTask::hold(false);
	return 1;
}

void mainPoll(){
	QuickTask::poll();                // przegląd i uruchamianie tasków
//	ConnectionAux::getInstance()->poll();	// z usarta od razu przetwarzamy
}
