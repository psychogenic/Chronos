/*
 * timesource.h
 * Main header used to select time source include.  Currently, only the
 * Arduino Time library is supported.
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 20, 2015
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

#ifndef CHRONOS_INTINCLUDES_PLATFORM_TIMESOURCE_H_
#define CHRONOS_INTINCLUDES_PLATFORM_TIMESOURCE_H_


#include "../ChronosConfig.h"


#ifdef CHRONOS_CLOCKSOURCE_TIMELIB
#include "../../chronosinc/platform/timesourceTimelib.h"
#else
#error "You MUST define a DATETIME_CLOCKSOURCE_* in ChronosConfig.h"

#endif

#ifndef DATETIME_GET_CURRENT_EPOCH
#error "Selected clocksource didn't set DATETIME_GET_CURRENT_EPOCH??"
#endif

#endif /* CHRONOS_INTINCLUDES_PLATFORM_TIMESOURCE_H_ */
