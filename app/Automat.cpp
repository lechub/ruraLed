/*
 * Automat.cpp
 *
 *  Created on: May 28, 2024
 *      Author: lechu
 */

#include <Automat.h>



QuickTask pseudoPwmQtsk = QuickTask(QuickTask::QT_PERIODIC, Automat::poll,  POLL_PERIOD_MS);
