/*
 * timeTypes.h
 * Library-scope type definitions.
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 18, 2015
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

#ifndef CHRONOS_INTINCLUDES_TIMETYPES_H_
#define CHRONOS_INTINCLUDES_TIMETYPES_H_

#include "../chronosinc/ChronosConfig.h"

#include "../chronosinc/timeExtInc.h"

namespace Chronos {

typedef uint32_t EpochTime;
typedef uint8_t Seconds;
typedef uint8_t Minutes;
typedef uint8_t Hours;
typedef uint8_t WeekDay;
typedef uint8_t Day;
typedef uint8_t Month;
typedef uint16_t Year;

#ifdef CHRONOS_CLOCKSOURCE_TIMELIB
typedef tmElements_t TimeElements;
#else
typedef struct  {
  uint8_t Second;
  uint8_t Minute;
  uint8_t Hour;
  uint8_t Wday;   // day of week, sunday is day 1
  uint8_t Day;
  uint8_t Month;
  uint16_t Year;   // offset from 1970;
} 	TimeElements;
#endif


namespace Weekday {

typedef enum DayEnum {

	Sunday = 1,
	Monday = 2,
	Tuesday = 3,
	Wednesday = 4,
	Thursday =  5,
	Friday = 	6,
	Saturday =  7,
} Day;

} /* namespace Weekday */


namespace Named {
namespace Month {

typedef enum MonthEnum {

	January = 1,
	February = 2,
	March = 3,
	April = 4,
	May = 5,
	June = 6,
	July = 7,
	August = 8,
	September = 9,
	October = 10,
	November = 11,
	December = 12

} Month;

} /* namespace Month */
} /* namespace Name */

} /* namespace Chronos */

#endif /* CHRONOS_INTINCLUDES_TIMETYPES_H_ */
