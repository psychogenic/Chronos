/*
 * timeExtInc.h
 * External includes.
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 19, 2015
 *      Author: Pat Deegan
 *      Part of the Chronos library project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 * 
 *  This file is part of the Chronos embedded datetime/calendar library.
 * 
 *     Chronos is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     Chronos is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Lesser Public License for more details.
 * 
 *    You should have received a copy of the GNU Lesser Public License
 *    along with Chronos.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHRONOS_INTINCLUDES_TIMEEXTINC_H_

#define CHRONOS_INTINCLUDES_TIMEEXTINC_H_

#include "../chronosinc/ChronosConfig.h"


#include <inttypes.h>

#ifdef CHRONOS_PLATFORM_ARDUINO
#include <Arduino.h>
#include <Print.h>
#include <HardwareSerial.h>
#endif

#ifdef CHRONOS_CLOCKSOURCE_TIMELIB
#include <Time.h>
#endif



#endif /* CHRONOS_INTINCLUDES_TIMEEXTINC_H_ */
