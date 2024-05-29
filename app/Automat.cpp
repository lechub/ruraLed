/*
 * Automat.cpp
 *
 *  Created on: May 28, 2024
 *      Author: lechu
 */

#include <Automat.h>
#include "QuickTask.h"


QuickTask automatQtsk = QuickTask(QuickTask::QT_PERIODIC, Automat::poll,  Automat::POLL_PERIOD_MS);
