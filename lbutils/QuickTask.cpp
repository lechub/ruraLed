/*
 * QuickTask.cpp
 *
 *  Created on: 19 lis 2018
 *      Author: lechu
 */


#include "QuickTask.h"

QuickTask * QuickTask::taskbase = 0;
volatile uint32_t QuickTask::milisCounter;
volatile bool QuickTask::holded = true;


void QuickTask::tick(uint32_t milis){
  QuickTask::milisCounter += milis;
  QuickTask * t = taskbase;
  while (t != 0){
    if (t->flags == QuickTask::QT_RUNNING){
      if (t->timectr > milis) t->timectr -= milis;
      else {
        t->flags = QuickTask::QT_STOPPED;
        t->timectr = 0;
      }
    }
    t = t->next;
  }
}


void QuickTask::poll(){
  if (holded) return;
  QuickTask * tsk = taskbase;
  while (tsk != nullptr){
    if (tsk->flags == QuickTask::QT_STOPPED){
      switch (tsk->type){
      case QuickTask::QT_PERIODIC:{
        tsk->timectr = tsk->updateValue;
        tsk->flags = QuickTask::QT_RUNNING;
        tsk->qCallback();// wykonanie zadania
      }
      break;
      case QuickTask::QT_ONCE:{
        tsk->flags = QuickTask::QT_DISABLED;
        tsk->qCallback();// wykonanie zadania
      }
      break;
      default:
        break;
      }
    }
    tsk = tsk->next;
  }
}

// tworzy nowy task i dodaje do kolejki tasków.
QuickTask::QuickTask(QTType typ, void (*const task)(), const uint32_t updateMilis){
  flags = QT_DISABLED;
  type = typ;
  qCallback = task;
  updateValue = updateMilis;
  if (taskbase == 0){	// jeszcze nie ma zadnych timemeterow, wiec ten jest pierwszy
    taskbase = this;
    taskbase->next = 0;
    flags = (updateMilis != TIME_INFINITY) ? QT_RUNNING : QT_DISABLED;
    return;
  }

  QuickTask * tmp = taskbase;	// kolejka timemeterow

  for(; tmp->next != 0; tmp = tmp->next) {
    if (tmp == this) return;	// jest juz w kolejce timerow
  }

  if (tmp == this) return;	// dodatkowy test dla ostatniego
  // Jednak ten timer nie jest na liscie - trzeba go dolozyc
  next = 0;
  tmp->next = this;
  flags = (updateMilis != TIME_INFINITY) ? QT_RUNNING : QT_DISABLED;
}


//#ifdef USE_HAL_DRIVER
//void HAL_SYSTICK_Callback();
//#else
//void SysTick_Handler(void) __attribute__ ((interrupt));
//#endif

//-------------------------------------------------------

#ifdef USE_HAL_DRIVER
void HAL_SYSTICK_Callback(){
#else
  void SysTick_Handler(void){
#endif

    QuickTask::tick(QuickTask::DEFAULT_TICK_INTERVAL_MILIS);
  }

  //-------------------------------------------------------


